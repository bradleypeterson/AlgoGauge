# Define variables
BUILD_DIR := build
CMAKE_DIR := $(BUILD_DIR)
DENO_SCRIPT := src/MultiLanguage/Javascript/AlgoGauge.mjs
DENO_OUTPUT := $(BUILD_DIR)/bin/AlgoGaugeJS
MORPHEUS_TARGET:= x86_64-unknown-linux-gnu
MORPHEUS_DIR := $(BUILD_DIR)/morpheus

WINDOWS_BUILD_DIR := $(BUILD_DIR)/windows
WINDOWS_DENO_OUTPUT := $(WINDOWS_BUILD_DIR)/bin/AlgoGaugeJS

LINUX_x86_64_BUILD_DIR := $(BUILD_DIR)/linux_x86_64
LINUX_x86_64_DENO_OUTPUT := $(BUILD_DIR)/linux_x86_64/bin/AlgoGaugeJS

LINUX_AARCH64_BUILD_DIR := $(BUILD_DIR)/linux_aarch64

# Default target
all:  cmake_build deno_build 
install: python_install
build_windows:  cmake_build_windows deno_build_windows


# default
cmake_build:
	@echo "Running CMake..."
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake ..
	cd $(BUILD_DIR) && cmake --build .
deno_build:
	@echo "Compiling Deno script..."
	mkdir -p $(BUILD_DIR)
	deno compile --output $(DENO_OUTPUT) $(DENO_SCRIPT)


cmake_build_windows:
	@echo "Running CMake with windows"
	mkdir -p $(WINDOWS_BUILD_DIR)
	cd $(WINDOWS_BUILD_DIR) && cmake -DCMAKE_TOOLCHAIN_FILE=../../windows-toolchain.cmake ../..
	cd $(WINDOWS_BUILD_DIR) && cmake --build .

deno_build_windows:
	@echo "Compiling Deno script..."
	mkdir -p $(WINDOWS_BUILD_DIR)
	deno compile --target x86_64-pc-windows-msvc --output $(WINDOWS_DENO_OUTPUT) $(DENO_SCRIPT)




deno_build_linux_x86_64:
	@echo "Compiling Deno script linux x86_64"
	mkdir -p $(LINUX_x86_64_BUILD_DIR)
	deno compile --target x86_64-unknown-linux-gnu --output $(LINUX_x86_64_DENO_OUTPUT) $(DENO_SCRIPT)

deno_build_linux_aarch64:
	@echo "Compiling Deno script linux aarch64"
	deno compile --target aarch64-unknown-linux-gnu --output $(LINUX_x86_64_DENO_OUTPUT) $(DENO_SCRIPT)


deno_build_macos_aarch64:
	@echo "Compiling Deno script linux aarch64"
	deno compile --target aarch64-apple-darwin --output $(LINUX_x86_64_DENO_OUTPUT) $(DENO_SCRIPT)

deno_build_macos_x86_64:
	@echo "Compiling Deno script linux aarch64"
	deno compile --target x86_64-apple-darwin --output $(LINUX_x86_64_DENO_OUTPUT) $(DENO_SCRIPT)


python_install:
	@echo "Install AlgoGaugePY to pipx"
	pipx uninstall AlgogaugePY
	pipx install src/MultiLanguage/Python/

# Clean up the build files
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all cmake_build deno_build clean install python_install  build_windows deno_build_linux_x86_64 deno_build_linux_aarch64 deno_build_macos_aarch64 deno_build_macos_x86_64
