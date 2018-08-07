file(GLOB HEADERS "include/creativecoin/private_message/*.hpp")

add_library( creativecoin_private_message
             private_message_plugin.cpp
           )

target_link_libraries( creativecoin_private_message creativecoin_chain creativecoin_protocol creativecoin_app )
target_include_directories( creativecoin_private_message
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   creativecoin_private_message

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
