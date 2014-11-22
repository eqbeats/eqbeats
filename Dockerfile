FROM phusion/baseimage
MAINTAINER Corentin Delcourt <codl@codl.fr>

VOLUME ["/var/lib/eqbeats"]

CMD ["/sbin/my_init"]

RUN rm -rf /etc/service/sshd /etc/my_init.d/00_regen_ssh_host_keys.sh

RUN add-apt-repository ppa:mc3man/trusty-media

RUN apt-get update && apt-get upgrade -yq && \
    apt-get install -yq spawn-fcgi libctemplate-dev nettle-dev libpq-dev libcgicc5-dev libfcgi-dev postgresql-client autoconf g++ libtool libmagickwand-dev libtag1-dev ffmpeg libhiredis-dev build-essential

RUN rm -rf /var/lib/apt/lists/*

RUN echo "host=db dbname=eqbeats user=postgres" > /etc/container_environment/EQBEATS_POSTGRES
RUN echo "redis" > /etc/container_environment/EQBEATS_REDIS
RUN echo "/var/lib/eqbeats" > /etc/container_environment/EQBEATS_DIR

EXPOSE 8100

ADD Makefile.am autogen.sh configure.ac /eqbeats/
ADD conf /eqbeats/conf
ADD include /eqbeats/include
ADD sql /eqbeats/sql
ADD static /eqbeats/static
ADD templates /eqbeats/templates

ADD lib /eqbeats/lib
ADD src /eqbeats/src

RUN cd /eqbeats &&\
    ./autogen.sh &&\
    CTEMPLATE_CFLAGS=" " CTEMPLATE_LIBS="-lctemplate"\
        ./configure --prefix=/usr &&\
    make -j install

ADD conf/docker/eqbeats.runit /etc/service/eqbeats/run
ADD conf/docker/syslog-ng.docker.conf /etc/syslog-ng/conf.d/00-docker.conf
