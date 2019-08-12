NAME = paste-light
VERSION = 0.1

CXX ?= g++

SRC_PATH = src
BUILD_PATH = build
BIN_PATH = $(BUILD_PATH)/bin

BIN_NAME = paste-light

SRC_EXT = cpp

SOURCES = $(shell find $(SRC_PATH) -name '*.$(SRC_EXT)' | sort -k 1nr | cut -f2-)
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
DEPS = $(OBJECTS:.o=.d)

COMPILE_FLAGS = -std=c++11 -Wall -Wextra -g
INCLUDES = -I include/ -I /usr/local/include
LIBS = -I libs/maddy/include

TEST_LIBS = -I libs/maddy/include -I libs/doctest/doctest

.PHONY: default_target
default_target: release

.PHONY: release
release: export CXXFLAGS := $(CXXFLAGS) $(COMPILE_FLAGS)
release: dirs
	@$(MAKE) all

.PHONY: dirs
dirs:
	@echo "Creating directories"
	@mkdir -p $(dir $(OBJECTS))
	@mkdir -p $(BIN_PATH)

.PHONY: clean
clean:
	@echo "Deleting $(BIN_NAME) symlink"
	@$(RM) $(BIN_NAME)
	@echo "Deleting directories"
	@$(RM) -r $(BUILD_PATH)
	@$(RM) -r $(BIN_PATH)

.PHONY: all
all: $(BIN_PATH)/$(BIN_NAME)
	@echo "Making symlink: $(BIN_NAME) -> $<"
	@$(RM) $(BIN_NAME)
	@ln -s $(BIN_PATH)/$(BIN_NAME) $(BIN_NAME)

.PHONY: uninstall
uninstall:
	@echo "Uninstalling $(BIN_NAME)"
	@rm /usr/bin/paste-light
	@rm /usr/local/man/man1/paste-light.1.gz
	@rm -rf /usr/local/share/paste-light

.PHONY: install
install:
	@echo "Installing $(BIN_NAME)"
	@cp -fv build/bin/$(BIN_NAME) /usr/bin/paste-light
	@mkdir -p /usr/local/share/paste-light
	@cp -r themes /usr/local/share/paste-light/themes
	@cp -r server /usr/local/share/paste-light/server
	@cp -f README.md /usr/local/share/paste-light/README
	@cp -f docs/paste-light /usr/local/man/man1/paste-light.1
	@gzip /usr/local/man/man1/paste-light.1

.PHONY: tests
tests:
	@echo "Building tests"
	$(CXX) $(COMPILE_FLAGS) $(INCLUDES) $(TEST_LIBS) tests/test_paste_light.cpp -o tests/test_paste_light

$(BIN_PATH)/$(BIN_NAME): $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(OBJECTS) -o $@

-include $(DEPS)

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LIBS) -MP -MMD -c $< -o $@
