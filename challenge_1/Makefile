all: build/main

build/main: src/main.cpp
	g++ -I ${mkEigenInc} -I include/ src/main.cpp -o build/main

run: build/main
	build/main

clean:
	rm build/*

.PHONY: all run clean