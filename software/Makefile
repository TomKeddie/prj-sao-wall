AVR_LIBC = /usr/lib/avr
CFLAGS += -I $(AVR_LIBC)/include -mmcu=avr5 -D__AVR_ATmega32U4__
LDFLAGS += -L $(AVR_LIBC)/lib -L $(AVR_LIBC)/lib/avr5
CC=avr-gcc

all : sao-wall.hex

sao-wall.bin : sao-wall.o

clean :
	rm -f sao-wall.bin sao-wall.o

%.hex : %.bin
	avr-size -C $<
	avr-objcopy -j .text -j .data -O ihex $< $@

%.bin : %.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LOADLIBES) $(LDLIBS)

# avrdude: safemode: Fuses OK (E:CB, H:D8, L:FF)
prog : sao-wall.hex
	avrdude -c dragon_isp -p m32u4 -e -Uflash:w:$<:i
