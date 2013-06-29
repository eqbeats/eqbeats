pkglibexec_SCRIPTS = autofeature feature transcode.sh ytmgr.py
pkglibexec_PROGRAMS = clint dumptracks fqueue mkthumbs multiplex udpstat updatetags

AM_CPPFLAGS = -I$(top_srcdir)/src

clint_SOURCES = clint.cpp

dumptracks_SOURCES = dumptracks.cpp
dumptracks_LDADD = -lpq ../core/libcore.a

fqueue_SOURCES = fqueue.cpp
fqueue_LDADD = -lpq ../core/libcore.a ../text/libtext.a

mkthumbs_SOURCES = mkthumbs.cpp
mkthumbs_CPPFLAGS = $(AM_CPPFLAGS) $(MAGICK_CFLAGS)
mkthumbs_LDADD = $(MAGICK_LIBS) ../track/libtrack.a ../text/libtext.a ../core/libcore.a

multiplex_SOURCES = multiplex.c

udpstat_SOURCES = udpstat.cpp
udpstat_LDADD = $(NETTLE_LIBS) ../misc/libmisc.a ../text/libtext.a

updatetags_SOURCES = updatetags.cpp
updatetags_LDADD = -lpq $(TAGLIB_LIBS) ../core/libcore.a ../text/libtext.a
