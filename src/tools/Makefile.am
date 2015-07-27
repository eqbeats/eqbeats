pkglibexec_SCRIPTS = autofeature feature transcode.sh
pkglibexec_PROGRAMS = dumptracks fqueue mkthumbs updatetags

AM_CPPFLAGS = -I$(top_srcdir)/src

dumptracks_SOURCES = dumptracks.cpp
dumptracks_LDADD = ../core/libcore.a $(PQ_LIBS) $(HIREDIS_LIBS) ../text/libtext.a

fqueue_SOURCES = fqueue.cpp
fqueue_LDADD = ../core/libcore.a $(PQ_LIBS) $(HIREDIS_LIBS) ../text/libtext.a

mkthumbs_SOURCES = mkthumbs.cpp
mkthumbs_CPPFLAGS = $(AM_CPPFLAGS) $(MAGICK_CFLAGS)
mkthumbs_LDADD = $(MAGICK_LIBS) ../track/libtrack.a ../core/libcore.a $(HIREDIS_LIBS) ../text/libtext.a

updatetags_SOURCES = updatetags.cpp
updatetags_LDADD = ../../lib/libeqbeats.la ../track/libtrack.a ../account/libaccount.a ../playlist/libplaylist.a ../log/liblog.a ../misc/libmisc.a ../text/libtext.a ../core/libcore.a $(CTEMPLATE_LIBS) $(CGICC_LIBS) $(MAGICK_LIBS) $(TAGLIB_LIBS) $(PQ_LIBS) $(HIREDIS_LIBS) $(NETTLE_LIBS)
updatetags_LDFLAGS = -Wl,--start-group
