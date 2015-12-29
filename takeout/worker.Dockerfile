FROM python:onbuild
MAINTAINER codl (Corentin Delcourt) <codl@codl.fr>

ENV EQBEATS_REDIS=redis \
    EQBEATS_POSTGRES="host=db dbname=eqbeats user=postgres" \
    EQBEATS_DIR=/var/lib/eqbeats \
    EQBEATS_URL="http://web:8642"

CMD ["rqworker", "-u", "redis://redis"]
