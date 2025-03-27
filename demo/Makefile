# COMPILER
CC=/usr/bin/cc -fPIC

# INTERFACE (AXI or I2C or I2C_STM32)
INTERFACE = I2C

# BOARD (PYNQZ2 or ZCU104)
BOARD = ZCU104

# COMPILER FLAGS
ifeq ($(INTERFACE), AXI)
	LDFLAGS_DEMO = -lm -lpynq -lcma
	LDFLAGS_DEMO_BUILD = -lm -lpynq -lcma -L../hw-rot/build/ -lhwrot 
	LDFLAGS_DEMO_INSTALL = -lm -lpynq -lcma -lhwrot 
	CFLAGS_DEMO = 
else ifeq ($(INTERFACE), I2C)
	LDFLAGS_DEMO = -lm 
	LDFLAGS_DEMO_BUILD = -lm -L../shw-rot/build/ -lhwrot 
	CFLAGS_DEMO = 
else
	@echo "ERROR: SELECT INTERFACE TYPE!"
endif	

# SOURCE DIRECTORY
SRCDIR = ../hw-rot/src/

# SHA3
LIB_SHA3_HW_SOURCES = $(SRCDIR)sha3/sha3_shake_hw.c
LIB_SHA3_HW_HEADERS = $(SRCDIR)sha3/sha3_shake_hw.h
# SHA2
LIB_SHA2_HW_SOURCES = $(SRCDIR)sha2/sha2_hw.c
LIB_SHA2_HW_HEADERS = $(SRCDIR)sha2/sha2_hw.h
# EDDSA
LIB_EDDSA_HW_SOURCES = $(SRCDIR)eddsa/eddsa_hw.c 
LIB_EDDSA_HW_HEADERS = $(SRCDIR)eddsa/eddsa_hw.h 
# X25519
LIB_X25519_HW_SOURCES = $(SRCDIR)x25519/x25519_hw.c 
LIB_X25519_HW_HEADERS = $(SRCDIR)x25519/x25519_hw.h 
# TRNG
LIB_TRNG_HW_SOURCES = $(SRCDIR)trng/trng_hw.c 
LIB_TRNG_HW_HEADERS = $(SRCDIR)trng/trng_hw.h 
# AES
LIB_AES_HW_SOURCES = $(SRCDIR)aes/aes_hw.c 
LIB_AES_HW_HEADERS = $(SRCDIR)aes/aes_hw.h
# PUF
LIB_PUF_HW_SOURCES = $(SRCDIR)puf/puf_hw.c $(SRCDIR)puf/puf4.c 
LIB_PUF_HW_HEADERS = $(SRCDIR)puf/puf_hw.h $(SRCDIR)puf/puf4.h
# MAC
LIB_MAC_HW_SOURCES = $(SRCDIR)mac/mac_hw.c 
LIB_MAC_HW_HEADERS = $(SRCDIR)mac/mac_hw.h 
# COMMON
ifeq ($(INTERFACE), AXI) 
	LIB_COMMON_SOURCES = $(SRCDIR)common/intf.c $(SRCDIR)common/mmio.c $(SRCDIR)common/extra_func.c
	LIB_COMMON_HEADERS = $(SRCDIR)common/intf.h $(SRCDIR)common/mmio.h $(SRCDIR)common/extra_func.h $(SRCDIR)common/conf.h
else ifeq ($(INTERFACE), I2C)
	LIB_COMMON_SOURCES = $(SRCDIR)common/intf.c $(SRCDIR)common/i2c.c $(SRCDIR)common/extra_func.c
	LIB_COMMON_HEADERS = $(SRCDIR)common/intf.h $(SRCDIR)common/i2c.h $(SRCDIR)common/extra_func.h $(SRCDIR)common/conf.h
else
	@echo "ERROR: SELECT INTERFACE TYPE!"
endif	
# HW-ROT HEADER
LIB_HEADER = ../hw-rot.h

# LIBRARY SOURCES & HEADERS
LIB_SOURCES = $(LIB_COMMON_SOURCES) $(LIB_SHA3_HW_SOURCES) $(LIB_SHA2_HW_SOURCES) $(LIB_EDDSA_HW_SOURCES) $(LIB_X25519_HW_SOURCES) $(LIB_TRNG_HW_SOURCES) $(LIB_AES_HW_SOURCES) $(LIB_PUF_HW_SOURCES) $(LIB_MAC_HW_SOURCES)
LIB_HEADERS = $(LIB_COMMON_HEADERS) $(LIB_SHA3_HW_HEADERS) $(LIB_SHA2_HW_HEADERS) $(LIB_EDDSA_HW_HEADERS) $(LIB_X25519_HW_HEADERS) $(LIB_TRNG_HW_HEADERS) $(LIB_AES_HW_HEADERS) $(LIB_PUF_HW_HEADERS) $(LIB_MAC_HW_HEADERS) $(LIB_HEADER)

#DEMO
SRC_DEMO = src/

DEMO_SOURCES =	$(SRC_DEMO)demo_aes.c \
				$(SRC_DEMO)demo_eddsa.c \
				$(SRC_DEMO)demo_x25519.c \
				$(SRC_DEMO)demo_sha2.c \
				$(SRC_DEMO)demo_sha3.c \
				$(SRC_DEMO)demo_trng.c \
				$(SRC_DEMO)test_func.c

DEMO_SPEED_SOURCES =	$(SRC_DEMO)demo_aes_speed.c \
						$(SRC_DEMO)demo_sha2_speed.c \
						$(SRC_DEMO)demo_sha3_speed.c \
						$(SRC_DEMO)demo_eddsa_speed.c \
						$(SRC_DEMO)demo_x25519_speed.c \
						$(SRC_DEMO)demo_trng_speed.c \
						$(SRC_DEMO)test_func.c

DEMO_HEADERS = $(SRC_DEMO)demo.h $(SRC_DEMO)test_func.h

SOURCES = $(LIB_SOURCES) $(DEMO_SOURCES)
HEADERS = $(LIB_HEADERS) $(DEMO_HEADERS)

SOURCES_SPEED = $(LIB_SOURCES) $(DEMO_SPEED_SOURCES)
HEADERS_SPEED = $(LIB_HEADERS) $(DEMO_HEADERS)

SOURCES_ACC = $(LIB_SOURCES) $(DEMO_ACC_SOURCES)
HEADERS_ACC = $(LIB_HEADERS) $(DEMO_HEADERS)

# PROGRAMS
demo-all: $(SOURCES) demo.c $(HEADERS)
	$(CC) -o $@ $(CFLAGS_DEMO) $(SOURCES) demo.c $(LDFLAGS_DEMO) -D$(BOARD) -D$(INTERFACE)

demo-build: $(DEMO_SOURCES) demo.c $(DEMO_HEADERS)
	$(CC) -o $@ $(CFLAGS_DEMO_BUILD) $(DEMO_SOURCES) demo.c $(LDFLAGS_DEMO_BUILD) -D$(BOARD) -D$(INTERFACE)

demo-install: $(DEMO_SOURCES) demo.c $(DEMO_HEADERS)
	$(CC) -o $@ $(DEMO_SOURCES) demo.c $(LDFLAGS_DEMO_INSTALL) -D$(BOARD) -D$(INTERFACE) -DSEQUBIP_INST

demo-speed-all: $(SOURCES_SPEED) demo_speed.c $(HEADERS)
	$(CC) -o $@ $(CFLAGS_DEMO) $(SOURCES_SPEED) demo_speed.c $(LDFLAGS_DEMO) -D$(BOARD) -D$(INTERFACE)

demo-speed-build: $(DEMO_SPEED_SOURCES) demo_speed.c $(DEMO_HEADERS)
	$(CC) -o $@ $(CFLAGS_DEMO_BUILD) $(DEMO_SPEED_SOURCES) demo_speed.c $(LDFLAGS_DEMO_BUILD) -D$(BOARD) -D$(INTERFACE)

demo-speed-install: $(DEMO_SPEED_SOURCES) demo_speed.c $(DEMO_HEADERS)
	$(CC) -o $@ $(CFLAGS_DEMO_BUILD) $(DEMO_SPEED_SOURCES) demo_speed.c $(LDFLAGS_DEMO_INSTALL) -D$(BOARD) -D$(INTERFACE) -DSEQUBIP_INST

demo-fpl: $(SOURCES) demo_fpl.c $(HEADERS)
	$(CC) -o $@ $(CFLAGS_DEMO) $(SOURCES) demo_fpl.c $(LDFLAGS_DEMO) -D$(BOARD) -D$(INTERFACE)
	
.PHONY: all demo clean

# CLEAN
clean:
	-rm demo-*
