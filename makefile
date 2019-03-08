CC = avr-gcc
OC = avr-objcopy
SZ = avr-size

TARGET = atmega328p
F_CPU = 16000000

CC_FLAGS = -funsigned-char -funsigned-bitfields -O1 -fpack-struct -fshort-enums -g2 -Wall -std=gnu99 

# Final binary
BIN = NanoTest
# Put all auto generated stuff to this build dir.
BUILD_DIR = ./build

# List of all .c source files.
SRC = $(wildcard src/*.c) 
INC = -I src/

# All .o files go to build dir.
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
# gcc will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)

# make the hex file
$(BIN).hex: $(BIN).elf
	@echo $(MSG_OBJCPY_CMD) Extracting HEX file data from \"$<\"
	@$(OC) -O ihex -R .eeprom -R .fuse -R .lock -R .signature $< $@
	@$(SZ) --mcu=$(TARGET) --format=avr $<

# Actual target of the binary - depends on all .o files.
$(BIN).elf : $(OBJ)
# Create build directories - same structure as sources.
	@mkdir -p $(@D)
	@$(CC) $(CC_FLAGS) -mmcu=$(TARGET) $^ -o $@

# Include all .d files
-include $(DEP)

# Build target for every single object file.
# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(@D)
# The -MMD flags additionaly creates a .d file with
# the same name as the .o file.
	@$(CC) $(CC_FLAGS) $(INC) -mmcu=$(TARGET) -DF_CPU=$(F_CPU) -MMD -c $< -o $@

.PHONY : clean
clean :
	-@rm -rf $(BIN).elf $(BIN).hex $(OBJ) $(DEP) 2>/dev/null || true

.PHONY : install
install :
	avrdude -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:$(BIN).hex:i 
	
