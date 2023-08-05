CFLAGS := -std=c11 -pedantic -Wall -Wextra
SOURCES := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c,out/%.o,$(SOURCES))

.PHONY: all
all: out/ag

.PHONY: clean
clean:
	rm -rf out

out/ag: $(OBJS)
	mkdir -p out && cc $(CFLAGS) -o $@ $^ `pkg-config --libs raylib` -lm

src/vendor/raygui.h:
	mkdir -p out
	curl -Lo out/raygui.zip https://github.com/raysan5/raygui/archive/refs/tags/3.6.zip
	mkdir -p src/vendor
	unzip -p out/raygui.zip raygui-3.6/src/raygui.h > $@

out/main.o: src/main.c
	mkdir -p out && cc $(CFLAGS) -c -o $@ $< `pkg-config --cflags raylib`

out/%.o: src/%.c
	mkdir -p out && cc $(CFLAGS) -DAG_$(basename $(notdir $<))_IMPLEMENTATION -c -o $@ $<
