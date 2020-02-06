COMMON = -std=c++11 -W -Wall -O0 $(Mco) -I/usr/local/include/cryptopp
LINKLIB = -lpthread -lcryptopp -L/usr/local/lib
export COMMON
export LINKLIB


all: checkdir test libnap
	./output/test

build: checkdir test libnap

test: checkdir
	make -C test

libnap: checkdir
	make -C src

clean:
	rm -rf ./output
	make -C src -s clean
	make -C test -s clean

checkdir: 
	mkdir -p ./output

fast: 
	make -j -s build

# install:
# 	make -C src install

# uninstall:
# 	make -C src install