SUBDIRS = account core log misc pages playlist render social stat text track userfeature youtube tools

pkglibexec_PROGRAMS = eqbeats.fcgi
eqbeats_fcgi_LDADD = \
    -lpq -lfcgi -lfcgi++ $(MAGICK_LIBS) $(TAGLIB_LIBS) $(CTEMPLATE_LIBS) $(CGICC_LIBS) -lcgicc $(PCRECPP_LIBS) $(NETTLE_LIBS) \
    pages/libpages.a \
    playlist/libplaylist.a playlist/pages/libplaylistpages.a \
    social/libsocial.a social/pages/libsocialpages.a \
    stat/libstat.a \
    track/libtrack.a track/pages/libtrackpages.a \
    userfeature/libuserfeature.a userfeature/pages/libuserfeaturepages.a \
    youtube/libyoutube.a youtube/pages/libyoutubepages.a \
    account/libaccount.a account/pages/libaccountpages.a \
    misc/libmisc.a \
    render/librender.a \
    text/libtext.a \
    log/liblog.a \
    core/libcore.a
eqbeats_fcgi_SOURCES = main.cpp
AM_CPPFLAGS = -I$(top_srcdir)/src $(MAGICK_CFLAGS) $(TAGLIB_CFLAGS)
