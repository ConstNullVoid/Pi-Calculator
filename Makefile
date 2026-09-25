CXX = g++
CXXFLAGS = -std=c++26 -O3 -march=native -mtune=native -DNDEBUG
CPPFLAGS = -I/ucrt64/include
LDFLAGS = -L/ucrt64/lib
LDLIBS = -lflint -lgmp -lstdc++exp

TARGET = outputproject.exe
SRC = src/main.cpp src/pi.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC) $(LDFLAGS) $(LDLIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean