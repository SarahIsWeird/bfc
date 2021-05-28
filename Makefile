TARGET := bfc
CC ?= gcc
CFLAGS := -std=c89

SRC := $(shell find src -name "*.c")
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

.PHONY: clean

all: build

$(OBJ): build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

build: $(OBJ)
	$(CC) -o $(TARGET) $(OBJ)

clean:
	rm -rf build/ $(TARGET)