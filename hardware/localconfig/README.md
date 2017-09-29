# LocalConfig

LocalConfig is a library for reading run time tunable parameter. The parameters are
stored in a json file on target. The default file is located in the /vendor/etc folder
and is copied to /oem_config/localconfig on startup if missing.

## Update default LocalConfig

Change the ./misc/localconfig.json file to fit your needs. Run the tests

    ./unit_test_build.sh && ./unit_test_run.sh
    ./component_test_build.sh && ./component_test_run.sh

Build the image and update the IHU. Note that when an IHU is updated with fastboot the
/oem_config/localconfig is not overwritten by default. Therefor you need to explicitly
remove the /oem_config/localconfig/localconfig.json file and reboot the IHU in order to
get your updated LocalConfig to be used.

## Using LocalConfig in your component

### Usage

Add the liblocalconfig library in your Android.mk file:

    LOCAL_SHARED_LIBRARIES += liblocalconfig

Now you can use LocalConfig in your component:

    #include <vcc/localconfig.h>
    auto * lcfg = vcc::LocalConfigDefault();
    int some_param = lcfg->GetInt("Param1");

If you want to provide default value in case parameter is missing you can either
a) initialize variable before using lcfg, and then call:

    lcfg->TryGetValue(&variable, "PARAM_NAME");

b) directly use TryGetValueOrDefault

    lcfg->TryGetValueOrDefault(&variable, 90, "PARAM_NAME");

### Testing

Let's say that you want to test your component which has a dependency to LocalConfig
and you want to change default LocalConfig parameters to verify your handling of the parameters.
Now you can use a test interface to load your own localconfig_mytest.json file, such as:

1. In your test build configuration, replace the shared library liblocalconfig with
   the liblocalconfig_static library. This will allow you for atomic changes in both your component
   and in LocalConfig if needed.

       LOCAL_STATIC_LIBRARIES += liblocalconfig_static

2. When testing classes/components relying on LCFG use DependencyInjection instead of relying on singleton. Do not use auto * lcfg = vcc::LocalConfigDefault();, but instead initialize your own vcc::LocalConfig implementation, or use vcc::mocks::MockLocalConfigReader. 

       #include <vcc/localconfig.h>
       ...
       vcc::LocalConfigFileReader lcfg("/data/local/tmp/localconfig_mytest.json");

       or just vcc::mocks::MockLocalConfigReader lcfg;
       ... setup expectations

       and use &lcfg instead of vcc::LocalConfigDefault()

Quite convenient balance between convenience and testability:

       class SampleLcfgUser {
        public:
         SampleLcfgUser(vcc::LocalConfigReaderInterface * lcfg = vcc::LocalConfigDefault()) {
           //use or store lcfg
         }
       }

This does not require passing pointer through all layers, but also allows testing code relying on lcfg with injected implementation/mock.

3. In your test case, copy your own localconfig_mytest.json to target /data/local/tmp/localconfig_mytest.json.
   Example on how this can be done in VTS is available in the unit test for localconfig.

## Verification

The unit tests and component tests is implemented for usage with VTS. Unit tests are also host compatible.
