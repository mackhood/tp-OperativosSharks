CC=gcc

BUILD_DIR=_build
SOURCES=sockets.c  consola_linux.c estructuras_arbol.c fileSystem.c configurar.c bitmap.c
HEADERS=sockets.h  conexiones.h consola_linux.h estructuras_arbol.h fileSystem.h configurar.h bitmap.h
OBJS=$(SOURCES:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/libmdj.so

$(BUILD_DIR)/libmdj.so: create-dirs $(OBJS)
	$(CC) -shared -o $(BUILD_DIR)/libmdj.so $(OBJS) 

$(BUILD_DIR)/%.o: %.c
	$(CC) -c -fPIC -o "$@" "$<"

create-dirs:
	mkdir --parents $(BUILD_DIR)

debug: CC += -DDEBUG -g
debug: all

clean:
	rm -rf $(BUILD_DIR)

install: all
	cp -vu $(BUILD_DIR)/libmdj.so /usr/lib
	mkdir --parents /usr/include/mdj
	cp -vu $(HEADERS) /usr/include/mdj

uninstall:
	rm -v -f /usr/lib/libmdj.so
	rm -v -rf /usr/include/mdj

.PHONY: all create-dirs clean debug install uninstall
