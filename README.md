# EqBeats
## Quick and ugly startup guide

### Dependencies

* A webserver that supports FastCGI
* PostgreSQL with libpq
* libfcgi (or just fcgi in some package managers)
* GNU cgicc
* libpcre++ (sometimes included with pcre)
* TagLib
* ImageMagick
* Nettle
* ctemplate
* FFmpeg
* Optional: A Haskell compiler (like GHC) for hitsd
* Optional: Python 3.x and psycopg2 for YouTube support

### Preliminary setup

#### Environment

Set the `EQBEATS_DIR` environment variable to your eqbeats directory and the `EQBEATS_YT_API_KEY` to your YouTube API key. You can acquire one [here](https://code.google.com/apis/youtube/dashboard/gwt/index.html).

You can set the `EQBEATS_HTTPS` variable to anything if you wish to always use https.

You also need to make sure that you are in the `http` group, as well as the user running the webserver.

There should be a `tracks` directory and an `art` directory with two subdirectories, `art/medium` and `art/thumb`, in the directory where EqBeats is installed. The user you plan to run as should have write permission to these.

#### Webserver

Point your FastCGI-enabled webserver to the Unix socket `$EQBEATS_DIR/eqbeats.sock`.

You'll also need to alias `/static/` to `$EQBEATS_DIR/static/`,
and proxy `/download/art/` and `/download/tracks/` to `$EQBEATS_DIR/art/` and `$EQBEATS_DIR/tracks/`, respectively.

Here is a sample config for nginx:

    server {
        listen       [::]:80;
        server_name localhost;
        client_max_body_size 100m;

        error_page 502 /static/502.html;

        location / {
            include fastcgi.conf;
            fastcgi_pass unix:/home/you/eqbeats/eqbeats.sock;
        }
        location /static {
            root /home/you/eqbeats/;
        }
        location /downloads/art {
            internal;
            alias /home/you/eqbeats/art/;
        }
        location /downloads/tracks {
            internal;
            alias /home/you/eqbeats/tracks/;
        }
        location /robots.txt { alias /home/you/eqbeats/static/robots.txt; }
    }


#### Database

By default, EqBeats will use the default PostgreSQL DB for your user, so make sure you have one and that the pgcrypto extension is enabled on it.

    su postgres
    createuser -d YOURNAME
    echo "CREATE EXTENSION pgcrypto;" | psql -d YOURNAME

Then, import the database schema:

    psql -d YOURNAME < sql/db.sql

### Compiling and starting

    cmake src
    make
    ./launch.sh start

