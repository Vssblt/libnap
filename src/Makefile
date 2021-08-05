DEPS = $(shell find ./ -name "*.h")
SRC = $(shell find ./ -name "*.cpp")
OBJ = $(SRC:%.cpp=%.o) 
OUTF = ../output

#gen rule
build:  libnap

libnap: $(OBJ)
	ar rcs $(OUTF)/libnap.a  $(OBJ)
	mv ./*.o $(OUTF)/

%.o: %.cpp $(DEPS)
	$(CXX) -c $<  -o $@ $(COMMON)

clean:
	rm -f *.o

install: libnap
	cp ../output/libnap.a /usr/local/lib/
	mkdir /usr/local/include/libnap -p
	cp ./*.h /usr/local/include/libnap/
