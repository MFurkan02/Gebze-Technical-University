CXX = g++
CXXFLAGS = -Wall -g -Iinclude
LDFLAGS = -lglfw -ldl -lGL -lassimp

SRC = src/openGL.cpp src/glad.c src/model.cpp  #src/shader.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = openGL

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.o
