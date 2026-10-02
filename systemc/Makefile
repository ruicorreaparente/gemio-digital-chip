BUILD_DIR := build

.PHONY: all build run clean

all: build

build:
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && make -j$(nproc)

run: build
	@$(BUILD_DIR)/gpio_sim

clean:
	@rm -rf $(BUILD_DIR)
