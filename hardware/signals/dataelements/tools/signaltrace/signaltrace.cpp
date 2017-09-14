
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include "vendor/volvocars/hardware/signals/1.0/ISignals.h"
#include "vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h"
#include "vendor/volvocars/hardware/signals/1.0/types.h"

#undef LOG_TAG
#define LOG_TAG "SignalTrace"
#include <cutils/log.h>

using namespace vendor::volvocars::hardware::signals::V1_0;

static bool listSignals = false;
static bool allSignals = true;
static std::string tag = "*";
static Dir dir;

class SignalChangedCallback : public ISignalsChangedCallback
{
 public:
  ::android::hardware::Return<void> signalChanged(const ::android::hardware::hidl_string& signalName, Dir dir,
                                                  const ::android::hardware::hidl_string& data)
  {
    printf("\n %s %s", signalName.c_str(), data.c_str());
    return ::android::hardware::Void();
  }
};

void printHelp()
{
  printf("\nUsage: [in|out|internal] [SIGNALS] [-l, -list]\n");
  printf("Example: 'signaltrace' trace all signals\n");
  printf("Example: 'signaltrace in' trace all signals received by IHU\n");
  printf("Example: 'signaltrace Prof*' trace all signals that starts with \"Prof\"\n");
  printf("Example: 'signaltrace in *Mod*' trace all in signals that contains string \"Mod\"\n");
  printf("Example: 'signaltrace -list' Lists all received in signals last received value \n\n");
}

bool handleArguments(int argc, char* argv[])
{
  for (int i = 1; i < argc; i++)
  {
    std::string arg = argv[i];
    if (arg == "in" || arg == "IN")
    {
      dir = Dir::IN;
      allSignals = false;
    }
    else if (arg == "out" || arg == "OUT")
    {
      dir = Dir::OUT;
      allSignals = false;
    }
    else if (arg == "internal" || arg == "INTERNAL")
    {
      dir = Dir::INTERNAL;
      allSignals = false;
    }
    else if (arg == "-l" || arg == "--list" || arg == "-list")
    {
      listSignals = true;
    }
    else if (arg == "--help" || arg == "-help" || arg == "-h")
    {
      printHelp();
      return false;
    }
    else
    {
      tag = arg;
    }
  }
  return true;
}

void printCurrentSignalsForDir(Dir dir, std::string prefix)
{
    ::android::hardware::hidl_vec<Result> result;
    auto _hidl_cb = [&](const ::android::hardware::hidl_vec<Result>& data) {
        result = data;
    };
    ::android::sp<ISignals> service = ISignals::getService();
    if(!service->get_all(tag, dir, _hidl_cb).isOk())
        ALOGE("Failed to call get_all() to server");
    for (size_t i = 0; i < result.size(); i++)
    {
        printf("%s Signal: %s Data: %s\n", prefix.c_str(), result[i].name.c_str(), result[i].value.c_str());
    }
}

void printCurrentSignals(Dir dir)
{
  if (!allSignals)
  {
      switch (dir)
      {
          case Dir::IN:
            printf("List of all current in signals:\n");
            printCurrentSignalsForDir(dir, "<-");
            break;
          case Dir::OUT:
            printf("List of all current out signals:\n");
            printCurrentSignalsForDir(dir, "->");
            break;
          case Dir::INTERNAL:
            printf("List of all current internal signals:\n");
            printCurrentSignalsForDir(dir, "[INTERNAL]");
            break;
      }
  }
  else
  {
      printf("List of all current signals:\n");
      printCurrentSignalsForDir(Dir::IN, "<-");
      printCurrentSignalsForDir(Dir::OUT, "->");
      printCurrentSignalsForDir(Dir::INTERNAL, "[INTERNAL]");
  }
}

int main(int argc, char* argv[])
{
    if (!handleArguments(argc, argv))
        return 0;
    ::android::sp<ISignals> service = ISignals::getService();
    ::android::sp<ISignalsChangedCallback> signalChanged = new SignalChangedCallback();
    if (listSignals)
    {
      printCurrentSignals(dir);
    }
    else
    {
      if (!allSignals)
      {
          switch (dir)
          {
              case Dir::IN:
                if(!service->subscribe(tag, dir, signalChanged).isOk())
                    ALOGE("Failed to subscribe to server");
                break;
              case Dir::OUT:
                if(!service->subscribe(tag, dir, signalChanged).isOk())
                    ALOGE("Failed to subscribe to server");
                break;
              case Dir::INTERNAL:
                if(!service->subscribe(tag, dir, signalChanged).isOk())
                    ALOGE("Failed to subscribe to server");
                break;
          }
      }
      else
      {
          if(!service->subscribe(tag, Dir::IN, signalChanged).isOk())
            ALOGE("Failed to subscribe to server");
          if(!service->subscribe(tag, Dir::OUT, signalChanged).isOk())
            ALOGE("Failed to subscribe to server");
          if(!service->subscribe(tag, Dir::INTERNAL, signalChanged).isOk())
            ALOGE("Failed to subscribe to server");
      }
    }
    while(true) {
      std::this_thread::sleep_for (std::chrono::seconds(1));
    }
}
