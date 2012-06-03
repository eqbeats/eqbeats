cmake_minimum_required ( VERSION 2.8 )

project ( eqbeats )

add_executable ( eqbeats.fcgi
    account/account.cpp account/user.cpp
    core/db.cpp core/path.cpp
    ipc/repl.cpp
    misc/number.cpp
    playlist/playlist.cpp
    render/http.cpp render/render.cpp
    session/session.cpp
    track/track.cpp
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
