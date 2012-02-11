cmake_minimum_required ( VERSION 2.8 )

project ( eqbeats )

add_executable ( eqbeats.fcgi
    main.cpp routing.cpp timer.cpp utils.cpp db.cpp news.cpp comment.cpp track.cpp ffmpeg.cpp user.cpp account.cpp session.cpp art.cpp mail.cpp contest.cpp
    render/oembed.cpp  render/json.cpp render/http.cpp render/feed.cpp
    render/html/page.cpp render/html/escape.cpp render/html/faq.cpp render/html/credits.cpp render/html/track.cpp render/html/contest.cpp render/html/user.cpp render/html/news.cpp render/html/comment.cpp render/html/home.cpp
    actions/registration.cpp actions/login.cpp actions/account.cpp actions/favorite.cpp actions/upload.cpp actions/track.cpp actions/comment.cpp actions/art.cpp actions/contest.cpp
    FCgiIO.cpp
)

include_directories ( /usr/include/ImageMagick )

target_link_libraries ( eqbeats.fcgi
    cgicc fcgi fcgi++ pq pcrecpp tag Magick++ MagickCore
)

add_custom_command (
    TARGET eqbeats.fcgi
    POST_BUILD
    COMMAND "$ENV{EQBEATS_DIR}/launch.sh" restart
)

add_definitions ( -Wall )

add_subdirectory( tools )
