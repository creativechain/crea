file(GLOB HEADERS "include/crea/blockchain_statistics/*.hpp")

add_library( crea_blockchain_statistics
             blockchain_statistics_plugin.cpp
             blockchain_statistics_api.cpp
           )

target_link_libraries( crea_blockchain_statistics crea_chain crea_protocol crea_app )
target_include_directories( crea_blockchain_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   crea_blockchain_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)