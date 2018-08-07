file(GLOB HEADERS "include/creativecoin/plugins/auth_util/*.hpp")

add_library( creativecoin_auth_util
             ${HEADERS}
             auth_util_plugin.cpp
             auth_util_api.cpp
           )

target_link_libraries( creativecoin_auth_util creativecoin_app creativecoin_chain creativecoin_protocol fc )
target_include_directories( creativecoin_auth_util
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
