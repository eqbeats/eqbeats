pkglibexec_SCRIPTS = autofeature feature transcode.sh ytmgr.py
pkglibexec_PROGRAMS = dumptracks fqueue mkthumbs udpstat updatetags

AM_CPPFLAGS = -I$(top_srcdir)/src

dumptracks_SOURCES = dumptracks.cpp
dumptracks_LDADD = -lpq $(HIREDIS_LIBS) ../core/libcore.a ../text/libtext.a

fqueue_SOURCES = fqueue.cpp
fqueue_LDADD = -lpq $(HIREDIS_LIBS) ../core/libcore.a ../text/libtext.a

mkthumbs_SOURCES = mkthumbs.cpp
mkthumbs_CPPFLAGS = $(AM_CPPFLAGS) $(MAGICK_CFLAGS)
mkthumbs_LDADD = $(MAGICK_LIBS) ../track/libtrack.a ../text/libtext.a ../core/libcore.a

udpstat_SOURCES = udpstat.cpp
udpstat_LDADD = $(NETTLE_LIBS) ../misc/libmisc.a ../text/libtext.a

updatetags_SOURCES = updatetags.cpp
updatetags_LDADD = -lpq $(HIREDIS_LIBS) $(TAGLIB_LIBS) ../core/libcore.a ../text/libtext.a
