file(GLOB HEADERS "include/creativecoin/blockchain_statistics/*.hpp")

add_library( creativecoin_blockchain_statistics
             blockchain_statistics_plugin.cpp
             blockchain_statistics_api.cpp
           )

target_link_libraries( creativecoin_blockchain_statistics creativecoin_chain creativecoin_protocol creativecoin_app )
target_include_directories( creativecoin_blockchain_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   creativecoin_blockchain_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)