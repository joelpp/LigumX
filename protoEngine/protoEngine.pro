TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include($$PWD/libPaths.pri)

#Platform independent
INCLUDEPATH += \
    $$GLM_INCLUDEPATH \
    $$FREEIMAGE_INCLUDEPATH \
    $$ANTTWEAKBAR_INCLUDEPATH \
    $$TINYXML2_INCLUDEPATH \
    $$FREETYPE_INCLUDEPATH

LIBS += -L$$ANTTWEAKBAR_LIBPATH -L$$ANTTWEAKBAR_DLLPATH -L$$FREETYPE_LIBPATH


unix {
    #on linux, you need packages :
    # - libglfw3-dev
    # - libglew-dev (>= 1.11 !!)
    # - libfreeimage-dev
    QMAKE_CXXFLAGS += "-std=c++11 $$system(pkg-config --cflags glew) $$system(pkg-config --cflags glfw3) $$system(pkg-config --cflags box2d)"
    LIBS += -L/usr/local/lib /usr/local/lib/libGLEW.a -lGLU -lGL #$$system(pkg-config --libs glew)
    LIBS += $$system(pkg-config --libs glfw3) $$system(pkg-config --libs box2d)
    LIBS += -lX11 -lXrandr -lXi -lXxf86vm -lpthread
    LIBS += -lAntTweakBar
    LIBS += -lfreeimage
    LIBS += -lfreetype
}

win32 {
    INCLUDEPATH += $$OPENGL_INCLUDEPATH $$GLEW_INCLUDEPATH $$GLFW_INCLUDEPATH
    LIBS += -L$$FREEIMAGE_LIBPATH -L$$FREEIMAGE_DLLPATH
    LIBS += -L$$GLEW_LIBPATH -L$$GLEW_DLLPATH
    LIBS += -L$$GLFW_LIBPATH -L$$GLFW_DLLPATH
    LIBS += -L$$OPENGL_LIBPATH -L$$OPENGL_DLLPATH
    LIBS += -lAntTweakBar -lFreeImage -lglew32 -lglfw3dll -lopengl32 -lfreetype
}


SOURCES += \
    src/program_pipeline.cpp \
    src/game.cpp \
    src/camera.cpp \
    src/way.cpp \
    src/node.cpp \
    src/tinyxml2.cpp \
    src/relation.cpp \
    src/osm_element.cpp \
    src/texture.cpp \
    src/filter.cpp \
    src/linesegment.cpp \
    src/callbacks.cpp \
    src/main_loop.cpp \
    src/heightfield.cpp \
    src/init/pipelines/filled_buildings.cpp \
    src/init/pipelines/screen_quad.cpp \
    src/init/pipelines/roads.cpp \
    src/init/pipelines.cpp \
    src/init.cpp \
    src/init/pipelines/ground_triangles.cpp \
    src/init/pipelines/general_lines.cpp \
    src/init/pipelines/building_sides.cpp \
    src/init/pipelines/envmap.cpp \
    src/init/pipelines/text.cpp \
    src/init/tweak_bar.cpp \
    src/query.cpp \
    src/load.cpp \
    src/triangle.cpp \
    src/entity.cpp \
    src/world.cpp


HEADERS += \
    src/program_pipeline.h \
    src/camera.h \
    src/way.h \
    src/node.h \
    src/tinyxml2.h \
    src/game.h \
    src/relation.h \
    src/osm_element.h \
    src/texture.h \
    src/filter.h \
    src/common.h \
    src/linesegment.h \
    src/heightfield.h \
    src/triangle.h \
    src/entity.h \
    src/world.h

OTHER_FILES += \
    src/shaders/roads.gsh \
    src/shaders/roads2.gsh \
    src/shaders/roads3.gsh \
    src/shaders/buildingSides.gsh \
    src/shaders/pipeline_envmap/vertex.vsh \
    src/shaders/pipeline_envmap/fragment.fsh\
    src/shaders/pipeline_ground/vertex.vsh \
    src/shaders/pipeline_ground/fragment.fsh
DISTFILES += \
    libPaths.pri \
    src/shaders/text/fragment.fsh \
    src/shaders/text/vertex.vsh

