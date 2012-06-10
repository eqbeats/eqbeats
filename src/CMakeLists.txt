cmake_minimum_required ( VERSION 2.8 )

project ( eqbeats )

add_executable ( eqbeats.fcgi
    account/account.cpp account/user.cpp
    core/db.cpp core/path.cpp
    media/art.cpp media/audio.cpp media/file.cpp
    misc/hash.cpp misc/timer.cpp
    render/fcgiio.cpp render/http.cpp
    session/session.cpp session/login.cpp
    text/date.cpp text/email.cpp text/format.cpp text/irc.cpp text/number.cpp text/string.cpp
    track/extended.cpp track/list.cpp track/track.cpp
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
