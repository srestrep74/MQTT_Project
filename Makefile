# Directorio donde se encuentran los archivos fuente
SRC_DIR = encoders messages base connack connect headers publish subscribe .

# Directorio donde se encuentran los archivos de cabecera
INC_DIR = .

# Objetos a compilar
OBJS = $(SRC_DIR)/main.o $(SRC_DIR)/client.o $(SRC_DIR)/server.o \
       $(SRC_DIR)/encoders/connect/connect.o $(SRC_DIR)/messages/base/message.o \
       $(SRC_DIR)/messages/connack/connack.o $(SRC_DIR)/messages/headers/fixed_header.o \
       $(SRC_DIR)/messages/publish/publish.o $(SRC_DIR)/messages/subscribe/subscribe.o

# Dependencias de cada objeto
$(SRC_DIR)/main.o: $(INC_DIR)/constants.h $(INC_DIR)/encoders/connect/connect.h $(INC_DIR)/messages/base/message.h $(SRC_DIR)/main.c
$(SRC_DIR)/client.o: $(INC_DIR)/constants.h $(SRC_DIR)/client.c
$(SRC_DIR)/server.o: $(INC_DIR)/constants.h $(SRC_DIR)/server.c
$(SRC_DIR)/encoders/connect/connect.o: $(INC_DIR)/connect.h $(SRC_DIR)/encoders/connect/connect.c
$(SRC_DIR)/messages/base/message.o: $(INC_DIR)/message.h $(SRC_DIR)/messages/base/message.c
$(SRC_DIR)/messages/connack/connack.o: $(INC_DIR)/connack.h $(SRC_DIR)/messages/connack/connack.c
$(SRC_DIR)/messages/headers/fixed_header.o: $(INC_DIR)/fixed_header.h $(INC_DIR)/payload.h $(INC_DIR)/variable_header.h $(SRC_DIR)/messages/headers/fixed_header.c
$(SRC_DIR)/messages/publish/publish.o: $(INC_DIR)/publish.h $(SRC_DIR)/messages/publish/publish.c
$(SRC_DIR)/messages/subscribe/subscribe.o: $(INC_DIR)/subscribe.h $(SRC_DIR)/messages/subscribe/subscribe.c

# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -pedantic

# Nombre del programa final
PROGRAM = mqtt-client

# Regla para compilar el programa
all: $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para limpiar los archivos temporales
clean:
	rm -f $(OBJS) $(PROGRAM)
