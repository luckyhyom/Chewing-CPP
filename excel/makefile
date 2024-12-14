CXX = g++
CXXFLAGS = -Wall -g
TARGET = table_program
SOURCES = main.cpp Cell.cpp Table.cpp TextTable.cpp CSVTable.cpp HTMLTable.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
