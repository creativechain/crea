file(GLOB HEADERS "include/crea/private_message/*.hpp")

add_library( crea_private_message
             private_message_plugin.cpp
           )

target_link_libraries( crea_private_message crea_chain crea_protocol crea_app )
target_include_directories( crea_private_message
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   crea_private_message

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
