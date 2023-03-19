CXX = g++

TARGET_EXEC := searchInDSA

SRC_DIR := ./src
BUILD_DIR := ./bin

# Check if the gui=0. Run this with: make gui=1
ifeq ($(gui),0)
$(info Building without GUI)
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s' -or -name '*.S' | grep -v "gui")
else
$(info Building with GUI)
SRCS := $(shell find $(SRC_DIR) -name '*.cpp' -or -name '*.c' -or -name '*.s' -or -name '*.S')
endif

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

INC_DIRS := $(shell find $(SRC_DIR) -type d | sed 's/ /\\ /g')
INC_FLAGS := $(addprefix -I,$(INC_DIRS))
LIBS:=

ifneq ($(gui),0)
ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) `pkg-config --static --libs glfw3`

	CXXFLAGS += `pkg-config --cflags glfw3`
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
	LIBS += -L/usr/local/lib -L/opt/local/lib -L/opt/homebrew/lib
	#LIBS += -lglfw3
	LIBS += -lglfw

	CXXFLAGS += -I/usr/local/include -I/opt/local/include -I/opt/homebrew/include
endif


CXXFLAGS = -std=c++11 $(INC_FLAGS) 
CXXFLAGS += -g -Wall -Wformat -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-unused-function -Wno-missing-field-initializers -MMD -MP 

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
