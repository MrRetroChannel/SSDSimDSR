CC=gcc

BUILD_DIR=build
HASH_MAP_DIR=hash_map
SSD_SIM_DIR=ssd_sim
UTIL_DIR=util

INCLUDE=$(wildcard $(1)/*.h) $(wildcard $(1)/**/*.h)
SRC=$(wildcard $(1)/*.c) $(wildcard $(1)/**/*.c)

HASH_MAP_INCLUDE=$(call INCLUDE,$(HASH_MAP_DIR))
HASH_MAP_SRC=$(call SRC,$(HASH_MAP_DIR))
HASH_MAP_BUILD_DIR=$(BUILD_DIR)/$(HASH_MAP_DIR)

SSD_SIM_INCLUDE=$(call INCLUDE,$(SSD_SIM_DIR))
SSD_SIM_SRC=$(call SRC, $(SSD_SIM_DIR))

all: hash_map_lib
	$(CC) $(SSD_SIM_SRC) -I$(SSD_SIM_DIR)/include -I$(HASH_MAP_DIR)/include -I. -o ssdsim $(BUILD_DIR)/$(HASH_MAP_DIR)/hash_map.a
 
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(HASH_MAP_BUILD_DIR): $(BUILD_DIR)
	mkdir $(BUILD_DIR)/$(HASH_MAP_DIR)

hash_map_lib: $(HASH_MAP_INCLUDE) $(HASH_MAP_BUILD_DIR) $(BUILD_DIR)
	$(CC) -c $(HASH_MAP_SRC) -I. -I$(HASH_MAP_DIR) -o $(BUILD_DIR)/$(HASH_MAP_DIR)/hash_map.a