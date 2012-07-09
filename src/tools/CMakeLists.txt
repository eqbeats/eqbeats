add_executable ( mkthumbs mkthumbs.cpp ../text/number.cpp ../media/art.cpp ../core/path.cpp )
target_link_libraries ( mkthumbs Magick++ MagickCore )

add_executable ( stats stats.cpp ../core/db.cpp )
target_link_libraries ( stats pq )

add_executable ( dumptracks dumptracks.cpp ../core/db.cpp )
target_link_libraries ( dumptracks pq )

add_executable ( updatetags updatetags.cpp ../core/db.cpp ../text/number.cpp )
target_link_libraries ( updatetags pq tag )

add_executable ( fqueue fqueue.cpp ../core/db.cpp ../text/number.cpp ../core/path.cpp )
target_link_libraries ( fqueue pq )
