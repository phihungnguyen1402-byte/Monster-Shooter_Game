include sources/common/container/Makefile.mk

CFLAGS   += -I./sources/common
CPPFLAGS += -I./sources/common

VPATH += sources/common

SOURCES_CPP += sources/common/view_item.cpp
SOURCES_CPP += sources/common/view_render.cpp
SOURCES_CPP += sources/common/screen_manager.cpp

SOURCES += sources/common/utils.c
SOURCES += sources/common/xprintf.c
SOURCES += sources/common/cmd_line.c

CFLAGS   += -I./sources/app
CPPFLAGS += -I./sources/app
CFLAGS   += -I./sources/ak/inc
CPPFLAGS += -I./sources/ak/inc
CFLAGS   += -I./sources/driver/Adafruit_oled_drv
CPPFLAGS += -I./sources/driver/Adafruit_oled_drv
CFLAGS   += -I./sources/platform/stm32l/arduino/cores
CPPFLAGS += -I./sources/platform/stm32l/arduino/cores
CFLAGS   += -DAPP_START_ADDR=0x08003000
CPPFLAGS += -DAPP_START_ADDR=0x08003000
CFLAGS   += -I./sources/driver/buzzer
CPPFLAGS += -I./sources/driver/buzzer
