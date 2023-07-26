CFLAGS := -std=c11 -pedantic -Wall -Wextra
COMMON_SOURCES := world.c agent_group.c agent.c vector2.c tools.c

.PHONY: all
all: out/ag out/agv

.PHONY: clean
clean:
	rm -rf out

out/ag: main.c $(COMMON_SOURCES)
	mkdir -p out && cc $(CFLAGS) -o $@ $< -lm

out/agv: main_visual.c $(COMMON_SOURCES)
	mkdir -p out && cc $(CFLAGS) -o $@ $< `pkg-config --libs --cflags raylib` -lm
