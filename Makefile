CXX := g++

all: main
clean:
	rm -f main
	rm -f *.o

main: main.cpp Simulation.o
	$(CXX) $? -o main 
	
%.o: %.cpp
	$(CXX) -o $@ -c $<