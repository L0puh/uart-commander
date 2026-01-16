# RUN before building the library: git submodule update --init --recursive --remote 


OPENCM3_DIR := ./libopencm3
FREERTOS_DIR := ./freertos
BUILD_DIR := ./build
LIB_DIR := ./lib

FREERTOS_SRC_FILES = $(wildcard $(FREERTOS_DIR)/*.c)
FREERTOS_SRC_FILES += $(wildcard $(FREERTOS_DIR)/portable/GCC/ARM_CM3/*.c)
FREERTOS_SRC_FILES += $(wildcard $(FREERTOS_DIR)/portable/MemMang/*.c)
FREERTOS_OBJ_FILES = $(patsubst $(FREERTOS_DIR)/%.c,$(BUILD_DIR)/freertos/%.o,$(FREERTOS_SRC_FILES))

FREERTOS_LIB = $(LIB_DIR)/libfreertos_cm3.a
LIBOPENCM3_LIB = $(OPENCM3_DIR)/lib/libopencm3_stm32f1.a

TARGETS := stm32/f1
ARCH_FLAGS = -mthumb -mcpu=cortex-m3 
PREFIX ?= arm-none-eabi

CC := $(PREFIX)-gcc
AR := $(PREFIX)-ar
RANLIB := $(PREFIX)-ranlib

CFLAGS += -Os -std=c99
CFLAGS += $(ARCH_FLAGS)
CFLAGS += -Wextra -Wshadow -Wimplicit-function-declaration
CFLAGS += -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
CFLAGS += -fno-common -ffunction-sections -fdata-sections
CFLAGS += -MD -Wall -Wundef

INCLUDES =  -I$(OPENCM3_DIR)/include
INCLUDES += -I$(FREERTOS_DIR)/include
INCLUDES += -I$(FREERTOS_DIR)/portable/GCC/ARM_CM3
INCLUDES += -I.

DEFS = -DSTM32F1 -D__SOFTFP__

all: $(LIBOPENCM3_LIB) $(FREERTOS_LIB)

$(LIBOPENCM3_LIB):
	@echo "Building libopencm3..."
	$(Q)$(MAKE) -C $(OPENCM3_DIR) TARGETS=$(TARGETS)

$(FREERTOS_LIB): $(FREERTOS_OBJ_FILES) | $(LIB_DIR)
	@echo "Building FreeRTOS library..."
	$(Q)$(AR) rcs $@ $(FREERTOS_OBJ_FILES)
	$(Q)$(RANLIB) $@
	@echo "FreeRTOS library built: $@"

$(BUILD_DIR)/freertos/%.o: $(FREERTOS_DIR)/%.c 
	@echo "  CC    $<"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) $(CFLAGS) $(INCLUDES) $(DEFS) -c $< -o $@

$(LIB_DIR):
	@mkdir -p $(LIB_DIR)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

clean:
	$(Q)rm -rf $(BUILD_DIR)
	$(Q)rm -rf $(LIB_DIR)
	$(Q)rm -rf dist


.PHONY: all clean 
