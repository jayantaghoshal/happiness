#include <android/log.h>

#define  LOG_TAG    "netman"

int main(int argc, char* argv[])
{
    __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "Netman started...");
    
    return 0;
}
