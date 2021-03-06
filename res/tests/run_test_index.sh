#! /usr/bin/env bash

location=$1
echo "DDS from this location is used ${location}"
source ${location}/DDS_env.sh

topologyFile=${DDS_LOCATION}/tests/index_test.xml
resultFile=${DDS_LOCATION}/tests/index_test_result.txt
tmpResultFile=${DDS_LOCATION}/tests/index_test_result_tmp.txt
requiredNofAgents=6

echo "Starting DDS server..."
startOutput=$(dds-session start --local)

echo "${startOutput}"

sessionID=$(echo -e "${startOutput}" | head -1 | awk '{split($0,a,":"); print a[2]}' |  tr -d '[:space:]')

echo "SESSION ID: ${sessionID}"

echo "Submiting agents..."
dds-submit -r localhost -n ${requiredNofAgents} --session ${sessionID}

counter=0
nofAgents=$(dds-info -n --session ${sessionID})
while [ ${nofAgents} -lt ${requiredNofAgents} ]; do
    nofAgents=$(dds-info -n --session ${sessionID})
    let counter=counter+1
    if [ ${counter} -gt 20 ]; then
        echo "Error: not enough agents"
        exit 1
    fi
    sleep 1
done

echo "Activating topology..."
dds-topology --disable-validation --session ${sessionID} --activate ${topologyFile}

# Give 30 seconds for tasks to finish
echo "Waiting 30 seconds..."
sleep 30

echo "Getting logs..."
dds-agent-cmd getlog -a --session ${sessionID}

wrkDir=$(dds-user-defaults --session ${sessionID} -V --key server.work_dir)
logDir=$(eval echo "${wrkDir}/log/agents")
echo "Search for logs in: ${logDir}"

for file in $(find "${logDir}" -name "*.tar.gz"); do tar -xf ${file} -C "${logDir}" ; done

nofGoodResults=$(grep -r --include "*.log" "USER TASK test passed" "${logDir}" | wc -l)

if [ "${nofGoodResults}" -eq "${requiredNofAgents}" ]
then
echo "Test passed."
else
echo "Test failed: number of good results is ${nofGoodResults}, required number of good results is ${requiredNofAgents}"
fi

echo "Stoping server..."
dds-session stop ${sessionID}

exit 0
