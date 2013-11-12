PROJECT=saRTOS
SOURCES=main.c
LIBRARY=
INCPATHS=
LIBPATHS=
MCU=atmega1280
LDFLAGS=-g -mmcu=$(MCU)
CFLAGS=-c -Wall -g -Os -mmcu=$(MCU)
CC=avr-gcc
OBJCOPY=avr-objcopy
 
# Automatic generation of some important lists
OBJECTS=$(SOURCES:.c=.o)
INCFLAGS=$(foreach TMP,$(INCPATHS),-I$(TMP))
LIBFLAGS=$(foreach TMP,$(LIBPATHS),-L$(TMP))
 
# Set up the output file names for the different output types
BINARY=$(PROJECT)
 
all: $(SOURCES) $(BINARY)
 
$(BINARY): $(OBJECTS)
	$(CC) $(LIBFLAGS) $(OBJECTS) $(LDFLAGS) -o $@.elf
	$(OBJCOPY) -j .text -j .data -O ihex $@.elf $@.hex
 
.c.o:
	$(CC) $(INCFLAGS) $(CFLAGS) -fPIC $< -o $@
 
distclean: clean
	rm -f $(BINARY)
 
clean:
	rm -f $(OBJECTS)

upload:
	avrdude  -DV -p $(MCU) -P /dev/ttyUSB0 -c arduino -b 57600 -U flash:w:$(PROJECT).hex:i