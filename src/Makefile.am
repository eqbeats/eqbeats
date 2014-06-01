if HIREDIS
  MAYBE_STAT = stat
  MAYBE_LIBSTAT = stat/libstat.a
endif

SUBDIRS = account core log misc pages playlist render social $(MAYBE_STAT) text track userfeature youtube tools

pkglibexec_PROGRAMS = eqbeats.fcgi
eqbeats_fcgi_LDADD = $(top_srcdir)/lib/libeqbeats.la \
    $(PQ_LIBS) $(FCGI_LIBS) \
    $(MAGICK_LIBS) $(TAGLIB_LIBS) $(CTEMPLATE_LIBS) $(CGICC_LIBS) $(PCRECPP_LIBS) $(NETTLE_LIBS) $(HIREDIS_LIBS) \
    pages/libpages.a \
    playlist/libplaylist.a playlist/pages/libplaylistpages.a \
    social/libsocial.a social/pages/libsocialpages.a \
    track/libtrack.a track/pages/libtrackpages.a \
    userfeature/libuserfeature.a userfeature/pages/libuserfeaturepages.a \
    youtube/libyoutube.a youtube/pages/libyoutubepages.a \
    account/libaccount.a account/pages/libaccountpages.a \
    $(MAYBE_LIBSTAT) \
    misc/libmisc.a \
    render/librender.a \
    text/libtext.a \
    log/liblog.a \
    core/libcore.a
eqbeats_fcgi_SOURCES = main.cpp
AM_CPPFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/src $(MAGICK_CFLAGS) $(TAGLIB_CFLAGS) -DTEMPLATES_DIR=\"$(templates_dir)\"
