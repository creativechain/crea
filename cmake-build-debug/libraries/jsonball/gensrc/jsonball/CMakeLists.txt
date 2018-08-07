
add_library( creativecoin_jsonball
   mm_ticks.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/include/creativecoin/jsonball/jsonball.hpp
)

target_link_libraries( creativecoin_jsonball fc )

target_include_directories( creativecoin_jsonball PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

INSTALL( TARGETS
   creativecoin_jsonball

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)