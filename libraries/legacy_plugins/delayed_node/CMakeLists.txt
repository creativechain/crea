file(GLOB HEADERS "include/crea/delayed_node/*.hpp")

add_library( crea_delayed_node
             delayed_node_plugin.cpp
           )

target_link_libraries( crea_delayed_node crea_chain crea_protocol crea_app )
target_include_directories( crea_delayed_node
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if(MSVC)
  set_source_files_properties( delayed_node_plugin.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

install( TARGETS
   crea_delayed_node

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
