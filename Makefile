CC = zig cc
CFLAGS := -std=c11 -pedantic -Wall -Wextra
MAINS := $(wildcard src/main*.c)
COMMON_SOURCES := $(filter-out $(MAINS), $(wildcard src/*.c))
OBJECTS := $(wildcard out/*.o)

.PHONY: all
all: out/ag

.PHONY: run
run: out/ag
	./out/ag

.PHONY: clean
clean:
	rm -rf out

out/ag: src/main.c $(COMMON_SOURCES) vendor/raygui.o $(OBJECTS)
	mkdir -p out
	$(CC) $(CFLAGS) -I./vendor -o $@ $< vendor/raygui.o $(OBJECTS) `pkg-config --libs --cflags raylib` -lm

out/config.o: src/config.zig
	mkdir -p out
	zig build-obj $<
	mv config.o out/config.o
	rm config.o.o

out/tools.o: src/tools.zig
	mkdir -p out
	zig build-obj $<
	mv tools.o out/tools.o
	rm tools.o.o

vendor/raygui.o: vendor/raygui.h
	$(CC) -DRAYGUI_IMPLEMENTATION -x c -c -o $@ $< `pkg-config --cflags raylib`

vendor/raygui.h:
	mkdir -p out
	curl -Lo out/raygui.zip https://github.com/raysan5/raygui/archive/refs/tags/4.0.zip
	mkdir -p vendor
	unzip -p out/raygui.zip raygui-4.0/src/raygui.h > $@
