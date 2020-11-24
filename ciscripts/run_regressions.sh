#/bin/sh

BUILD_DIR="${PWD}/build"
TESTS_DIR=${BUILD_DIR}/tests

cd $TESTS_DIR

# $1 unit test name group
execute_unittest_group()
{
  local unit_test_group=$1
  echo "Start unit tests group '${unit_test_group}'"
  if ! ctest -R ^unit/${unit_test_group}.* --output-on-failure -vv
  then
    exit 1
  fi
}

execute_crea_functional()
{
  echo "Start crea functional tests"
  if ! ctest -R ^functional/.* --output-on-failure -vv
  then
    exit 1
  fi
}

echo "      _____                    _____                    _____                _____                    _____          ";
echo "     /\    \                  /\    \                  /\    \              /\    \                  /\    \         ";
echo "    /::\    \                /::\    \                /::\    \            /::\    \                /::\    \        ";
echo "    \:::\    \              /::::\    \              /::::\    \           \:::\    \              /::::\    \       ";
echo "     \:::\    \            /::::::\    \            /::::::\    \           \:::\    \            /::::::\    \      ";
echo "      \:::\    \          /:::/\:::\    \          /:::/\:::\    \           \:::\    \          /:::/\:::\    \     ";
echo "       \:::\    \        /:::/__\:::\    \        /:::/__\:::\    \           \:::\    \        /:::/__\:::\    \    ";
echo "       /::::\    \      /::::\   \:::\    \       \:::\   \:::\    \          /::::\    \       \:::\   \:::\    \   ";
echo "      /::::::\    \    /::::::\   \:::\    \    ___\:::\   \:::\    \        /::::::\    \    ___\:::\   \:::\    \  ";
echo "     /:::/\:::\    \  /:::/\:::\   \:::\    \  /\   \:::\   \:::\    \      /:::/\:::\    \  /\   \:::\   \:::\    \ ";
echo "    /:::/  \:::\____\/:::/__\:::\   \:::\____\/::\   \:::\   \:::\____\    /:::/  \:::\____\/::\   \:::\   \:::\____\ ";
echo "   /:::/    \::/    /\:::\   \:::\   \::/    /\:::\   \:::\   \::/    /   /:::/    \::/    /\:::\   \:::\   \::/    /";
echo "  /:::/    / \/____/  \:::\   \:::\   \/____/  \:::\   \:::\   \/____/   /:::/    / \/____/  \:::\   \:::\   \/____/ ";
echo " /:::/    /            \:::\   \:::\    \       \:::\   \:::\    \      /:::/    /            \:::\   \:::\    \     ";
echo "/:::/    /              \:::\   \:::\____\       \:::\   \:::\____\    /:::/    /              \:::\   \:::\____\    ";
echo "\::/    /                \:::\   \::/    /        \:::\  /:::/    /    \::/    /                \:::\  /:::/    /    ";
echo " \/____/                  \:::\   \/____/          \:::\/:::/    /      \/____/                  \:::\/:::/    /     ";
echo "                           \:::\    \               \::::::/    /                                 \::::::/    /      ";
echo "                            \:::\____\               \::::/    /                                   \::::/    /       ";
echo "                             \::/    /                \::/    /                                     \::/    /        ";
echo "                              \/____/                  \/____/                                       \/____/         ";
echo "                                                                                                                     ";




execute_unittest_group plugin_test
execute_unittest_group chain_test

execute_crea_functional

exit 0
