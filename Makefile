CC = zig cc
CFLAGS := -std=c11 -pedantic -Wall -Wextra
MAINS := $(wildcard src/main*.zig)
COMMON_SOURCES := $(filter-out $(MAINS), $(wildcard src/*.zig))

.PHONY: all
all: out/bin/ag

.PHONY: run
run: out/bin/ag
	./out/bin/ag

.PHONY: clean
clean:
	rm -rf out

out/bin/ag: src/main.zig $(COMMON_SOURCES) vendor/raygui.o
	zig build --prefix out

vendor/raygui.o: vendor/raygui.h
	$(CC) -DRAYGUI_IMPLEMENTATION -x c -c -o $@ $< `pkg-config --cflags raylib`

vendor/raygui.h:
	mkdir -p out
	curl -Lo out/raygui.zip https://github.com/raysan5/raygui/archive/refs/tags/4.0.zip
	mkdir -p vendor
	unzip -p out/raygui.zip raygui-4.0/src/raygui.h > $@
