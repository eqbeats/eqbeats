import boto3
import eqbeats
import os
import redis
import rq
import tempfile
import uuid
import shutil
import requests
import datetime
import csv
from flask import g, request
from zipfile import ZipFile, ZIP_DEFLATED
from math import floor, log10

bucket = os.getenv("TAKEOUT_S3_BUCKET", "eqbeats-takeout")
eqbeats_url = os.getenv("EQBEATS_URL", "https://eqbeats.org/")

def get_redis():
    redis_location = os.getenv("EQBEATS_REDIS", "localhost:6379")
    if redis_location[0] in ('/', '.'):
        conn = redis.Redis(unix_socket_path=redis_location)
    else:
        try:
            host, port = redis_location.split(':')
        except ValueError:
            host = redis_location
            port = 6379
        conn = redis.Redis(host=host, port=port)
    return conn

def get_q():
    try:
        return g.q
    except AttributeError:
        q = g.q = rq.Queue(connection=get_redis())
        return q

def archive_user(user):
    """
    expects an eqbeats.User
    returns a job or None if no tracks were found
    """
    return get_q().enqueue(archive_tracks, user.tracks(), job_id="userarchive:%s" % (user.id,), result_ttl=3600)

def archive_tracks(tracks):
    #build file array
    files = []
    tempdir = tempfile.mkdtemp(prefix="eqbtakeout_")
    for track in tracks:

        # audio file
        filename = eqbeats.orig_file(track['id'])
        if not filename:
            # the track probably failed transcoding, skip it
            continue
        ext = filename.split(".")[-1]
        archive_filename = "%s-%s.%s" % (track['id'], sanitize(track['title']), ext)
        files.append({"path":filename, "name":archive_filename})

        # stats
        r = requests.get("%s/track/%s/stats" % (eqbeats_url, track['id']))
        if not r.status_code == 200:
            continue
        stats = r.json()
        first = last = None

        filename = "%s-%s-stats.csv" % (track['id'], sanitize(track['title']))
        with open(tempdir + "/" + filename, "w", newline='') as f:
            for arr in list(stats['unique_days'].values()) + list(stats['days'].values()):
                for key in arr.keys():
                    if not first or first > key:
                        first = key
                    if not last or last < key:
                        last = key

            first = datetime.datetime.strptime(first, "%Y-%m-%d").date()
            last = datetime.datetime.strptime(last, "%Y-%m-%d").date()
            date = first
            c = csv.writer(f)
            c.writerow(['date', 'visits', 'plays', 'downloads', 'unique visits', 'unique plays', 'unique downloads'])
            while date <= last:
                c.writerow([date.isoformat(),
                    stats['days']['trackView'].get(date.isoformat(), 0),
                    stats['days']['trackPlay'].get(date.isoformat(), 0),
                    stats['days']['trackDownload'].get(date.isoformat(), 0),
                    stats['unique_days']['trackView'].get(date.isoformat(), 0),
                    stats['unique_days']['trackPlay'].get(date.isoformat(), 0),
                    stats['unique_days']['trackDownload'].get(date.isoformat(), 0)
                ])
                date += datetime.timedelta(1)

        files.append({"path": tempdir + "/" + filename, "name": filename})

    result = None
    if(len(files) > 0):
        result = archive(files)
    shutil.rmtree(tempdir)
    return result

def sanitize(string):
    # this filter is a bit overzealous but at least we're guaranteed not to hit any filesystem naming limitations
    sanitized = ""
    for char in string:
        if char in "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_. ":
            sanitized += char
        else:
            sanitized += '-'
        if len(sanitized) >= 100: break
    return sanitized

def archive(files):
    """
    argument expected:
    [
        {"path":"/path/to/file", "name","file_name_in_zip"},
        { ... },
        ...
    ]
    returns a s3 url
    """
    job = rq.get_current_job(get_redis())
    handle, tempname = tempfile.mkstemp()
    os.close(handle)
    with ZipFile(tempname, mode='w', compression=ZIP_DEFLATED) as zipfile:
        for file_ in files:
            zipfile.write(file_['path'], file_['name'])

    job.meta['size'] = si_unit(os.path.getsize(tempname))
    job.save()

    objname = str(uuid.uuid4()) + ".zip"

    s3 = boto3.resource('s3')
    s3.Bucket(bucket).upload_file(tempname, objname, ExtraArgs={'ContentType':'application/zip'})

    os.remove(tempname)

    url = "https://%s.s3.amazonaws.com/%s" % (bucket, objname)
    return url

def si_unit(number):
    units = ".KMGTP"
    unit_n = min(floor(log10(number)/3), len(units))
    unit = units[unit_n]
    value = number // (10 ** (3*unit_n))
    if unit_n > 0:
        return "%s%s" % (value, unit)
    else:
        return str(value)
