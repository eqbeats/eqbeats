pkgdata_DATA = nginx.conf
CLEANFILES = nginx.conf
EXTRA_DIST = nginx.conf.in

nginx.conf: nginx.conf.in Makefile
	sed -e 's,[@]pkglibexecdir[@],$(pkglibexecdir),g' \
	    -e 's,[@]pkgdatadir[@],$(pkgdatadir),g' \
	    < $< > $@
