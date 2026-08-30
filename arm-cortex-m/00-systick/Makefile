TARGET = systick

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE    = arm-none-eabi-size

CPUFLAGS = -mcpu=cortex-m3 -mthumb

CFLAGS = $(CPUFLAGS) \
         -Wall \
         -Wextra \
         -O0 \
         -g3 \
         -ffreestanding \
         -fdata-sections \
         -ffunction-sections \
         -fno-unwind-tables \
         -fno-asynchronous-unwind-tables

LDFLAGS = $(CPUFLAGS) \
          -T linker.ld \
          -nostdlib \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map \
          -Wl,--print-memory-usage

SRC = startup.c main.c
OBJ = $(SRC:.c=.o)

all: $(TARGET).elf $(TARGET).bin

$(TARGET).elf: $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@
	$(SIZE) $@

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

flash: $(TARGET).elf
	openocd \
		-f interface/stlink.cfg \
		-f target/stm32f1x.cfg \
		-c "program $(TARGET).elf verify reset exit"

clean:
	rm -f *.o *.elf *.bin *.map

.PHONY: all flash clean
