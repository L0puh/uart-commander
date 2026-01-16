OPENCM3_DIR := ./libopencm3
FREERTOS_DIR := ./freertos
BUILD_DIR := ./build
LIB_DIR := ./lib

# FreeRTOS source files for the library
FREERTOS_SRC_FILES = $(FREERTOS_DIR)/tasks.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/list.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/queue.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/timers.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/event_groups.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/stream_buffer.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/croutine.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/portable/GCC/ARM_CM3/port.c
FREERTOS_SRC_FILES += $(FREERTOS_DIR)/portable/MemMang/heap_4.c

# FreeRTOS object files
FREERTOS_OBJ_FILES = $(patsubst $(FREERTOS_DIR)/%.c,$(BUILD_DIR)/freertos/%.o,$(FREERTOS_SRC_FILES))

# Target library names
FREERTOS_LIB = $(LIB_DIR)/libfreertos_cm3.a
LIBOPENCM3_LIB = $(OPENCM3_DIR)/lib/libopencm3_stm32f1.a

# Target architecture
TARGETS := stm32/f1
ARCH_FLAGS = -mthumb -mcpu=cortex-m3 
PREFIX ?= arm-none-eabi

CC := $(PREFIX)-gcc
AR := $(PREFIX)-ar
RANLIB := $(PREFIX)-ranlib

# C flags for FreeRTOS
CFLAGS += -Os -std=c99
CFLAGS += $(ARCH_FLAGS)
CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS += -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -MD -Wall -Wundef

# Include paths for FreeRTOS
INCLUDES = -I$(OPENCM3_DIR)/include
INCLUDES += -I$(FREERTOS_DIR)/include
INCLUDES += -I$(FREERTOS_DIR)/portable/GCC/ARM_CM3
INCLUDES += -I.

# Defines for FreeRTOS
DEFS = -DSTM32F1 -D__SOFTFP__

# Main target - build both libraries
all: $(LIBOPENCM3_LIB) $(FREERTOS_LIB)

# Build libopencm3 library
$(LIBOPENCM3_LIB):
	@echo "Building libopencm3..."
	$(Q)if [ ! "`ls -A $(OPENCM3_DIR)`" ] ; then \
		printf "#===========# ERROR #===========#\n"; \
		printf "  libopencm3 is not initialized\n"; \
		printf "  Please run:\n"; \
		printf "    $$ git submodule init\n"; \
		printf "    $$ git submodule update\n"; \
		printf "  before running make.\n"; \
		printf "#===========# ERROR #===========#\n"; \
		exit 1; \
	fi
	$(Q)$(MAKE) -C $(OPENCM3_DIR) TARGETS=$(TARGETS)

# Build FreeRTOS library
$(FREERTOS_LIB): $(FREERTOS_OBJ_FILES) | $(LIB_DIR)
	@echo "Building FreeRTOS library..."
	$(Q)$(AR) rcs $@ $(FREERTOS_OBJ_FILES)
	$(Q)$(RANLIB) $@
	@echo "FreeRTOS library built: $@"

# Check FreeRTOS sources before building
check-freertos:
	$(Q)if [ ! "`ls -A $(FREERTOS_DIR)`" ] ; then \
		printf "#===========# ERROR #===========#\n"; \
		printf "  FreeRTOS is not initialized\n"; \
		printf "  Please run:\n"; \
		printf "    $$ git submodule init\n"; \
		printf "    $$ git submodule update\n"; \
		printf "  before running make.\n"; \
		printf "#===========# ERROR #===========#\n"; \
		exit 1; \
	fi

# Rule to compile FreeRTOS source files
$(BUILD_DIR)/freertos/%.o: $(FREERTOS_DIR)/%.c | check-freertos
	@echo "  CC    $<"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

# Create directories
$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Install headers and libraries to system locations (optional)
install: all
	@echo "Installing libraries..."
	@mkdir -p /usr/local/include/freertos
	@cp -r $(FREERTOS_DIR)/include/* /usr/local/include/freertos/
	@cp -r $(FREERTOS_DIR)/portable/GCC/ARM_CM4F/*.h /usr/local/include/freertos/
	@cp $(FREERTOS_LIB) /usr/local/lib/
	@cp $(LIBOPENCM3_LIB) /usr/local/lib/
	@echo "Installation complete."

# Development headers package
headers:
	@mkdir -p dist/include/freertos
	@mkdir -p dist/lib
	@cp -r $(FREERTOS_DIR)/include/* dist/include/freertos/
	@cp -r $(FREERTOS_DIR)/portable/GCC/ARM_CM4F/*.h dist/include/freertos/
	@cp FreeRTOSConfig.h dist/include/freertos/ 2>/dev/null || true
	@echo "Headers packaged in dist/include/freertos/"

# Package both libraries for distribution
package: all headers
	@mkdir -p dist/lib
	@cp $(FREERTOS_LIB) dist/lib/
	@cp $(LIBOPENCM3_LIB) dist/lib/
	@echo "Libraries packaged in dist/lib/"
	@echo "To use in your project:"
	@echo "  - Add -I/path/to/dist/include to your compiler flags"
	@echo "  - Add -L/path/to/dist/lib -lfreertos_cm4f -lopencm3_stm32f1 to your linker flags"

# Clean build artifacts
clean:
	@echo "Cleaning build directories..."
	$(Q)rm -rf $(BUILD_DIR)
	$(Q)rm -rf $(LIB_DIR)
	$(Q)rm -rf dist
	@echo "Clean complete."

# Deep clean (also clean libopencm3)
distclean: clean
	@echo "Cleaning libopencm3..."
	$(Q)$(MAKE) -C $(OPENCM3_DIR) clean
	@echo "Distclean complete."

.PHONY: all check-freertos install headers package clean distclean
