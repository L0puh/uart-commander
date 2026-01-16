PROJECT = app
BUILD_DIR = build
BIN_DIR = bin

SRC = $(wildcard src/*c) 
OBJ = $(SRC:src/%.c=$(BUILD_DIR)/%.o)

LIBS_DIR = libs
OPENCM3_LIB = $(LIBS_DIR)/libopencm3/lib/libopencm3_stm32f1.a
FREERTOS_LIB = $(LIBS_DIR)/lib/libfreertos_cm3.a
LDSCRIPT = $(LIBS_DIR)/stm32f103c8t6.ld

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m3 -mthumb -Os -std=c99 -DSTM32F1
CFLAGS += -I$(LIBS_DIR)
CFLAGS += -I$(LIBS_DIR)/libopencm3/include
CFLAGS += -I$(LIBS_DIR)/freertos/include
CFLAGS += -I$(LIBS_DIR)/freertos/portable/GCC/ARM_CM3
CFLAGS += -I. -Iinclude
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wextra

LDFLAGS = -T$(LDSCRIPT) -mcpu=cortex-m3 -mthumb
LDFLAGS += -Wl,--gc-sections -nostartfiles --specs=nano.specs

all: $(BIN_DIR)/$(PROJECT).bin

$(BIN_DIR)/$(PROJECT).bin: $(BIN_DIR)/$(PROJECT).elf
	$(OBJCOPY) -O binary $< $@
	@echo "Binary created: $@"

$(BIN_DIR)/$(PROJECT).elf: $(OBJ) | $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) \
		-L$(LIBS_DIR)/libopencm3/lib -L$(LIBS_DIR)/lib \
		-lfreertos_cm3 -lopencm3_stm32f1 \
		-lc -lgcc -lnosys
	$(SIZE) $@

$(BUILD_DIR)/%.o: src/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

$(BIN_DIR):
	mkdir -p $@

flash: $(BIN_DIR)/$(PROJECT).bin
	st-flash write $< 0x08000000

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all flash clean
