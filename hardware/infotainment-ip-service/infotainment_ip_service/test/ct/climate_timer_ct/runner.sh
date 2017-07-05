#!/usr/bin/env bash
output_dir_base_path=output
output_dir=$output_dir_base_path/`date +%Y%m%d`

# read the options
TEMP=`getopt -o cnh --long clean,clean-all,no-run,help -- "$@"`
eval set -- "$TEMP"
no_run=false

# extract options and their arguments into variables.
while true ; do
    case "$1" in
        -c|--clean)
            rm -rf $output_dir
            echo "Removed todays test result, $output_dir"
            shift ;;

        --clean-all)
            rm -rf $output_dir_base_path/*
            echo "Removed all test results"
            shift ;;

        -n|--no-run)
            no_run=true
            shift;;
        -h|--help)
            echo "Usage: runner.sh [OPTION] TEST_SUIT_FILE"
            echo "   or: runner.sh [OPTION] TEST_SUIT_DIRECTORY"
            echo "Runs all test cases in TEST_SUIT or TEST_SUIT_DIRECTORY"
            echo ""
            echo "  -c, --clean         Removes all test results from from current day."
            echo "  --clean-all         Removes all test results."
            echo "  -h, --help          Show this help."
            shift;;

        --) shift ; break ;;
        *) echo "Internal error!" ; exit 1 ;;
    esac
done

# if 'no-run' specified, just exit, used in combination with --clean or clean-all
if [ "$no_run" = true ]
    then
        echo "'no-run' specified, exiting without running any tests"
        exit 0
fi

# check that we got at least one test suite file or directory
if [ -z $1 ]
    then
        echo "Please specify at least one test suite file or directory"
        exit 1
fi

# create the output directory
mkdir -p $output_dir

# finally launch robotframework with extended options from 'settings/options'
pybot -A settings/options -d $output_dir  $1
exit $?
