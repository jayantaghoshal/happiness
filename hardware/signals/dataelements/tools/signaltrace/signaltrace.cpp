
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>

#include "vendor/volvocars/hardware/signals/1.0/ISignals.h"
#include "vendor/volvocars/hardware/signals/1.0/ISignalsChangedCallback.h"
#include "vendor/volvocars/hardware/signals/1.0/types.h"

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
  printf(
      "Example: 'signaltrace in -list' First lists all received in signals last received value then trace all in "
      "signals \n\n");
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
  auto _hidl_cb = [&](const ::android::hardware::hidl_vec<Result>& data) { result = data; };
  ::android::sp<ISignals> service = ISignals::getService();
  auto res = service->get_all(tag, dir, _hidl_cb);
  if (!res.isOk())
  {
    printf("Failed to call get_all() to server. Description: %s", res.description().c_str());
    return;
  }
  for (const auto& r : result)
  {
    printf("%s Signal: %s Data: %s\n", prefix.c_str(), r.name.c_str(), r.value.c_str());
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
  if (!handleArguments(argc, argv)) return 0;
  ::android::sp<ISignals> service = ISignals::getService();
  ::android::sp<ISignalsChangedCallback> signalChanged = new SignalChangedCallback();
  if (listSignals)
  {
    printCurrentSignals(dir);
  }
  if (!allSignals)
  {
    auto subscribe = service->subscribe(tag, dir, signalChanged);
    if (!subscribe.isOk())
    {
      printf("Failed to subscribe to server. Description: %s", subscribe.description().c_str());
      return 0;
    }
  }
  else
  {
    auto subscribe = service->subscribe(tag, Dir::IN, signalChanged);
    if (!subscribe.isOk())
    {
      printf("Failed to subscribe to server. Description: %s", subscribe.description().c_str());
      return 0;
    }
    subscribe = service->subscribe(tag, Dir::OUT, signalChanged);
    if (!subscribe.isOk())
    {
      printf("Failed to subscribe to server. Description: %s", subscribe.description().c_str());
      return 0;
    }
    subscribe = service->subscribe(tag, Dir::INTERNAL, signalChanged);
    if (!subscribe.isOk())
    {
      printf("Failed to subscribe to server. Description: %s", subscribe.description().c_str());
      return 0;
    }
  }
  while (true)
  {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}
