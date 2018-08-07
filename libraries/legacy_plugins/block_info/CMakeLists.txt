file(GLOB HEADERS "include/creativecoin/plugins/block_info/*.hpp")

add_library( creativecoin_block_info
             ${HEADERS}
             block_info_plugin.cpp
             block_info_api.cpp
           )

target_link_libraries( creativecoin_block_info creativecoin_app creativecoin_chain creativecoin_protocol fc )
target_include_directories( creativecoin_block_info
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )
