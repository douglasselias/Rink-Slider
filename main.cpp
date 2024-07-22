#include "raylib.h"
#include "raymath.h"

// #define RAYGUI_IMPLEMENTATION
// #include "vendor/raygui.h"

#include "std/types.cpp"
#include "std/log.cpp"
#include "std/macros.cpp"
#include "std/vector_overload.cpp"

#include "src/screen.cpp"
#include "src/font.cpp"
// #include "src/editor.cpp"

void draw_texture(Texture2D texture, Vector2 position, f32 scale = 1) {
  Rectangle source = {0, 0, (f32)texture.width, (f32)texture.height};
  Rectangle dest = {position.x, position.y, (f32)texture.width * scale, (f32)texture.height * scale};
  Vector2 origin = {0,0};
  u8 rotation = 0;
  Color tint = WHITE;
  DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

Vector2 convert_screen_position_to_board_position_player(Vector2 board_top_left, Vector2 player_offset, u8 move_distance, Vector2 position) {
  // log("Top          ", board_top_left);
  // log("Player offset", player_offset);
  // log("Move dist    ", move_distance);
  // log("Position     ", position);
  // log("Result       ", position - board_top_left - player_offset);
  /// @note: I don't know why the y value returns the desired value plus one, so I subtract the y value by one
  /// @note: Now it's suddenly working?!
  return ((position - board_top_left - player_offset) / (Vector2){(f32)move_distance, (f32)move_distance}) - (Vector2){0, 0};
}

Vector2 convert_board_position_to_screen_position_player(Vector2 board_top_left, Vector2 player_offset, u8 move_distance, Vector2 position) {
  return board_top_left + player_offset + position * (Vector2){(f32)move_distance, (f32)move_distance};
}

/// -----------

Vector2 convert_screen_position_to_board_position(Vector2 board_top_left, u8 move_distance, Vector2 position) {
  /// @todo: test this function with platform
  /// @todo: remove this function
  return (position - board_top_left) / (Vector2){(f32)move_distance, (f32)move_distance};
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
  s8 selected_platform = -1;

  Texture2D platform_frame_texture = LoadTexture("gfx/platform_frame.png");

  struct BoardSquare {
    bool has_platform;
    bool has_player;
  };

  // BoardSquare board[7][7] = {};
  bool board[7][7] = {};
  for(Vector2 platform_position : platform_positions) {
    board[(u8)platform_position.x][(u8)platform_position.y] = true;
  }

  Texture2D player_texture = LoadTexture("gfx/player_purple.png");
  Vector2 player_offset = (Vector2){(f32)player_texture.width / 4, 0};
  Vector2 player_position = board_top_left + player_offset;
  Vector2 player_position_board = {};

  bool is_debug = true;

  Vector2 platform_final_position = {};
  Vector2 player_final_position = {};

  while (!WindowShouldClose()) {
    // f32 dt = GetFrameTime();
    // Vector2 world_mouse_position = GetMousePosition();

    // if(IsKeyPressed(KEY_W)) platform_position.y -= move_distance;
    // if(IsKeyPressed(KEY_S)) platform_position.y += move_distance;
    // if(IsKeyPressed(KEY_A)) platform_position.x -= move_distance;
    // if(IsKeyPressed(KEY_D)) platform_position.x += move_distance;

    if(IsKeyPressed(KEY_W)) {
      if(selected_platform == -1) {
        // Vector2 player_board_position = convert_screen_position_to_board_position_player(board_top_left, player_offset, move_distance, player_position);
        s8 y = player_position_board.y - 1;
        if(y >= 0 && board[(u8)player_position_board.x][y])
          player_position_board.y -= 1;
      }
    }
    if(IsKeyPressed(KEY_S)) {
      if(selected_platform == -1) {
        // Vector2 player_board_position = convert_screen_position_to_board_position_player(board_top_left, player_offset, move_distance, player_position);
        s8 y = player_position_board.y + 1;
        if(y < 7 && board[(u8)player_position_board.x][y])
          player_position_board.y += 1;
      }
    }
    if(IsKeyPressed(KEY_A)) {
      if(selected_platform == -1) {
        // Vector2 player_board_position = convert_screen_position_to_board_position_player(board_top_left, player_offset, move_distance, player_position);
        s8 x = player_position_board.x - 1;
        if(x >= 0 && board[x][(u8)player_position_board.y])
          player_position_board.x -= 1;
      }
    }
    if(IsKeyPressed(KEY_D)) {
      if(selected_platform == -1) {
        // Vector2 player_board_position = convert_screen_position_to_board_position_player(board_top_left, player_offset, move_distance, player_position);
        s8 x = player_position_board.x + 1;
        if(x < 7 && board[x][(u8)player_position_board.y])
          player_position_board.x += 1;
      } else if(selected_platform != -1) {
        Vector2 platform_position = platform_positions[selected_platform];
        // Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, move_distance, platform_position);
        s8 x = player_position_board.x + 1;
        while(x < 7 && board[x][(u8)player_position_board.y] == false) {
          // platform_final_position.x += move_distance;
          x++;
        }
        // platform_positions[selected_platform].x = x - 1;
        platform_final_position.x = x - 1;
        player_final_position.x   = x - 1;
        log("PP", platform_final_position);
      }
    }

    if(IsKeyPressed(KEY_SPACE)) {
      if(selected_platform != -1) {
        selected_platform = -1;
      } else {
        // Vector2 player_board_position = convert_screen_position_to_board_position_player(board_top_left, player_offset, move_distance, player_position);
        u8 index = 0;
        for(Vector2 platform_position : platform_positions) {
          if((u8)player_position_board.x == (u8)platform_position.x
          && (u8)player_position_board.y == (u8)platform_position.y) {
            selected_platform = index;
            break;
          }
          index++;
        }
      }
    }

    if(selected_platform != -1 && !FloatEquals(platform_final_position.x, 0)) {
      platform_positions[selected_platform].x = Lerp(platform_positions[selected_platform].x, platform_final_position.x, 0.1);
      if(FloatEquals(platform_positions[selected_platform].x, platform_final_position.x)) {
        selected_platform = -1;
        platform_final_position = {};
      }

      player_position.x = Lerp(player_position.x, player_final_position.x, 0.1);
      if(FloatEquals(player_position.x, player_final_position.x)) {
        player_final_position = {};
      }
    }

    BeginDrawing();
    ClearBackground(BLACK);
    
    draw_texture(board_texture, board_position, board_texture_scale);

    for(Vector2 platform_position : platform_positions) {
      Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, move_distance, platform_position);
      draw_texture(platform_texture, screen_position, board_texture_scale);
    }

    if(selected_platform != -1) {
      Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, move_distance, platform_positions[selected_platform]);
      draw_texture(platform_frame_texture, screen_position, 1.5);
    }

    Vector2 player_position_screen = convert_board_position_to_screen_position_player(board_top_left, player_offset, move_distance, player_position_board);
    draw_texture(player_texture, player_position_screen);
    // draw_texture(player_texture, player_position);
    // DrawRectangleLinesEx((Rectangle){player_position.x, player_position.y, (f32)player_texture.width, (f32)player_texture.height}, 2, RED);

    if(is_debug) {
      // draw_text(TextFormat("(%.f, %.f)", world_mouse_position.x, world_mouse_position.y), (Vector2){world_mouse_position.x + 20, world_mouse_position.y});
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