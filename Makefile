CC = zig cc
CFLAGS := -std=c11 -pedantic -Wall -Wextra
MAINS := $(wildcard src/main*.zig)
COMMON_SOURCES := $(filter-out $(MAINS), $(wildcard src/*.zig))

.PHONY: all
all: out/ag

.PHONY: run
run: out/ag
	./out/ag

.PHONY: clean
clean:
	rm -rf out

out/ag: src/main.zig $(COMMON_SOURCES) vendor/raygui.o
	mkdir -p out
	zig build-exe --name ag $< vendor/raygui.o `pkg-config --libs --cflags raylib` -lm
	mv ag out/ag

vendor/raygui.o: vendor/raygui.h
	$(CC) -DRAYGUI_IMPLEMENTATION -x c -c -o $@ $< `pkg-config --cflags raylib`

vendor/raygui.h:
	mkdir -p out
	curl -Lo out/raygui.zip https://github.com/raysan5/raygui/archive/refs/tags/4.0.zip
	mkdir -p vendor
	unzip -p out/raygui.zip raygui-4.0/src/raygui.h > $@
