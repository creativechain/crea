file(GLOB HEADERS "include/creativecoin/delayed_node/*.hpp")

add_library( creativecoin_delayed_node
             delayed_node_plugin.cpp
           )

target_link_libraries( creativecoin_delayed_node creativecoin_chain creativecoin_protocol creativecoin_app )
target_include_directories( creativecoin_delayed_node
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if(MSVC)
  set_source_files_properties( delayed_node_plugin.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

install( TARGETS
   creativecoin_delayed_node

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
