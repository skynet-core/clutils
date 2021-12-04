all: config build install

.PHONY: config
config:
	cmake -B ./build -DCLUTILS_BUILD_TESTS:BOOL=ON

.PHONY: build
build:
	cmake --build ./build


.PHONY: install
install:
	cmake --install ./build --prefix ./install

clean:
	rm -rf ./build ./install