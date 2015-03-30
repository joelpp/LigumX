TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/libPaths.pri)

INCLUDEPATH += \
    $$OPENGL_INCLUDEPATH \
    $$GLFW_INCLUDEPATH \
    $$GLEW_INCLUDEPATH \
    $$GLM_INCLUDEPATH \

LIBS += -L$$OPENGL_LIBPATH -L$$OPENGL_DLLPATH -lopengl32
LIBS += -L$$GLFW_LIBPATH -L$$GLFW_DLLPATH -lglfw3dll
LIBS += -L$$GLEW_LIBPATH -L$$GLEW_DLLPATH -lglew32

SOURCES += \
    src/ProgramPipeline.cpp \
    src/Game.cpp \
    src/Camera.cpp

HEADERS += \
    src/ProgramPipeline.h \
    src/Game.h \
    src/Camera.h

