add_executable ( mknews mknews.cpp ../db.cpp ../utils.cpp )
target_link_libraries ( mknews pq )

add_executable ( mkcontest mkcontest.cpp ../db.cpp ../utils.cpp )
target_link_libraries ( mkcontest pq )

add_executable ( mkthumbs mkthumbs.cpp ../utils.cpp ../art.cpp )
target_link_libraries ( mkthumbs Magick++ MagickCore )

add_executable ( stats stats.cpp ../db.cpp )
target_link_libraries ( stats pq )

add_executable ( dumptracks dumptracks.cpp ../db.cpp )
target_link_libraries ( dumptracks pq )
