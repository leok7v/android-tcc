APP_ABI := armeabi-v7a
APP_PLATFORM := android-17
#APP_STL := gnustl_shared
APP_CFLAGS := -Wall -funwind-tables $(ANT_CFLAGS) -fexceptions -Wno-unused-variable
#APP_CPPFLAGS += -std=c++11

ifeq ("$(NDK_DEBUG)","1")
#   $(info "NDK_DEBUG=1")
    APP_CFLAGS += -DDEBUG -UNDEBUG -g
    APP_OPTIM := debug
else
#   $(info "NDK_DEBUG=0")
    APP_CFLAGS += -UDEBUG -DNDEBUG -DRELEASE -Ofast
    APP_OPTIM := release
endif