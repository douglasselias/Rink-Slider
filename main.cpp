#include "raylib.h"
#include "raymath.h"

// #define RAYGUI_IMPLEMENTATION
// #include "vendor/raygui.h"

#include "std/types.cpp"
#include "std/macros.cpp"
#include "std/vector_overload.cpp"

#include "src/screen.cpp"
#include "src/font.cpp"
// #include "src/editor.cpp"

// #include "src/camera.cpp"

void log_v2(const char* name, Vector2 vector) {
  TraceLog(LOG_WARNING, TextFormat("%s: (%2f, %2f)", name, vector.x, vector.y));
}

void log_f32(const char* name, f32 f) {
  TraceLog(LOG_WARNING, TextFormat("%s: %2f", name, f));
}

void draw_texture(Texture2D texture, Vector2 position, f32 scale = 1) {
  Rectangle source = {0, 0, (f32)texture.width, (f32)texture.height};
  Rectangle dest = {position.x, position.y, (f32)texture.width * scale, (f32)texture.height * scale};
  Vector2 origin = {0,0};
  u8 rotation = 0;
  Color tint = WHITE;
  DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

Vector2 convert_screen_position_to_board_position(Vector2 position) {
  return {};
}

Vector2 convert_board_position_to_screen_position(Vector2 board_top_left, u8 move_distance, Vector2 position) {
  return board_top_left + position * (Vector2){(f32)move_distance, (f32)move_distance};
}

s32 main() {
  init_screen();
  init_font();

  Texture2D board_texture = LoadTexture("gfx/board.png");
  Vector2 board_position = {
    screen_center.x - board_texture.width,
    screen_center.y - board_texture.height,
  };
  u8 board_texture_scale = 2;
  Vector2 board_border_thickness = {5,5};
  Vector2 board_top_left = board_position + board_border_thickness * (Vector2){(f32)board_texture_scale, (f32)board_texture_scale};

  Texture2D platform_texture = LoadTexture("gfx/platform.png");
  Vector2 platform_position = board_top_left;
  u8 move_distance = platform_texture.width  * board_texture_scale;

  Vector2 platform_positions[12] = {
    /// @note: Top left
    {0,0},
    {1,0},
    {0,1},
    /// @note: Top right
    {6,0},
    {5,0},
    {6,1},
    /// @note: Bottom left
    {0,6},
    {0,5},
    {1,6},
    /// @note: Bottom right
    {6,6},
    {5,6},
    {6,5},
  };
  // u8 selected_platform = -1;

  // Vector2 board[7][7] = {};

  // for() {}

  Texture2D player_texture = LoadTexture("gfx/player_purple.png");
  Vector2 player_position = platform_position + (Vector2){(f32)player_texture.width / 4, 0};

  bool is_debug = true;

  while (!WindowShouldClose()) {
    // f32 dt = GetFrameTime();
    Vector2 world_mouse_position = GetMousePosition();

    // if(IsKeyPressed(KEY_W)) platform_position.y -= move_distance;
    // if(IsKeyPressed(KEY_S)) platform_position.y += move_distance;
    // if(IsKeyPressed(KEY_A)) platform_position.x -= move_distance;
    // if(IsKeyPressed(KEY_D)) platform_position.x += move_distance;

    if(IsKeyPressed(KEY_W)) player_position.y -= move_distance;
    if(IsKeyPressed(KEY_S)) player_position.y += move_distance;
    if(IsKeyPressed(KEY_A)) player_position.x -= move_distance;
    if(IsKeyPressed(KEY_D)) player_position.x += move_distance;

    // update_camera();

    BeginDrawing();
    // BeginMode2D(camera);
    ClearBackground(BLACK);
    
    draw_texture(board_texture, board_position, board_texture_scale);
    // draw_texture(platform_texture, platform_position, board_texture_scale);

    for(Vector2 platform_position : platform_positions) {
      Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, move_distance, platform_position);
      draw_texture(platform_texture, screen_position, board_texture_scale);
    }

    draw_texture(player_texture, player_position);
    // DrawRectangleLinesEx((Rectangle){player_position.x, player_position.y, (f32)player_texture.width, (f32)player_texture.height}, 2, RED);

    // EndMode2D();

    if(is_debug) {
      // DrawText(TextFormat("(%.f, %.f)", world_mouse_position.x, world_mouse_position.y), world_mouse_position.x + 20, world_mouse_position.y, 24, WHITE);
      // draw_text(TextFormat("(%.f, %.f)", world_mouse_position.x, world_mouse_position.y), (Vector2){world_mouse_position.x + 20, world_mouse_position.y});
      // DrawLineV((Vector2){0, screen_center.y / 2}, (Vector2){board_texture.width, screen_center.y / 2}, RED);
    }
    EndDrawing();
  }

  /// @todo: not really necessary
  unload_font();
  UnloadTexture(platform_texture);
  UnloadTexture(board_texture);

  CloseWindow();
  return 0;
}