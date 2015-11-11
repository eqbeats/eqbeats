import os
import rq
import boto3
import tempfile
from zipfile import ZipFile, ZIP_DEFLATED
import uuid

bucket = os.getenv("TAKEOUT_S3_BUCKET", "eqbeats-takeout")

def archive(files):
    """
    argument expected:
    [
        {"path":"/path/to/file", "name","file_name_in_zip"},
        { ... },
        ...
    ]
    """
    handle, tempname = tempfile.mkstemp()
    os.close(handle)
    with ZipFile(tempname, mode='w', compression=ZIP_DEFLATED) as zipfile:
        for file_ in files:
            zipfile.write(file_['path'], file_['name'])

    objname = uuid.uuid4().hex + ".zip"

    s3 = boto3.resource('s3')
    s3.Bucket(bucket).upload_file(tempname, objname, ExtraArgs={'ContentType':'application/zip'})

    os.remove(tempname)

    url = "https://%s.s3.amazonaws.com/%s" % (bucket, objname)
    return url

