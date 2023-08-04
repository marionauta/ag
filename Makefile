CFLAGS := -std=c11 -pedantic -Wall -Wextra
MAINS := $(wildcard src/main*.c)
COMMON_SOURCES := $(filter-out $(MAINS), $(wildcard src/*.c))

.PHONY: all
all: out/ag

.PHONY: clean
clean:
	rm -rf out

out/ag: src/main.c $(COMMON_SOURCES)
	mkdir -p out
	cc $(CFLAGS) -o $@ $< `pkg-config --libs --cflags raylib` -lm

src/vendor/raygui.h:
	mkdir -p out
	curl -Lo out/raygui.zip https://github.com/raysan5/raygui/archive/refs/tags/3.6.zip
	mkdir -p src/vendor
	unzip -p out/raygui.zip raygui-3.6/src/raygui.h > $@
