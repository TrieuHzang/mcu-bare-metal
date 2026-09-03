TARGET = gpio_input

CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
NM      = arm-none-eabi-nm
SIZE    = arm-none-eabi-size
OPENOCD = openocd

CPU_FLAGS = -mcpu=cortex-m3 -mthumb

CMSIS_CORE = ../../resources/CMSIS/Core/Include
CMSIS_DEVICE = ../../resources/CMSIS/Device/STM32F1xx/Include

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
         -fno-asynchronous-unwind-tables \
         -DSTM32F103xB \
         -I$(CMSIS_CORE) \
         -I$(CMSIS_DEVICE)

LDFLAGS = $(CPU_FLAGS) \
          -T stm32f103c8t6.ld \
          -nostdlib \
          -nostartfiles \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map

OBJECTS = startup.o main.o

ELF = $(TARGET).elf
BIN = $(TARGET).bin

OPENOCD_FLAGS = -f interface/stlink.cfg \
                -c "transport select hla_swd" \
                -c "adapter speed 1000" \
                -f target/stm32f1x.cfg

all: $(ELF) $(BIN)

startup.o: startup.c
	$(CC) $(CFLAGS) -c startup.c -o startup.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

$(ELF): $(OBJECTS) stm32f103c8t6.ld
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(ELF)
	$(SIZE) $(ELF)

$(BIN): $(ELF)
	$(OBJCOPY) -O binary $(ELF) $(BIN)

disasm: $(ELF)
	$(OBJDUMP) -d -S $(ELF) > $(TARGET).lst

symbols: $(ELF)
	$(NM) -n $(ELF)

sections: $(ELF)
	$(OBJDUMP) -h $(ELF)

flash: $(ELF)
	$(OPENOCD) $(OPENOCD_FLAGS) \
	-c "program $(ELF) verify reset exit"

clean:
	rm -f $(OBJECTS) \
	      $(ELF) \
	      $(BIN) \
	      $(TARGET).map \
	      $(TARGET).lst

.PHONY: all clean disasm symbols sections flash
