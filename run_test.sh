#! /usr/bin/env bash


clean_up()
{
 popd
 exit 1
}

trap clean_up SIGHUP SIGINT SIGTERM


exec_test() {

if [ -e "./$1" ]; then
	echo ">>> Processing $1"
	"./$1" $2 || clean_up
	echo
else
	echo "WARNING: can't find $1"
fi

}

source "$1/DDS_env.sh"

pushd $(pwd)
cd "$1"/tests


echo "Working directory: $(pwd)"
echo

echo "----------------------"
echo "MiscCommon UNIT-TESTs"
echo "----------------------"
exec_test "MiscCommon_test_MiscUtils"
#  we use --catch_system_errors=no here to prevent BOOST catching SIGCHLD
#exec_test "MiscCommon_test_Process" "--catch_system_errors=no"
exec_test "MiscCommon_test_SysHelper"
#exec_test "MiscCommon_test_FindCfgFile"
exec_test "MiscCommon_test_Logger"
exec_test "MiscCommon_test_wordexp"

echo "----------------------"
echo "dds-topology UNIT-TESTs"
echo "----------------------"
exec_test "dds_topology_lib-tests" "--catch_system_errors=no"

echo "----------------------"
echo "dds-topology performance UNIT-TESTs"
echo "----------------------"
exec_test "dds_topology_lib-performance-tests" "--catch_system_errors=no"

echo "----------------------"
echo "Protocol UNIT-TESTs"
echo "----------------------"
exec_test "dds_protocol_lib-ProtocolMessage-tests"
#exec_test "dds-protocol-lib-client-tests"
#exec_test "dds-protocol-lib-server-tests"

echo "----------------------"
echo "Commander UNIT-TESTs"
echo "----------------------"
exec_test "dds-scheduler-tests" "--catch_system_errors=no"

echo "----------------------"
echo "Intercom lib UNIT-TESTs"
echo "----------------------"
exec_test "dds_intercom_lib-tests" "--catch_system_errors=no"

echo "----------------------"
echo "dds-user-defaults UNIT-TESTs"
echo "----------------------"
exec_test "dds-user-defaults-tests"

echo "----------------------"
echo "dds_tools_lib-protocol-tests UNIT-TESTs"
echo "----------------------"
exec_test "dds_tools_lib-protocol-tests"

echo "----------------------"
echo "dds_tools_lib-session-tests UNIT-TESTs"
echo "----------------------"
exec_test "dds_tools_lib-session-tests"

popd

exit 0
