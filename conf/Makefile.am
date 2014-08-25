pkgdata_DATA = nginx.conf
CLEANFILES = nginx.conf
EXTRA_DIST = nginx.conf.in

nginx.conf: nginx.conf.in Makefile
	sed -e 's,[@]eqbeats_dir[@],$(eqbeats_dir),g' \
	    -e 's,[@]pkgdatadir[@],$(pkgdatadir),g' \
	    < $< > $@
