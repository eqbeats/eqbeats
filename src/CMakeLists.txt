cmake_minimum_required ( VERSION 2.8 )

project ( eqbeats )

set ( EQBEATS_DIR /srv/eqbeats )

add_executable ( eqbeats.fcgi
    main.cpp routing.cpp timer.cpp utils.cpp db.cpp news.cpp comment.cpp
    track.cpp ffmpeg.cpp flac.cpp user.cpp account.cpp session.cpp
    html/redirect.cpp html/page.cpp html/escape.cpp
    html/track.cpp html/user.cpp html/news.cpp
    actions/registration.cpp actions/login.cpp actions/account.cpp
    actions/upload.cpp actions/track.cpp
    FCgiIO.cpp
)

target_link_libraries ( eqbeats.fcgi
    cgicc fcgi fcgi++ pq FLAC++ pcrecpp
)

ADD_CUSTOM_COMMAND (
    TARGET eqbeats.fcgi
    POST_BUILD
    COMMAND ${EQBEATS_DIR}/launch.sh restart
)

add_executable ( mknews mknews.cpp db.cpp utils.cpp )
target_link_libraries ( mknews pq )

add_definitions ( -Wall -D 'EQBEATS_DIR="${EQBEATS_DIR}"' )
