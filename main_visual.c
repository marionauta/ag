#include <raylib.h>

#define __AG_USING_RAYLIB__

int main(void) {
  const int screen_width = 600;
  const int screen_height = screen_width;

  InitWindow(screen_width, screen_height, "Agents");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Window", 20, 20, 20, LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();
}
