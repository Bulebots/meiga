OBJS = $(filter-out main.o,$(patsubst %.c,%.o,$(wildcard *.c)))

BINARY = main

OPENCM3_DIR	= ../.opencm3/libopencm3
LDSCRIPT	= stm32f405xg.ld
LDLIBS		+= -lm
LDFLAGS		+= -L./
DEFS		+= -I./

# Target configuration
LIBNAME		= opencm3_stm32f4
DEFS		+= -DSTM32F4
FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS	= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)

# OpenOCD specific variables
OOCD		?= openocd
OOCD_INTERFACE	?= stlink-v2
OOCD_TARGET	?= stm32f4x

include ../.opencm3/libopencm3.rules.mk
