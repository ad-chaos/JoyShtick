all: | build
	cmake --build build

build:
	cmake -B build -G Ninja

clean:
	rm -rf build
