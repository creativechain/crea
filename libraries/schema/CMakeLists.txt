file(GLOB HEADERS "include/crea/schema/*.hpp")
add_library( crea_schema schema.cpp ${HEADERS} )
target_link_libraries( crea_schema fc )
target_include_directories( crea_schema PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   crea_schema

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
install( FILES ${HEADERS} DESTINATION "include/crea/schema" )
