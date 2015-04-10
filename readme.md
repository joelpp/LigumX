LIGUMX

This project is TOP SECRET. Reveal any information with care.

--------
BUILDING
--------

You will need to link against a few libraries; binaries are included in the repo
for most of these but you might need to recompile them for your system.

The libraries are:
- GLFW
- GLEW
- FreeImage
- TinyXML2
- GLM
- AntTweakBar

Most straightforward way would be to download and install Visual Studio 2012, available for free at DreamSpark. This will install
the MSVC compiler on your system.

Once this is installed download QT Creator and open the .pro file in the /protoEngine folder. QT will ask you to configure
the project. You should also write a libPaths.pri file and add it to the project. Here is a sample .pri file, modify it
to reflect the correct lib paths on your system :

SAMPLE PRI FILE
---------------

OPENGL_INCLUDEPATH = " "
OPENGL_LIBPATH = " "
OPENGL_DLLPATH = " "

GLFW_INCLUDEPATH = "$$PWD/libs/glfw/include"
GLFW_LIBPATH   = "$$PWD/libs/glfw"
GLFW_DLLPATH = "$$PWD/libs/glfw"

GLEW_INCLUDEPATH = "$$PWD/libs/glew/include"
GLEW_LIBPATH   = "$$PWD/libs/glew/lib/Release/win32"
GLEW_DLLPATH = "$$PWD/libs/glew/bin/Release/win32"

GLM_INCLUDEPATH = "$$PWD/libs/glm"

TINYXML2_INCLUDEPATH = "$$PWD/libs/tinyxml2"

FREEIMAGE_INCLUDEPATH = "$$PWD/libs/FreeImage/Dist/x32/"
FREEIMAGE_DLLPATH = "$$PWD/libs/FreeImage/Dist/x32/"
FREEIMAGE_LIBPATH = "$$PWD/libs/FreeImage/Dist/x32/"

ANTTWEAKBAR_INCLUDEPATH = "$$PWD/libs/AntTweakBar/include/"
ANTTWEAKBAR_LIBPATH = "$$PWD/libs/AntTweakBar/lib/"
ANTTWEAKBAR_DLLPATH = "$$PWD/libs/AntTweakBar/lib/"

---------------
END OF SAMPLE PRI FILE

You should then be able to compile and run.