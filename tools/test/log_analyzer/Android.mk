# Copyright 2017 Volvo Car Corporation
# This file is covered by LICENSE file in the root of this project

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

# Only compile source java files in this lib.
LOCAL_SRC_FILES := $(call all-java-files-under, src)

LOCAL_JAVACFLAGS += -g -Xlint

LOCAL_MODULE := vccLogTests

LOCAL_MODULE_TAGS := optional
LOCAL_JAVA_LIBRARIES := tradefed loganalysis

LOCAL_JAR_MANIFEST := MANIFEST.mf

include $(BUILD_HOST_JAVA_LIBRARY)

# makefile rules to copy jars to HOST_OUT/tradefed
# so tradefed.sh can automatically add to classpath
DEST_JAR := $(HOST_OUT)/tradefed/$(LOCAL_MODULE).jar
$(DEST_JAR): $(LOCAL_BUILT_MODULE)
	$(copy-file-to-new-target)

# this dependency ensure the above rule will be executed if jar is built
$(LOCAL_INSTALLED_MODULE) : $(DEST_JAR)
