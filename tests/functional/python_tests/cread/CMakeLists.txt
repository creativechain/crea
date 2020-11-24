SET ( cread_tests_args --run-cread ${cread_path})

IF ( BUILD_CREA_TESTNET )
    ADD_PYTHON_FUNCTIONAL_TEST( cread_options_tests ${cread_tests_args} )
    ADD_PYTHON_FUNCTIONAL_TEST( cread_hybrid_operations_test initminer ${initminer_key} ${cread_tests_args} )
ELSE ( BUILD_CREA_TESTNET )
    MESSAGE ( STATUS "cread functional tests skipped: BUILD_CREA_TESTNET is required" )
ENDIF ( BUILD_CREA_TESTNET )
