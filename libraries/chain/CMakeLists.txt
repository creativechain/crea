file(GLOB HEADERS "include/creativecoin/chain/*.hpp" "include/creativecoin/chain/util/*.hpp")

## SORT .cpp by most likely to change / break compile
add_library( creativecoin_chain

             # As database takes the longest to compile, start it first
             database.cpp

             smt_evaluator.cpp
             smt_objects/smt_market_maker.cpp
             creativecoin_evaluator.cpp
             creativecoin_objects.cpp

             witness_schedule.cpp
             fork_database.cpp

             shared_authority.cpp
             block_log.cpp

             generic_custom_operation_interpreter.cpp

             util/reward.cpp
             util/impacted.cpp
             util/advanced_benchmark_dumper.cpp

             ${HEADERS}
           )

target_link_libraries( creativecoin_chain creativecoin_jsonball creativecoin_protocol fc chainbase creativecoin_schema appbase
                       ${PATCH_MERGE_LIB} )
target_include_directories( creativecoin_chain
                            PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}"
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${CMAKE_CURRENT_BINARY_DIR}/include" )

if( CLANG_TIDY_EXE )
   set_target_properties(
      creativecoin_chain PROPERTIES
      CXX_CLANG_TIDY "${DO_CLANG_TIDY}"
   )
endif( CLANG_TIDY_EXE )

if(MSVC)
  set_source_files_properties( database.cpp PROPERTIES COMPILE_FLAGS "/bigobj" )
endif(MSVC)

INSTALL( TARGETS
   creativecoin_chain

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
INSTALL( FILES ${HEADERS} DESTINATION "include/creativecoin/chain" )
