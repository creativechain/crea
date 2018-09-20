#!/bin/bash

function echo(){ builtin echo $(basename $0 .sh): "$@"; }
pushd () { command pushd "$@" > /dev/null; }
popd () { command popd "$@" > /dev/null; }

function print_help_and_quit {
   echo Usage: jobs test_cread_path ref_cread_path test_work_path ref_work_path block_limit [--dont-copy-config]
   echo Example: 16 ~/creativecoin/1/cread ~/creativecoin/2/cread ~/creativecoin/1/wdir ~/creativecoin/2/wdir 5000000
   exit -1
}

if [ $# -lt 6 -o $# -gt 7 ]
then
   print_help_and_quit
fi

COPY_CONFIG=$7

if [ "$COPY_CONFIG" != "--dont-copy-config" -a "$COPY_CONFIG" != "" ]
then
   echo "Unknown option: " $7
   print_help_and_quit
fi

SCRIPT_DIR=../scripts
PY_SCRIPT_DIR=../../api_tests
REPLAY_SCRIPT=run_replay.sh
NODE_SCRIPT=open_node.sh
NODE_ADDRESS=0.0.0.0
TEST_PORT=8090
REF_PORT=8091
TEST_NODE=$NODE_ADDRESS:$TEST_PORT
REF_NODE=$NODE_ADDRESS:$REF_PORT
TEST_NODE_OPT=--webserver-http-endpoint=$TEST_NODE
REF_NODE_OPT=--webserver-http-endpoint=$REF_NODE
EXIT_CODE=0
JOBS=$1
TEST_CREAD_PATH=$2
REF_CREAD_PATH=$3
TEST_WORK_PATH=$4
REF_WORK_PATH=$5
BLOCK_LIMIT=$6
WDIR=$PWD/logs
TEST_CREAD_PID=-1
REF_CREAD_PID=-1
export CREAD_NODE_PID=-1

function run_replay {
   echo Running $SCRIPT_DIR/$REPLAY_SCRIPT $TEST_CREAD_PATH $REF_CREAD_PATH $TEST_WORK_PATH $REF_WORK_PATH $BLOCK_LIMIT $COPY_CONFIG
   $SCRIPT_DIR/$REPLAY_SCRIPT $TEST_CREAD_PATH $REF_CREAD_PATH $TEST_WORK_PATH $REF_WORK_PATH $BLOCK_LIMIT $COPY_CONFIG
   [ $? -ne 0 ] && echo test group FAILED && exit -1
}

function open_node {
   echo Running $SCRIPT_DIR/$NODE_SCRIPT $1 $2 $3 $4 $5
   . $SCRIPT_DIR/$NODE_SCRIPT $1 $2 $3 $4 $5
}

function run_test {
   echo Running test $1
   WDIR=$PWD/$1/logs
   rm -rf $WDIR
   mkdir -p $1
   pushd $PY_SCRIPT_DIR
   
   echo Running python3 $1 $JOBS http://$TEST_NODE http://$REF_NODE $WDIR
   python3 $1 $JOBS http://$TEST_NODE http://$REF_NODE 50 $WDIR
   [ $? -ne 0 ] && echo test $1 FAILED && EXIT_CODE=-1

   popd
}

run_replay

open_node "tested" $TEST_CREAD_PATH $TEST_NODE_OPT $TEST_WORK_PATH $TEST_PORT
TEST_CREAD_PID=$CREAD_NODE_PID

open_node "reference" $REF_CREAD_PATH $REF_NODE_OPT $REF_WORK_PATH $REF_PORT
REF_CREAD_PID=$CREAD_NODE_PID

function cleanup {
   ARG=$1
   if [ $TEST_CREAD_PID -ne -1 ]
   then
      sleep 0.5 && kill -s SIGINT $TEST_CREAD_PID &
      wait -n $TEST_CREAD_PID
      [ $? -ne 0 ] && echo ERROR: $TEST_CREAD_PATH exit failed && EXIT_CODE=-1
   fi

   if [ $REF_CREAD_PID -ne -1 ]
   then
      sleep 0.5 && kill -s SIGINT $REF_CREAD_PID &
      wait -n $REF_CREAD_PID
      [ $? -ne 0 ] && echo ERROR: $REF_CREAD_PATH exit failed && EXIT_CODE=-1
   fi

   wait

   if [ $ARG -eq 0 ]
   then
      exit $EXIT_CODE
   else
      exit $ARG
   fi
}

trap cleanup SIGINT SIGPIPE

echo TEST_CREAD_PID: $TEST_CREAD_PID REF_CREAD_PID: $REF_CREAD_PID
if [ $TEST_CREAD_PID -ne -1 ] &&  [ $REF_CREAD_PID -ne -1 ]; then
   run_test "test_list_votes.py"
   # obsolete: run_test "test_list_votes2.py"
else
   EXIT_CODE=-1
fi

cleanup $EXIT_CODE
