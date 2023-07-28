CFLAGS := -std=c11 -pedantic -Wall -Wextra
MAINS := $(wildcard src/main*.c)
COMMON_SOURCES := $(filter-out $(MAINS), $(wildcard src/*.c))

.PHONY: all
all: out/ag out/agv

.PHONY: clean
clean:
	rm -rf out

out/ag: src/main.c $(COMMON_SOURCES)
	mkdir -p out && cc $(CFLAGS) -o $@ $< -lm

out/agv: src/main_visual.c $(COMMON_SOURCES)
	mkdir -p out && cc $(CFLAGS) -o $@ $< `pkg-config --libs --cflags raylib` -lm
