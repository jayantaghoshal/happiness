#include <linux/rtnetlink.h>

#include <sys/socket.h>
#include <sys/un.h>
#include <utils/String16.h>

#define LOG_TAG "NetdSocketTest"

#include <cutils/log.h>

int main(int argc, char **argv) {
  int mChannel = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
  if (mChannel == -1) {
    return -errno;
  }
  const sockaddr_un NETD_SERVER_PATH = {AF_UNIX, "/dev/socket/netd"};

  if (TEMP_FAILURE_RETRY(
          connect(mChannel, reinterpret_cast<const sockaddr *>(&NETD_SERVER_PATH), sizeof(NETD_SERVER_PATH))) == -1) {
    return 0;
  }

  const int kRecvBufferSize = 4096;
  char buf[kRecvBufferSize] = {0};
  struct iovec iov = {.iov_base = buf, .iov_len = sizeof(buf)};
  struct sockaddr_nl sa;
  struct msghdr msg = {.msg_name = (void *)&sa,
                       .msg_namelen = sizeof(sa),
                       .msg_iov = &iov,
                       .msg_iovlen = 1,
                       .msg_control = NULL,
                       .msg_controllen = 0,
                       .msg_flags = 0};

  struct nlmsghdr *nl_message_header;
  int message_length;
  for (;;) {
    message_length = TEMP_FAILURE_RETRY(recvmsg(mChannel, &msg, 0));

    ALOGI("MESSAGE: %s", buf);
    if (message_length <= 0) {
      exit(1);
    }
  }

  close(mChannel);

  exit(0);
}
