CC = avr-gcc
OC = avr-objcopy

TARGET = atmega328p
F_CPU = 16000000

# Declaration of variables
CC = avr-gcc
CC_FLAGS = -funsigned-char -funsigned-bitfields -O1 -fpack-struct -fshort-enums -g2 -Wall -c -std=gnu99 

# File names
EXEC = NanoTest
INC = -I .
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
 
# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC).elf -mmcu=$(TARGET) 
	$(OC) -R .eeprom -O ihex $(EXEC).elf $(EXEC).hex 
 
# To obtain object files
%.o: %.c
	$(CC) -c $(CC_FLAGS) $(INC) -mmcu=$(TARGET) -DF_CPU=$(F_CPU) $< -o $@ 

# To remove generated files
.PHONY: clean
clean:
	rm -f $(EXEC) $(OBJECTS)

.PHONY: dude
dude:
	avrdude -v -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:NanoTest.hex:i 
#	avrdude -v -patmega328p -carduino -P/dev/ttyUSB0 -b57600 -D -Uflash:w:FastBlink.ino.hex:i 
