CXX		:=g++
CXXFLAGS:= -std=c++17 -O2 -Wall -Iinclude
BUILD	:= build
TARGET	:= program

SRCS	:= $(wildcard src/*.cpp)
OBJS	:= $(patsubst src/%.cpp, $(BUILD)/%.o, $(SRCS))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: src/%.cpp | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD) $(TARGET)

.PHONY: all clean