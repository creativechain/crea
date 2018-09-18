file(GLOB HEADERS "include/crea/plugins/auth_util/*.hpp")

add_library( crea_auth_util
             ${HEADERS}
             auth_util_plugin.cpp
             auth_util_api.cpp
           )

target_link_libraries( crea_auth_util crea_app crea_chain crea_protocol fc )
target_include_directories( crea_auth_util
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
