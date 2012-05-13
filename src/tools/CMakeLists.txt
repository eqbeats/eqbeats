add_executable ( mknews mknews.cpp ../db.cpp ../number.cpp )
target_link_libraries ( mknews pq )

add_executable ( mkcontest mkcontest.cpp ../db.cpp ../number.cpp )
target_link_libraries ( mkcontest pq )

add_executable ( mkthumbs mkthumbs.cpp ../number.cpp ../art.cpp ../path.cpp )
target_link_libraries ( mkthumbs Magick++ MagickCore )

add_executable ( stats stats.cpp ../db.cpp )
target_link_libraries ( stats pq )

add_executable ( dumptracks dumptracks.cpp ../db.cpp )
target_link_libraries ( dumptracks pq )

add_executable ( updatetags updatetags.cpp ../db.cpp )
target_link_libraries ( updatetags pq tag )

add_executable ( fqueue fqueue.cpp ../db.cpp ../number.cpp ../path.cpp )
target_link_libraries ( fqueue pq )
