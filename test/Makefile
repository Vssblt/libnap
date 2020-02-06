DEPS = $(shell find ./ -name "*.h")
SRC = $(shell find ./ -name "*.cpp")
OBJ = $(SRC:%.cpp=%.o) 
OUTF = ../output

#gen rule
build: test

test: test.cpp
	g++ -o $(OUTF)/test $(COMMON) $< $(LINKLIB) -g
	mv ./*.o $(OUTF)/

%.o: %.cpp $(DEPS)
	$(CXX) -c $<  -o $@ $(COMMON)

clean:
	rm -f *.o