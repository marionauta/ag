out/ag: main.c simulation.c world.c agent_group.c agent.c vector2.c tools.c
	mkdir -p out && cc -std=c11 -Wall -Wextra -o out/ag main.c -lm
