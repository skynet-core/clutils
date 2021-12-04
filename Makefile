.PHONY: config
config:
	cmake -B ./build -Dclutils_BUILD_TESTING=ON

.PHONY: build
build:
	cmake --build ./build


.PHONY: install
install:
	cmake --install ./build --prefix ./install

clean:
	rm -rf ./build ./install