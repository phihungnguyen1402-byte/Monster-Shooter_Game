VPATH += sources/app/game
VPATH += sources/app/game/entity
VPATH += sources/app/game/engine
VPATH += sources/app/game/resource
VPATH += sources/app/game/ui

CPPFLAGS += -I./sources/app/game
CPPFLAGS += -I./sources/app/game/entity
CPPFLAGS += -I./sources/app/game/engine
CPPFLAGS += -I./sources/app/game/resource
CPPFLAGS += -I./sources/app/game/ui

SOURCES_CPP += sources/app/game/entity/player.cpp
SOURCES_CPP += sources/app/game/engine/game_renderer.cpp
SOURCES_CPP += sources/app/game/engine/game_manager.cpp
SOURCES_CPP += sources/app/game/engine/input.cpp
SOURCES_CPP += sources/app/game/entity/bullet.cpp
SOURCES_CPP += sources/app/game/entity/enemy.cpp
SOURCES_CPP += sources/app/game/engine/collision.cpp
SOURCES_CPP += sources/app/game/entity/boss.cpp
SOURCES_CPP += sources/app/game/entity/boss_bullet.cpp
SOURCES_CPP += sources/app/game/sound_manager.cpp
