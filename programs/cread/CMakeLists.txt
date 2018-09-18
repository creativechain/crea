add_executable( creativecoind main.cpp )
if( UNIX AND NOT APPLE )
  set(rt_library rt )
endif()

find_package( Gperftools QUIET )
if( GPERFTOOLS_FOUND )
    message( STATUS "Found gperftools; compiling creativecoind with TCMalloc")
    list( APPEND PLATFORM_SPECIFIC_LIBS tcmalloc )
endif()

if( CREA_STATIC_BUILD )
   target_link_libraries( creativecoind PRIVATE
      "-static-libstdc++ -static-libgcc"
      appbase
      creativecoin_utilities
      creativecoin_plugins
      ${CMAKE_DL_LIBS}
      ${PLATFORM_SPECIFIC_LIBS}
   )
else( CREA_STATIC_BUILD )
   target_link_libraries( creativecoind PRIVATE
      appbase
      creativecoin_utilities
      creativecoin_plugins
      ${CMAKE_DL_LIBS}
      ${PLATFORM_SPECIFIC_LIBS}
   )
endif( CREA_STATIC_BUILD )

if( CLANG_TIDY_EXE )
   set_target_properties(
      creativecoind PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

install( TARGETS
   creativecoind

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
