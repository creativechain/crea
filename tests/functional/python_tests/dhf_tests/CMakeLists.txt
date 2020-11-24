SET ( dhf_tests_args initminer initminer ${initminer_key} --run-cread ${cread_path} --working-dir ${CMAKE_BINARY_DIR}/tests/crea-node-data)

IF ( BUILD_CREA_TESTNET )
    ADD_PYTHON_FUNCTIONAL_TEST( beem_dhf_tests ${dhf_tests_args} --no-erase-proposal )
    ADD_PYTHON_FUNCTIONAL_TEST( list_proposals_tests ${dhf_tests_args} )
ELSE ( BUILD_CREA_TESTNET )
    MESSAGE ( STATUS "DHF functional tests skipped: BUILD_CREA_TESTNET is required" )
ENDIF ( BUILD_CREA_TESTNET )