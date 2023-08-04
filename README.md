# Agent-based model in C

Simple implementation of agent-based models in C, with graphic representation using raylib. This is a project to learn more about C memory handling.

## Dependencies

- A C compiler
- GNU make
- [raylib 4.5](https://www.raylib.com)
- [raygui 3.6](https://github.com/raysan5/raygui/releases/tag/3.6)
    - Copy `src/raygui.h` to `src/vendor` folder.
    - Aditionally, you can run `make src/vendor/raygui.h` and it will download
      and extract it automatically.

## Compile

```console
$ make
$ out/ag examples/test.lua
```
