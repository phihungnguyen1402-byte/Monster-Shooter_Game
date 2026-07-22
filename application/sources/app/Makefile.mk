include sources/app/screens/Makefile.mk
include sources/app/game/Makefile.mk
VPATH += sources/app

SOURCES_CPP += sources/app/app.cpp
SOURCES_CPP += sources/app/app_bsp.cpp
SOURCES_CPP += sources/app/app_data.cpp
SOURCES_CPP += sources/app/app_non_clear_ram.cpp
SOURCES_CPP += sources/app/task_life.cpp
SOURCES_CPP += sources/app/task_display.cpp
SOURCES_CPP += sources/app/task_list.cpp
SOURCES_CPP += sources/app/task_system.cpp
SOURCES_CPP += sources/app/task_shell.cpp
SOURCES_CPP += sources/app/task_dbg.cpp
SOURCES_CPP += sources/app/task_fw.cpp
SOURCES_CPP += sources/app/task_if.cpp
SOURCES_CPP += sources/app/task_uart_if.cpp
SOURCES_CPP += sources/app/task_rf24_if.cpp
SOURCES_CPP += sources/app/shell.cpp
SOURCES_CPP += sources/app/app_modbus_pull.cpp

CPPFLAGS += -I./sources/app
