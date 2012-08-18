cmake_minimum_required ( VERSION 2.8 )

project ( eqbeats )

add_executable ( eqbeats.fcgi
    main.cpp path.cpp timer.cpp number.cpp db.cpp track.cpp media.cpp user.cpp account.cpp session.cpp art.cpp mail.cpp contest.cpp log.cpp follower.cpp repl.cpp event.cpp playlist.cpp md5.cpp
    render/render.cpp render/oembed.cpp  render/json.cpp render/http.cpp render/feed.cpp render/download.cpp render/news.cpp
    render/html/page.cpp render/html/escape.cpp render/html/static.cpp render/html/track.cpp render/html/contest.cpp render/html/user.cpp render/html/home.cpp render/html/feed.cpp render/html/tracklist.cpp render/html/forms.cpp render/html/player.cpp render/html/event.cpp render/html/playlist.cpp render/html/comment.cpp
    actions/registration.cpp actions/login.cpp actions/account.cpp actions/favorite.cpp actions/upload.cpp actions/track.cpp actions/art.cpp actions/contest.cpp actions/password.cpp actions/playlist.cpp actions/youtube.cpp actions/license.cpp actions/comment.cpp
)

include_directories ( /usr/include/ImageMagick )

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
