file(GLOB HEADERS "include/crea/protocol/*.hpp")
file(GLOB HF_PARTS "hardfork.d/*.hpp")

if( MSVC )
   set( hardfork_hpp_file "${CMAKE_CURRENT_SOURCE_DIR}/include/crea/protocol/hardfork.hpp" )
   add_custom_target( build_hardfork_hpp
      COMMAND cat-parts "${CMAKE_CURRENT_SOURCE_DIR}/hardfork.d" ${hardfork_hpp_file} )
   add_dependencies( build_hardfork_hpp cat-parts )
else( MSVC )
   set( hardfork_hpp_file "${CMAKE_CURRENT_BINARY_DIR}/include/crea/protocol/hardfork.hpp" )
   add_custom_command(
      COMMAND ${CMAKE_COMMAND} -E env PYTHONPATH=${CMAKE_CURRENT_SOURCE_DIR}/../../programs/build_helpers
      python3 -m crea_build_helpers.cat_parts
      "${CMAKE_CURRENT_SOURCE_DIR}/hardfork.d"
      ${hardfork_hpp_file}
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/include/crea/protocol/hardfork.hpp"
      DEPENDS ${HF_PARTS}
   )
endif( MSVC )

## SORT .cpp by most likely to change / break compile
add_library( crea_protocol

             smt_operations.cpp
             crea_operations.cpp
             crea_required_actions.cpp
             crea_optional_actions.cpp
             types.cpp
             schema_types.cpp
             authority.cpp
             operations.cpp
             sign_state.cpp
             transaction.cpp
             block.cpp
             asset.cpp
             version.cpp
             get_config.cpp

             ${HEADERS}
             ${hardfork_hpp_file}
             "${CMAKE_CURRENT_BINARY_DIR}/include/crea/protocol/hardfork.hpp"
           )

target_link_libraries( crea_protocol crea_schema fc )
target_include_directories( crea_protocol
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include" )

if( CLANG_TIDY_EXE )
   set_target_properties(
      crea_protocol PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

INSTALL( TARGETS
   crea_protocol

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)

INSTALL( FILES ${HEADERS} DESTINATION "include/crea/protocol" )
