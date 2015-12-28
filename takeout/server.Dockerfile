FROM python:onbuild
MAINTAINER codl (Corentin Delcourt) <codl@codl.fr>

ENV EQBEATS_REDIS=redis \
    EQBEATS_POSTGRES="host=db dbname=eqbeats user=postgres" \
    EQBEATS_DIR=/var/lib/eqbeats

ENV TAKEOUT_DEBUG y

CMD python serve.py

EXPOSE 8051
