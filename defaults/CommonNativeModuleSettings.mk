# Set mandatory vars

LOCAL_VENDOR_MODULE:= true
LOCAL_MULTILIB:= 64

# Append to module existing settings
LOCAL_CFLAGS+= -Wall \
               -Wextra \
               -Wmissing-field-initializers \
               -Wunused-variable \
               -Werror=macro-redefined

# On some platforms even C needs this flag to forward exceptions through callback etc.
LOCAL_CFLAGS+= -fexceptions