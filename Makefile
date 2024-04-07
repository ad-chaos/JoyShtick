all: | build
	cmake --build build

build:
	cmake -B build -G Ninja

flash:
	picotool load -f build/jreader.uf2

clean:
	rm -rf build
