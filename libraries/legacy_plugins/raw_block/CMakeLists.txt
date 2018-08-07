file(GLOB HEADERS "include/creativecoin/plugins/raw_block/*.hpp")

add_library( creativecoin_raw_block
             ${HEADERS}
             raw_block_plugin.cpp
             raw_block_api.cpp
           )

target_link_libraries( creativecoin_raw_block creativecoin_app creativecoin_chain creativecoin_protocol fc )
target_include_directories( creativecoin_raw_block
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
