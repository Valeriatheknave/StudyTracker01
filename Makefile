CXX ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -pedantic -O2
TARGET = study_tracker
SRC = study_tracker_V01.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe tasks.txt tasks.txt.tmp

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
