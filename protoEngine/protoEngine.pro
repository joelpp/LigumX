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
    $$TINYXML2_INCLUDEPATH \

LIBS += -L$$OPENGL_LIBPATH -L$$OPENGL_DLLPATH -lopengl32
LIBS += -L$$GLFW_LIBPATH -L$$GLFW_DLLPATH -lglfw3dll
LIBS += -L$$GLEW_LIBPATH -L$$GLEW_DLLPATH -lglew32

SOURCES += \
    src/ProgramPipeline.cpp \
    src/Game.cpp \
    src/Camera.cpp \
    src/way.cpp \
    src/node.cpp \
    src/tinyxml2.cpp \
    src/relation.cpp \
    src/OSMElement.cpp


HEADERS += \
    src/ProgramPipeline.h \
    src/Camera.h \
    src/way.h \
    src/node.h \
    src/tinyxml2.h \
    src/Game.h \
    src/relation.h \
    src/OSMElement.h

OTHER_FILES += \
    src/shaders/roads.gsh \
    src/shaders/roads2.gsh \
    src/shaders/roads3.gsh

