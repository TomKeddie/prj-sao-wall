AVR_LIBC = /usr/lib/avr
CFLAGS += -I $(AVR_LIBC)/include -mmcu=atmega32u4 -D__AVR_ATmega32U4__ -Os
LDFLAGS += -L $(AVR_LIBC)/lib -L $(AVR_LIBC)/lib/avr5 -mmcu=atmega32u4 -Wl,-Map,sao-wall.map
CC=avr-gcc

all : sao-wall.hex sao-wall.s sao-wall.bin

sao-wall.elf : sao-wall.o

clean :
	rm -f sao-wall.elf sao-wall.o

%.hex : %.elf
	avr-objcopy -j .text -j .data -O ihex $< $@

%.bin : %.elf
	avr-objcopy -j .text -j .data -O binary $< $@

%.elf : %.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LOADLIBES) $(LDLIBS)
	avr-size -C $<

# avrdude: safemode: Fuses OK (E:CB, H:D8, L:FF)
prog : sao-wall.hex
	avrdude -c dragon_isp -p m32u4 -e -Uflash:w:$<:i -U hfuse:w:0xd8:m -U lfuse:w:0xFF:m -U efuse:w:0xcb:m

%.s : %.c
	$(CC) -S -fverbose-asm -g $(CFLAGS) $< -o $@
