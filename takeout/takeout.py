import boto3
import eqbeats
import os
import redis
import rq
import tempfile
import uuid
from flask import g
from zipfile import ZipFile, ZIP_DEFLATED
from math import floor, log10

bucket = os.getenv("TAKEOUT_S3_BUCKET", "eqbeats-takeout")

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
    #build file array
    files = []
    for track in user.tracks():
        filename = eqbeats.orig_file(track['id'])
        ext = filename.split(".")[-1]
        archive_filename = "%s-%s.%s" % (track['id'], sanitize(track['title']), ext)
        files.append({"path":filename, "name":archive_filename})
    if(len(files) > 0):
        return get_q().enqueue(archive, files, job_id="userarchive:%s" % (user.id,), result_ttl=3600)

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
    value = number / (10 ** 3*unit_n)
    if unit_n > 0:
        return "%s%s" (value, unit)
    else:
        return str(value)
