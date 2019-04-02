CC=gcc

BUILD_DIR=_build
SOURCES=conect.c pruebas.c sentencias.c cpu_2.c configuracionCPU.c fm9.c
HEADERS=conect.h pruebas.h sentencias.h cpu_2.h configuracionCPU.h  fm9.h
OBJS=$(SOURCES:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/libcpu.so

$(BUILD_DIR)/libcpu.so: create-dirs $(OBJS)
	$(CC) -shared -o $(BUILD_DIR)/libcpu.so $(OBJS) 

$(BUILD_DIR)/%.o: %.c
	$(CC) -c -fPIC -o "$@" "$<"

create-dirs:
	mkdir --parents $(BUILD_DIR)

debug: CC += -DDEBUG -g
debug: all

clean:
	rm -rf $(BUILD_DIR)

install: all
	cp -vu $(BUILD_DIR)/libcpu.so /usr/lib
	mkdir --parents /usr/include/cpu
	cp -vu $(HEADERS) /usr/include/cpu

uninstall:
	rm -v -f /usr/lib/libcpu.so
	rm -v -rf /usr/include/cpu

.PHONY: all create-dirs clean debug install uninstall
