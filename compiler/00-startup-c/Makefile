TARGET = led_blink

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size

CPU_FLAGS = -mcpu=cortex-m3 -mthumb

CFLAGS = $(CPU_FLAGS) \
         -std=c11 \
         -O0 \
         -g3 \
         -Wall \
         -Wextra \
         -ffreestanding \
         -fno-builtin \
         -ffunction-sections \
         -fdata-sections \
         -fno-unwind-tables \
         -fno-asynchronous-unwind-tables

LDFLAGS = $(CPU_FLAGS) \
          -T stm32f103c8t6.ld \
          -nostdlib \
          -nostartfiles \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map

SOURCE = led_blink.c
OBJECT = led_blink.o

ELF = $(TARGET).elf
BIN = $(TARGET).bin

all: $(ELF) $(BIN)

$(OBJECT): $(SOURCE) led_blink.h
	$(CC) $(CFLAGS) -c $(SOURCE) -o $(OBJECT)

$(ELF): $(OBJECT) stm32f103c8t6.ld
	$(CC) $(OBJECT) $(LDFLAGS) -o $(ELF)
	$(SIZE) $(ELF)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BIN)

disasm: $(ELF)
	$(OBJDUMP) -d -S $(ELF) > $(TARGET).lst

clean:
	rm -f $(OBJECT) $(ELF) $(BIN) $(TARGET).map $(TARGET).lst
OPENOCD = openocd

OPENOCD_FLAGS = -f interface/stlink.cfg \
                -c "transport select hla_swd" \
                -f target/stm32f1x.cfg

flash: $(ELF)
	$(OPENOCD) $(OPENOCD_FLAGS) \
	-c "program $(ELF) verify reset exit"

connect:
	$(OPENOCD) $(OPENOCD_FLAGS) \
	-c "init; reset halt; shutdown"
.PHONY: all clean disasm flash connect
