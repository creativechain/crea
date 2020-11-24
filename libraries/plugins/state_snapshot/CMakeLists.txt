file(GLOB HEADERS "include/crea/plugins/state_snapshot/*.hpp")

add_library( state_snapshot_plugin
             state_snapshot_plugin.cpp
             ${HEADERS}
           )

target_link_libraries( state_snapshot_plugin
   rocksdb chain_plugin crea_chain crea_protocol json_rpc_plugin
   )

target_include_directories( state_snapshot_plugin
   PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
          "${CMAKE_CURRENT_SOURCE_DIR}/../../vendor/rocksdb/include"
   )

install( TARGETS
   state_snapshot_plugin

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
