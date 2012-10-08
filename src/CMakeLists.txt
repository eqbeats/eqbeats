cmake_minimum_required ( VERSION 2.8 )

project ( eqbeats )

find_path (fcgi_INCLUDE fcgio.h /usr/include)
find_library(FGCI_LIB fcgi fcgi)

if (NOT fcgi_INCLUDE)
  message(FATAL_ERROR "fcgio.h not found in include path, please install the fcgi development package")
endif (NOT fcgi_INCLUDE)

find_path (cgicc_INCLUDE cgicc/CgiInput.h /usr/include)
if (NOT cgicc_INCLUDE)
  message(FATAL_ERROR "cgicc/CgiInput.h not found in include path, please install cgicc development package")
endif (NOT cgicc_INCLUDE)

find_path(magick_INCLUDE Magick++.h /usr/include /usr/include/ImageMagick)
if (NOT magick_INCLUDE)
  message(FATAL_ERROR "Magick++.h not found in include path, please install ImageMagick-c++ development package")
endif (NOT magick_INCLUDE)
include_directories ( /usr/include/ImageMagick )

find_path(postgres_INCLUDE libpq-fe.h /usr/include)
if (NOT postgres_INCLUDE)
  message(FATAL_ERROR "libpq-fe.h not found in include path, please install Postgresql development package")
endif (NOT postgres_INCLUDE)
find_path(pcre_INCLUDE pcrecpp.h /usr/include)
if (NOT pcre_INCLUDE)
  message(FATAL_ERROR "pcrecpp.h not found in include path, please install pcre development package")
endif (NOT pcre_INCLUDE)
find_path(nettle_INCLUDE nettle/md5.h /usr/include)
if (NOT nettle_INCLUDE)
  message(FATAL_ERROR "nettle/md5.h not found in include path, please install nettle development package")
endif (NOT nettle_INCLUDE)




add_executable ( eqbeats.fcgi
    main.cpp path.cpp timer.cpp number.cpp db.cpp track.cpp media.cpp user.cpp account.cpp session.cpp art.cpp mail.cpp contest.cpp log.cpp follower.cpp repl.cpp event.cpp playlist.cpp md5.cpp
    render/render.cpp render/oembed.cpp  render/json.cpp render/http.cpp render/feed.cpp render/download.cpp render/news.cpp
    render/html/page.cpp render/html/escape.cpp render/html/static.cpp render/html/track.cpp render/html/contest.cpp render/html/user.cpp render/html/home.cpp render/html/feed.cpp render/html/tracklist.cpp render/html/forms.cpp render/html/player.cpp render/html/event.cpp render/html/playlist.cpp render/html/comment.cpp
    actions/registration.cpp actions/login.cpp actions/account.cpp actions/favorite.cpp actions/upload.cpp actions/track.cpp actions/art.cpp actions/contest.cpp actions/password.cpp actions/playlist.cpp actions/youtube.cpp actions/license.cpp actions/comment.cpp
)


target_link_libraries ( eqbeats.fcgi
    cgicc fcgi fcgi++ pq pcrecpp tag Magick++ MagickCore nettle
)

add_custom_command (
    TARGET eqbeats.fcgi
    POST_BUILD
    COMMAND "$ENV{EQBEATS_DIR}/launch.sh" restart
)

add_definitions ( -Wall )

add_subdirectory( tools )
