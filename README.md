# EqBeats

## Running a dev environment in docker

A very easy way to get a dev environment set up is by using [Docker][]
and [fig][]. Once Docker and fig are installed on your system, run

    fig up

Once the image is built and the services are up, a brand new instance
of EqBeats will be listening on [localhost:8642][]. To rebuild, run

    fig build

[Docker]: http://www.docker.com/
[fig]: http://www.fig.sh/
[localhost:8642]: http://localhost:8642/

## Setting up a dev environment by hand

### Dependencies

* A webserver that supports FastCGI
* PostgreSQL with libpq
* libfcgi (or just fcgi in some package managers)
* GNU cgicc
* spawn-fcgi
* libpcre++ (sometimes included with pcre)
* TagLib >= 1.9
* ImageMagick
* Nettle
* ctemplate
* FFmpeg compiled with libfdk\_aac and libopus support
* Redis and hiredis
* Optional: OpenBSD netcat for hitsd
* Optional: OpenBSD netcat, Python 3.x and psycopg2 for YouTube support

### Preliminary setup

#### Environment

If you want YouTube support, set the `EQBEATS_YT_API_KEY` to your YouTube API
key. You can acquire one [here](https://code.google.com/apis/youtube/dashboard/gwt/index.html).

You can set the `EQBEATS_HTTPS` variable to anything if you wish to always use
HTTPS.

You also need to make sure that you are in the `http` group, as well as the user
running the webserver.

You need the following file structure, writable by the user that will run
EqBeats:
* `EQBEATS_DIR` (by default: `/var/lib/eqbeats/`)
* `EQBEATS_DIR/tracks`
* `EQBEATS_DIR/art`
* `EQBEATS_DIR/art/medium`
* `EQBEATS_DIR/art/thumb`

#### Web server

We’ll assume you put the FastCGI socket in `/run/eqbeats/eqbeats.sock`, and that
EqBeats’s data files are installed in `/usr/share/eqbeats`.

Here is a sample config for nginx:

    server {
        listen       [::]:80;
        server_name localhost;
        client_max_body_size 100m;

        error_page 502 /static/502.html;

        location / {
            include fastcgi.conf;
            fastcgi_pass unix:/run/eqbeats/eqbeats.sock;
        }
        location /static {
            alias /usr/share/eqbeats/static/;
        }
        location /downloads/art {
            internal;
            alias /var/lib/eqbeats/art/;
        }
        location /downloads/tracks {
            internal;
            alias /var/lib/eqbeats/tracks/;
        }
        location /robots.txt {
            alias /usr/share/eqbeats/static/robots.txt;
        }
    }

#### PostgreSQL database

By default, EqBeats will use the default PostgreSQL DB for your user, so make
sure you have one and that the pgcrypto extension is enabled on it.

    su postgres -c 'createuser -d YOURNAME'
    su YOU -c 'createdb'
    su postgres
    echo "CREATE EXTENSION pgcrypto;" | psql -d YOURNAME

Then, import the database schema:

    psql -d YOURNAME < sql/db.sql

#### Redis database

If you want statistics support, you need to set up a Redis server to listen on a
Unix socket (like `/run/eqbeats/redis.sock`). It is recommended that only the
user that runs EqBeats is allowed to access it.

A sample Redis configuration is available in `conf/redis-eqbeats.conf`. A
systemd unit is also provided in `conf/redis-eqbeats.service`.

### Installing from source

If you want to generate the configure script, run:

    ./autogen.sh

You can then see the available options with:

    ./configure --help

After that, the usual procedure will do:

    ./configure
    make
    make install

### Running

If you’re running systemd, you can use the service unit at
`conf/eqbeats.service`. You’ll need to have spawn-fcgi and multiwatch installed
in order to use it. You might as well want `conf/tmpfiles-eqbeats.conf` (see
tmpfiles.d(5) for more information).

Since EqBeats is a regular FastCGI application, you can spawn it the way you
want though.
