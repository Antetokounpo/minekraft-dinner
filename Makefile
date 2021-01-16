# http://www.partow.net/programming/makefile/index.html

CXX      := g++
CXXFLAGS := -Wall -std=c++17 -DGLM_ENABLE_EXPERIMENTAL
LDFLAGS  := -lSDL2 -lSDL2_image -lGL -lGLEW
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
TARGET   := minekraft
INCLUDE  := -Isrc/
SRC      := $(wildcard src/*.cpp) \
			$(wildcard src/engine/*.cpp) \
			$(wildcard src/engine/loaders/*.cpp) \
			$(wildcard src/engine/renderer/*.cpp) \
			$(wildcard src/terrain/*.cpp)

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(BUILD)/$(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(BUILD)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(BUILD)/$(TARGET) $^ $(LDFLAGS)

.PHONY: all build clean debug release

build:
	@mkdir -p $(BUILD)
	@mkdir -p $(OBJ_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

profile: CXXFLAGS += -pg
profile: debug

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(BUILD)/*