HalModuleSink App is created for testing and debugging purpose.
Each module represents a set functions that will be tested.

Creation of a model:

1) fragment_one.xml is the example template for creating a HMI layout in the "res" folder
2) FragmentOne.java is the example template for the fragment implementation
3) Menu is need to be added in the Main activity and look for "Fragment init and add Menus"
4) Menu name and id need to be added in the menu.xml in "res/menu/main.xml"
5) Installation

adb install -r ~/$ANDROID_BUILD_TOP/out/target/product/ihu_vcc/system/priv-app/HalModulesSink/HalModulesSink.apk
adb shell am start -n "com.volvocars.halmodulesink/com.volvocars.halmodulesink.MainActivity" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER

