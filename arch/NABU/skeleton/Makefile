TOP=.

include Make.default

CFLAGS=-O2 -mz80 -I $(LIBCPM)/include -I include
LDFLAGS=-b -C0x100 -m $(BUILD_DIR)/$(APPNAME).map
CPPFLAGS=$(INCLUDES) -D$(ARCH)
LDLIBS=\
	$(LIBCPM)/lib/arch/NABU/libcpm.a \
	/opt/fcc/lib/z80/libz80.a

CRT0=$(LIBCPM)/lib/arch/NABU/crt0.o

SRC_S    = $(wildcard $(TOP)/asm/*.S)
OBJ_S    = $(patsubst $(TOP)/asm/%.S,$(BUILD_DIR)/%.o, $(SRC_S))

SRC_C    = $(wildcard $(TOP)/src/*.c)
OBJ_C    = $(patsubst $(TOP)/src/%.c,$(BUILD_DIR)/%.o, $(SRC_C))

OBJS= $(OBJ_S) $(OBJ_C)

.INTERMEDIATE: $(OBJS) $(BUILD_DIR)/$(APPNAME).bin
.PHONY: all clean world

all: $(BUILD_DIR)/$(APPNAME).com

$(BUILD_DIR)/%.s: $(TOP)/asm/%.S
	@mkdir -p $$(dirname $@)
	$(CPP) $(CPPFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(TOP)/src/%.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.s
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^

$(BUILD_DIR)/$(APPNAME).bin: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(CRT0) $^ $(LDLIBS)

$(BUILD_DIR)/$(APPNAME).com: $(BUILD_DIR)/$(APPNAME).bin
	dd if=$^ of=$@ skip=1 bs=256


clean:
	rm -vr $(BUILD_DIR)

world: clean all
