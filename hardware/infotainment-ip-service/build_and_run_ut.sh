#!/bin/bash
set -ue

# root directory
PROJECT_ROOT=`pwd`

CMAKE_FLAGS=$@

function run_unit_test {
	mkdir -p $1/build
	pushd $1/build > /dev/null

	cmake -DENABLE_COVERAGE=1 -DENABLE_TESTS=1 $CMAKE_FLAGS ..
	make -j$(nproc) all coverage

	popd > /dev/null
}

for COMPONENT in libipcommandbus infotainment_ip_service; do
	if [ -d $(pwd)/$COMPONENT/test/ut ]; then
		echo "*** Running unit tests for $COMPONENT"
		run_unit_test $(pwd)/$COMPONENT/test/ut
	else
		echo "Unable to find unit tests for component $COMPONENT in $(pwd)/${COMPONENT}/test/ut"
		exit 1
	fi
	echo ""
done

