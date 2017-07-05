#!/bin/bash

CT_HELPER_SCRIPTS_PATH=$(dirname $0)/test_utils

case $1 in
"run")
    # Run the component test
    cd infotainment_ip_service/test/ct/service_started
    ./test.sh

    if [ $? -eq 0 ]; then
        echo "Component build SUCCESS!"
        exit 0
    else
        echo "Component build FAILED!"
        exit 1
    fi
    ;;
"build")
    if [ -f $CT_HELPER_SCRIPTS_PATH/ihu_component_ct_build.sh ]; then
        $CT_HELPER_SCRIPTS_PATH/ihu_component_ct_build.sh $(dirname $0)/build
        exit $?
    fi
    ;;
*)
    echo "USAGE: $0 <run|build>"
    exit 1
    ;;
esac