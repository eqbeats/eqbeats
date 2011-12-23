cmake_minimum_required(VERSION 2.8)

project(eqbeats)

set ( COMMON FCgiIO.cpp )
set ( HTML
    html/redirect.cpp html/page.cpp html/escape.cpp
    html/track.cpp html/user.cpp html/home.cpp
    actions/registration.cpp actions/login.cpp actions/account.cpp
    actions/upload.cpp actions/track.cpp
)

#add_executable ( upload.fcgi upload.cpp ${COMMON} )
add_executable ( eqbeats.fcgi
    main.cpp routing.cpp timer.cpp utils.cpp
    track.cpp ffmpeg.cpp user.cpp account.cpp session.cpp
    ${HTML} ${COMMON}
)

set ( LIBS cgicc fcgi fcgi++ pq)
#target_link_libraries ( upload.fcgi ${LIBS} )
target_link_libraries ( eqbeats.fcgi ${LIBS} )

ADD_CUSTOM_COMMAND (
    TARGET eqbeats.fcgi
    POST_BUILD
    COMMAND /srv/eqbeats/launch.sh restart
)

add_definitions ( -Wall )
