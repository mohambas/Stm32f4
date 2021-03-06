PROJ_NAME=main

SRCS = src/main.c \
       src/system_stm32f4xx.c \
       src/startup_stm32f4xx.s \
	   src/uart.c \
	   src/camera.c \
       src/picojpeg.c


CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

OBJDIR = build

CFLAGS  = -g -Wall -Wno-missing-braces -std=c99
CFLAGS += -mthumb -mcpu=cortex-m4
CFLAGS += -mfloat-abi=soft
CFLAGS += -specs=nosys.specs
# TODO: hard float was causing an exception; see what's up.
LDFLAGS = -Wl,-Map,$(OBJDIR)/$(PROJ_NAME).map -g -T./src/stm32f4_flash.ld

CFLAGS += -I./inc -I../../Libraries/CMSIS/ST/STM32F4xx/Include -I../../Libraries/CMSIS/Include

OBJS := $(SRCS:.c=.o)
OBJS := $(OBJS:.s=.o)
OBJS := $(addprefix $(OBJDIR)/,$(OBJS))


all: $(OBJDIR)/$(PROJ_NAME).elf $(OBJDIR)/$(PROJ_NAME).hex $(OBJDIR)/$(PROJ_NAME).bin

$(OBJDIR)/%.elf: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.hex: %.elf
	$(OBJCOPY) -O ihex $^ $@

%.bin: %.elf
	$(OBJCOPY) -O binary $^ $@

$(OBJDIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR)/%.o: %.s
	$(CC) -c $(CFLAGS) -o $@ $^

$(OBJDIR):
	mkdir -p $@

clean:
	rm -f $(OBJDIR)/$(PROJ_NAME).elf
	rm -f $(OBJDIR)/$(PROJ_NAME).hex
	rm -f $(OBJDIR)/$(PROJ_NAME).bin
	rm -f $(OBJDIR)/$(PROJ_NAME).map
	find $(OBJDIR) -type f -name '*.o' -print0 | xargs -0 -r rm


program: $(OBJDIR)/$(PROJ_NAME).elf
	openocd-0.6.1 -f program.cfg


# Dependdencies
$(OBJDIR)/$(PROJ_NAME).elf: $(OBJS) | $(OBJDIR)
