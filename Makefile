#TOOLCHAIN=~/toolchain/gcc-arm-none-eabi-4_9-2014q4/bin
#PREFIX=$(TOOLCHAIN)/arm-none-eabi-
PREFIX=arm-none-eabi-

ARCHFLAGS=-mthumb -mcpu=cortex-m0plus
COMMONFLAGS=-g3 -Og -Wall -Werror $(ARCHFLAGS)

CFLAGS=-I./includes -I./drivers $(COMMONFLAGS) -D CPU_MKL46Z256VLL4
LDFLAGS=$(COMMONFLAGS) --specs=nano.specs -Wl,--gc-sections,-Map,$(TARGET).map,-Tlink.ld
LDLIBS=

CC=$(PREFIX)gcc
LD=$(PREFIX)gcc
OBJCOPY=$(PREFIX)objcopy
SIZE=$(PREFIX)size
RM=rm -f

TARGETHELLO = hello_world
TARGETLED = led_blinky

SRC=$(wildcard *.c drivers/*.c)
OBJ=$(patsubst %.c, %.o, $(SRC))
FILTERHELLO=$(filter-out pin_mux_blink.c pin_mux_blink.h led_blinky.c,$(SRC) )
FILTERLED=$(filter-out pin_mux_hello.c pin_mux_hello.h hello_world.c,$(SRC) )
OBJHELLO = $(patsubst %.c, %.o, $(FILTERHELLO))
OBJLED = $(patsubst %.c, %.o, $(FILTERLED))


all: build size
build: elf
elf: $(TARGETHELLO).elf $(TARGETLED).elf

clean:
	$(RM) $(TARGETHELLO).elf $(TARGETHELLO).map $(OBJHELLO) $(TARGETLED).elf $(TARGETLED).map $(OBJLED) 

$(TARGETHELLO).elf: $(OBJHELLO)
	$(LD) $(LDFLAGS) $(OBJHELLO) $(LDLIBS) -o $@

$(TARGETLED).elf: $(OBJLED)
	$(LD) $(LDFLAGS) $(OBJLED) $(LDLIBS) -o $@

size:
	$(SIZE) $(TARGETHELLO).elf $(TARGETLED).elf

flash_hello: all
		openocd -f openocd.cfg -c "program $(TARGETHELLO).elf verify reset exit"

flash_led: all
		openocd -f openocd.cfg -c "program $(TARGETLED).elf verify reset exit"
