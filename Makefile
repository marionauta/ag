CC = zig cc
CFLAGS := -std=c11 -pedantic -Wall -Wextra
MAINS := $(wildcard src/main*.c)
COMMON_SOURCES := $(filter-out $(MAINS), $(wildcard src/*.c))

.PHONY: all
all: out/ag

.PHONY: run
run: out/ag
	./out/ag

.PHONY: clean
clean:
	rm -rf out

out/ag: src/main.c $(COMMON_SOURCES) vendor/raygui.o out/config.o out/agent.o
	mkdir -p out
	$(CC) $(CFLAGS) -I./vendor -o $@ $< vendor/raygui.o out/config.o out/agent.o `pkg-config --libs --cflags raylib` -lm

out/config.o: src/config.zig
	mkdir -p out
	zig build-obj $<
	mv config.o out/config.o
	rm config.o.o

out/agent.o: src/agent.zig
	mkdir -p out
	zig build-obj $<
	mv agent.o out/agent.o
	rm agent.o.o

vendor/raygui.o: vendor/raygui.h
	$(CC) -DRAYGUI_IMPLEMENTATION -x c -c -o $@ $< `pkg-config --cflags raylib`

vendor/raygui.h:
	mkdir -p out
	curl -Lo out/raygui.zip https://github.com/raysan5/raygui/archive/refs/tags/4.0.zip
	mkdir -p vendor
	unzip -p out/raygui.zip raygui-4.0/src/raygui.h > $@
