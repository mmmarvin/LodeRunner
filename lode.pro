TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

HOME = $$system(echo $HOME)
INCLUDE_PATH_SDL2 = $${HOME}/userspace/shared/include
#LIB_PATH_SDL2 = $${HOME}/userspace/shared/lib64
LIB_NAME_SDL2 = -lSDL2
LIB_NAME_SDL2_IMAGE = -lSDL2_image
LIB_NAME_SDL2_TTF = -lSDL2_ttf
INCLUDE_PATH_SKA = $${HOME}/userspace/include

HEADERS += \
    camera.h \
    character.h \
    define.h \
    game.h \
    gamestate.h \
    ingamestate.h \
    map.h \
    object.h \
    rect.h \
    rect.inl \
    renderer.h \
    macro.h \
    3rdparty/high_resolution_clock.h \
    3rdparty/time.h \
    text.h \
    texture.h \
    vec.h \
    vec.inl

SOURCES += \
    camera.cpp \
    character.cpp \
    ingamestate.cpp \
    ingamestate_resources.cpp \
    lode.cpp \
    game.cpp \
    map.cpp \
    object.cpp \
    renderer.cpp \
    3rdparty/high_resolution_clock.cpp \
    3rdparty/time.cpp \
    3rdparty/time.inl \
    text.cpp \
    texture.cpp

INCLUDEPATH += \
#  $${INCLUDE_PATH_SDL2} \
  $${INCLUDE_PATH_SKA}

LIBS += \
  $${LIB_NAME_SDL2} \
  $${LIB_NAME_SDL2_IMAGE} \
  $${LIB_NAME_SDL2_TTF}
