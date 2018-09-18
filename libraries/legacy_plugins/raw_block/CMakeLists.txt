file(GLOB HEADERS "include/crea/plugins/raw_block/*.hpp")

add_library( crea_raw_block
             ${HEADERS}
             raw_block_plugin.cpp
             raw_block_api.cpp
           )

target_link_libraries( crea_raw_block crea_app crea_chain crea_protocol fc )
target_include_directories( crea_raw_block
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
