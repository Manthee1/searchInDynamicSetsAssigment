CXX = g++

TARGET_EXEC := searchInDSA

SRC_DIRS := ./src
BUILD_DIR := ./bin

# Check if the gui=0. Run this with: make gui=1
ifeq ($(gui),0)
	$(info Building without GUI)
	ifeq ($(OS) , Windows_NT)
		SRCS := $(shell dir /b /s $(SRC_DIRS)\*.cpp $(SRC_DIRS)\*.c $(SRC_DIRS)\*.s $(SRC_DIRS)\*.S | findstr /v /c:"gui")
	else
		SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s' -or -name '*.S' | grep -v "gui")
	endif
else
	$(info Building with GUI)
	ifeq ($(OS) , Windows_NT)
		SRCS := $(shell dir /b /s $(SRC_DIRS)\*.cpp $(SRC_DIRS)\*.c $(SRC_DIRS)\*.s $(SRC_DIRS)\*.S)
	else
		SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s' -or -name '*.S')
	endif
endif

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

ifeq ($(OS), Windows_NT)
	INC_DIRS := $(shell find $(SRC_DIRS) -type d)
else
	INC_DIRS := $(shell find $(SRC_DIRS) -type d | sed 's/ /\\ /g')
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CXXFLAGS = -std=c++11 $(INC_FLAGS) 
CXXFLAGS += -g -Wall -Wformat -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-field-initializers -MMD -MP 
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------
ifneq ($(gui),0)
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32

	CXXFLAGS += `pkg-config --cflags glfw3`
	CFLAGS = $(CXXFLAGS)
endif
else
CXX += -DNO_GUI
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

ifeq ($(o),1)
CXX += -O4
endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)


$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c -o $@ $<
	

# $(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
# $(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)


valgrind:
	valgrind --leak-check=full --show-leak-kinds=all ./$(BUILD_DIR)/$(TARGET_EXEC)

-include $(DEPS)
