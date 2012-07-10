cmake_minimum_required ( VERSION 2.8 )

project ( eqbeats )

add_executable ( eqbeats.fcgi
    account/account.cpp account/list.cpp account/user.cpp account/session.cpp
    account/pages/account.cpp account/pages/login.cpp account/pages/registration.cpp account/pages/user.cpp account/pages/users.cpp account/pages/password.cpp
    core/db.cpp core/path.cpp
    log/log.cpp
    misc/hash.cpp misc/mail.cpp misc/timer.cpp
    pages/pages.cpp
    render/document.cpp render/fcgiio.cpp render/file.cpp render/http.cpp
    text/date.cpp text/email.cpp text/format.cpp text/irc.cpp text/number.cpp text/string.cpp
    track/audio.cpp track/art.cpp track/extended.cpp track/list.cpp track/track.cpp
    track/pages/oembed.cpp track/pages/track.cpp track/pages/tracks.cpp
    main.cpp
)

include_directories ( /usr/include/ImageMagick . )

target_link_libraries ( eqbeats.fcgi
    cgicc fcgi fcgi++ pq pcrecpp tag Magick++ MagickCore nettle ctemplate
)

add_custom_command (
    TARGET eqbeats.fcgi
    POST_BUILD
    COMMAND "$ENV{EQBEATS_DIR}/launch.sh" restart
)

add_definitions ( -Wall )

add_subdirectory( tools )
