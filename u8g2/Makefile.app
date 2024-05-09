# Git hash of the library to use. Must match the hash in Makefile.
U8G2_VERSION_HASH := c4f9cd9f8717661c46be16bfbcb0017d785db3c1

# Base folder definitions
U8G2_LIB_DIR := $(TOCK_USERLAND_BASE_DIR)/u8g2
U8G2_SRC_DIR := $(U8G2_LIB_DIR)/u8g2-$(U8G2_VERSION_HASH)

override CFLAGS += -I$(U8G2_SRC_DIR)/csrc
override CFLAGS += -I$(U8G2_LIB_DIR)