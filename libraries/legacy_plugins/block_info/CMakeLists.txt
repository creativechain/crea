file(GLOB HEADERS "include/crea/plugins/block_info/*.hpp")

add_library( crea_block_info
             ${HEADERS}
             block_info_plugin.cpp
             block_info_api.cpp
           )

target_link_libraries( crea_block_info crea_app crea_chain crea_protocol fc )
target_include_directories( crea_block_info
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
