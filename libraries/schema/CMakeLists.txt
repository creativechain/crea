file(GLOB HEADERS "include/creativecoin/schema/*.hpp")
add_library( creativecoin_schema schema.cpp ${HEADERS} )
target_link_libraries( creativecoin_schema fc )
target_include_directories( creativecoin_schema PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" )

install( TARGETS
   creativecoin_schema

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
install( FILES ${HEADERS} DESTINATION "include/creativecoin/schema" )
