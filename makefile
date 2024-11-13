# Define variables
BUILD_DIR := build
CMAKE_DIR := $(BUILD_DIR)
DENO_SCRIPT := MultiLanguage/Javascript/Algogauge.mjs
DENO_OUTPUT := $(BUILD_DIR)/AlgogaugeJS


# Default target
all: cmake_build deno_build
install: python_install
    


# Target to configure and build with CMake
cmake_build:
	@echo "Running CMake..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && cmake --build .

# Target to compile Deno script
deno_build:
	@echo "Compiling Deno script..."
	mkdir -p $(BUILD_DIR)
	deno compile --output $(DENO_OUTPUT) $(DENO_SCRIPT)

python_install:
	@echo "Install AlgogaugePY to pipx"
	pipx install MultiLanguage/Python/ --force



# Clean up the build files
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all cmake_build deno_build clean
