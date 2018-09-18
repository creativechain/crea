file(GLOB HEADERS "include/crea/account_statistics/*.hpp")

add_library( crea_account_statistics
             account_statistics_plugin.cpp
             account_statistics_api.cpp
           )

target_link_libraries( crea_account_statistics crea_chain crea_protocol crea_app )
target_include_directories( crea_account_statistics
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   crea_account_statistics

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)