out/ag: main.c simulation.c actor_group.c actor.c vector2.c tools.c
	mkdir -p out && cc -std=c11 -Wall -Wextra -o out/ag main.c -lm
