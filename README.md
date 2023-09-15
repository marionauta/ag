# ag

Simple implementation of agent-based models in C, with graphic representation
using raylib. This is a project to learn more about C memory handling.

## Roadmap

- [x] Graphical representation of the agents' state.
- [x] Graphical toolbar to manage the simulation.
- [ ] Scripting language to define the simulation.

## Dependencies

- A C compiler
- GNU make
- [raylib 4.5](https://www.raylib.com)
- [raygui 4.0](https://github.com/raysan5/raygui/releases/tag/4.0)
    - Copy `src/raygui.h` to the `vendor` folder.
    - Aditionally, you can run `make vendor/raygui.h` and it will download
      and extract it automatically.

## Compile & run

```console
$ make
$ make run
```

## References

- [Agent-based model](https://en.wikipedia.org/wiki/Agent-based_model)
