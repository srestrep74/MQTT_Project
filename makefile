# Nombre del ejecutable
TARGET = main

# Compilador
CC = gcc

# Opciones de compilación
CFLAGS = -Wall -Wextra -std=c99

# Directorios de búsqueda de archivos fuente
SRC_DIRS = src/actions src/server/topic_tree

# Archivos fuente
SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
SRCS += main.c

# Archivos objeto generados a partir de los archivos fuente
OBJS := $(SRCS:.c=.o)

# Regla por defecto: compilar todos los archivos objeto y linkearlos
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Regla para compilar cada archivo fuente individualmente
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJS) $(TARGET)
