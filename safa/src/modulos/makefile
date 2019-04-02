CC=gcc

BUILD_DIR=_build
SOURCES=safaConfig.c cpu.c  planificadorLP.c adminColasPlanificacion.c dam.c conexionCPUDAM.c consolaGDTRequest.c consola.c planificadorCP.c
HEADERS=safaConfig.h cpu.h  planificadorLP.h adminColasPlanificacion.h dam.h conexionCPUDAM.h consolaGDTReques.h consola.h planificadorCP.h
OBJS=$(SOURCES:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/libsafa.so

$(BUILD_DIR)/libsafa.so: create-dirs $(OBJS)
	$(CC) -shared -o $(BUILD_DIR)/libsafa.so $(OBJS) 

$(BUILD_DIR)/%.o: %.c
	$(CC) -c -fPIC -o "$@" "$<"

create-dirs:
	mkdir --parents $(BUILD_DIR)

debug: CC += -DDEBUG -g
debug: all

clean:
	rm -rf $(BUILD_DIR)

install: all
	cp -vu $(BUILD_DIR)/libsafa.so /usr/lib
	mkdir --parents /usr/include/safa
	cp -vu $(HEADERS) /usr/include/safa

uninstall:
	rm -v -f /usr/lib/libsafa.so
	rm -v -rf /usr/include/safa

.PHONY: all create-dirs clean debug install uninstall



