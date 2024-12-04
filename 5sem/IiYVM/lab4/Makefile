BUILD_DIR=build

all:
	cmake -DBUILD_SHARED_LIBS=OFF -G "MinGW Makefiles" -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
