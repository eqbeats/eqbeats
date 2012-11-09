# EqBeats
## Quick and ugly startup guide

### Dependencies

* A webserver that supports fastCGI
* Postgresql
* libfcgi (or just fcgi in some package managers)
* Cgicc
* FFmpeg
* Optional: A haskell compiler and some other haskell stuff for hitsd
* Optional: Python 3.x and psycopg2 for YouTube support

### Preliminary setup

#### Environment

Set the `EQBEATS_DIR` environment variable to your eqbeats directory.
You also need to make sure that you are in the `http` group, as well as the user running the webserver.

You can set the `EQBEATS_HTTPS` variable to anything if you wish to always use https.

#### Webserver

Point your FCGI enabled webserver to the unix socket at `$EQBEATS_DIR/eqbeats.sock`.

You'll also need to alias `/static/` to `$EQBEATS_DIR/static/`,
and proxy `/download/art/` and `/download/tracks/` to `$EQBEATS_DIR/art/` and `$EQBEATS_DIR/tracks/`

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

By default EqBeats will use the default Postgresql DB for your user, so make sure you have one and that pgcrypto extensions are enabled on it.

    su postgres
    createuser -d YOURNAME
    echo "CREATE EXTENSION pgcrypto;" | psql -d YOURNAME

Then, import the database schema.

    psql -d YOURNAME < sql/db.sql

### Compiling and starting

    cmake src
    make
    ./launch.sh start

