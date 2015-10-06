program_NAME := LigumX
program_CXX_SRCS := $(wildcard src/*.cpp src/init/*.cpp src/init/pipelines/*.cpp)
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)
program_INCLUDE_DIRS := ./libs/GLFW/include/ ./libs/glm/ ./libs/AntTweakBar/include/ ./libs/FreeImage/OSX/ ./libs/Freetype ./libs/
program_LIBRARY_DIRS := ./libs/AntTweakBar/lib/ ./libs/FreeImage/OSX/ ./libs/Freetype .libs/glfw/ 
program_LIBRARIES := AntTweakBar glew glfw freeimage freetype spatialindex spatialindex_c \

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	g++ -g -stdlib=libc++ -std=c++11 $(program_OBJS) -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean