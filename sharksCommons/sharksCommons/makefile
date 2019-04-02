CC=gcc

BUILD_DIR=_build
SOURCES=logs.c protocoloCpuDAM.c  protocoloMdjDAM.c protocolo_msj.c sockets.c mensaje.c protocoloSafaDAM.c protocoloFm9DAM.c  protocoloSafaCPU.c serealizacion.c parser.c 
HEADERS=logs.h protocoloCpuDAM.h  protocoloMdjDAM.h protocolo_msj.h sockets.h mensaje.h protocoloSafaDAM.h protocoloFm9DAM.h protocoloSafaCPU.h serealizacion.h parser.h protocolo.h      

OBJS=$(SOURCES:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/libsharksCommons.so
all: $(BUILD_DIR)/libshark.so

$(BUILD_DIR)/libsharksCommons.so: create-dirs $(OBJS)
	$(CC) -shared -o $(BUILD_DIR)/libsharksCommons.so $(OBJS)
	$(CC) -shared -o $(BUILD_DIR)/libshark.so $(OBJS)
$(BUILD_DIR)/%.o: %.c
	$(CC) -c -fPIC -o "$@" "$<"

create-dirs:
	mkdir --parents $(BUILD_DIR)

debug: CC += -DDEBUG -g
debug: all

clean:
	rm -rf $(BUILD_DIR)

install: all
	cp -vu $(BUILD_DIR)/libsharksCommons.so /usr/lib
		cp -vu $(BUILD_DIR)/libshark.so /usr/lib
	mkdir --parents /usr/include/shark
	cp -vu $(HEADERS) /usr/include/shark
		mkdir --parents /usr/include/sharksCommons
	cp -vu $(HEADERS) /usr/include/sharksCommons
uninstall:
	rm -v -f /usr/lib/libsharksCommons.so
	rm -v -rf /usr/include/libsharksCommons
	rm -v -f /usr/lib/libshark.so
	rm -v -rf /usr/include/libshark

.PHONY: all create-dirs clean debug install uninstall
