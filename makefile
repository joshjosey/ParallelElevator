CXX = g++
FLAGS = -lpthread
LIBS = building.cpp
MAIN = main.cpp
TARGET = scheduler_os

all: $(MAIN) $(LIBS)
	$(CXX) $(MAIN) $(LIBS) -o $(TARGET) $(FLAGS)

clean:
	rm -f $(TARGET) make.log