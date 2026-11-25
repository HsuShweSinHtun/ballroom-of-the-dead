CXX ?= c++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRCS = $(wildcard src/*.cpp)
HDRS = $(wildcard include/*.h include/*/*.h)
TARGET = ballroom

all: $(TARGET)

$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
