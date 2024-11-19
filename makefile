# Define variables
BUILD_DIR := build
CMAKE_DIR := $(BUILD_DIR)
DENO_SCRIPT := MultiLanguage/Javascript/AlgoGauge.mjs
DENO_OUTPUT := $(BUILD_DIR)/AlgoGaugeJS
MORPHEUS_TARGET:= x86_64-unknown-linux-gnu
MORPHEUS_DIR := $(BUILD_DIR)/morpheus


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
	@echo "Install AlgoGaugePY to pipx"
	pipx install MultiLanguage/Python/ --force

deno_build_linux:
	@echo "Compiling Deno script linux x86_64"
	mkdir -p $(MORPHEUS_DIR)
	deno compile --target $(MORPHEUS_TARGET) --output $(MORPHEUS_DIR)/$(DENO_OUTPUT) $(DENO_SCRIPT)


# Clean up the build files
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all cmake_build deno_build clean install python_install deno_build_linux
