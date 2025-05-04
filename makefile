CXX = g++
# Compiler flags
CXXFLAGS = -Wall -Wextra -O2

# Linker flags
LDFLAGS = -pthread -lcurl

# Libraries
LIBS = building.cpp api_control.cpp

# Source file
MAIN = main.cpp

# Target executable
TARGET = scheduler_os

all: $(MAIN) $(LIBS)
	$(CXX) $(MAIN) $(LIBS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.txt *.log *.out