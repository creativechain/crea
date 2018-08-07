file(GLOB HEADERS "include/creativecoin/account_statistics/*.hpp")

add_library( creativecoin_account_statistics
             account_statistics_plugin.cpp
             account_statistics_api.cpp
           )

target_link_libraries( creativecoin_account_statistics creativecoin_chain creativecoin_protocol creativecoin_app )
target_include_directories( creativecoin_account_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   creativecoin_account_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)