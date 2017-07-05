# LocalConfig

LocalConfig is a library for reading run time tunable parameter. The parameters are
stored in a json file on target. The default file is located in the /vendor/etc folder
and is copied to /oem_config/localconfig on startup if missing.

## Using LocalConfig in your component

### Usage

Add the liblocalconfig library in your Android.mk file:

    LOCAL_SHARED_LIBRARIES += liblocalconfig

Now you can use LocalConfig in your component:

    #include <vcc/localconfig.h>
    ...
    int some_param = vcc::localconfig::GetInt("Param1");

### Testing

Let's say that you want to test your component which has a dependency to LocalConfig
and you want to change default LocalConfig parameters to verify your handling of the parameters.
Now you can use a test interface to load your own localconfig_mytest.json file, such as:

1. In your test build configuration, replace the shared library liblocalconfig with
   the liblocalconfig_static library. This will allow you for atomic changes in both your component
   and in LocalConfig if needed.

       LOCAL_STATIC_LIBRARIES += liblocalconfig_static

2. In your test code, initialize LocalConfig before executing the code which depends on
   LocalConfig:

       #include <localconfig_test.h>
       ...
       vcc::localconfig::testInit("/data/local/tmp/localconfig_mytest.json");

3. In your test case, copy your own localconfig_mytest.json to target /data/local/tmp/localconfig_mytest.json.
   Example on how this can be done in VTS is available in the unit test for localconfig.

## Verification

The unit tests and component tests is implemented using VTS/Trade Federation. To build and run test, please run:

    ./unit_test_build.sh && ./unit_test_run.sh
    or
    ./component_test_build.sh && ./component_test_run.sh

Note: that we have separated the build and run stages as this is needed for the CI-machinery where we have seperate
servers for build and test.
