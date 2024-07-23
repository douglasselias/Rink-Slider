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

void draw_texture(Texture2D texture, Vector2 position, f32 scale = 1) {
  Rectangle source = {0, 0, (f32)texture.width, (f32)texture.height};
  Rectangle dest = {position.x, position.y, (f32)texture.width * scale, (f32)texture.height * scale};
  Vector2 origin = {0,0};
  u8 rotation = 0;
  Color tint = WHITE;
  DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

Vector2 convert_board_position_to_screen_position_player(Vector2 board_top_left, Vector2 player_offset, u8 move_distance, Vector2 position) {
  return board_top_left + player_offset + position * (Vector2){(f32)move_distance, (f32)move_distance};
}

Vector2 convert_board_position_to_screen_position(Vector2 board_top_left, u8 move_distance, Vector2 position) {
  return board_top_left + position * (Vector2){(f32)move_distance, (f32)move_distance};
}

f32 ease_in_out_quart(f32 x) {
  return x < 0.5 ? 8 * pow(x, 4) : 1 - pow(-2 * x + 2, 4) / 2;
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
  u8 board_border_thickness = 5;
  Vector2 board_top_left = board_position + board_border_thickness * board_texture_scale;

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

  bool board[7][7] = {};
  for(auto platform_position : platform_positions) {
    board[(u8)platform_position.x][(u8)platform_position.y] = true;
  }

  Texture2D player_textures[4] = {};
  const char* player_colors[4] = {"orange", "green", "blue", "purple"};
  u8 player_color_index = 0;
  for(auto &player_texture : player_textures) {
    player_texture = LoadTexture(TextFormat("gfx/player_%s.png", player_colors[player_color_index++]));
  }

  Vector2 player_offset = {(f32)player_textures[0].width / 4, 0};

  Vector2 player_positions[4] = {
    /// @note: Top left
    {0,0},
    /// @note: Top right
    {6,0},
    /// @note: Bottom left
    {0,6},
    /// @note: Bottom right
    {6,6},
  };
  u8 selected_player = 0;

  Vector2 platform_final_position = {};

  f32 animation_current_time = 0;
  f32 animation_duration = 1;

  while (!WindowShouldClose()) {
    f32 dt = GetFrameTime();

    if(IsKeyPressed(KEY_W)) {
      s8 y = player_positions[selected_player].y - 1;
      if(selected_platform == -1) {
        if(0 <= y && board[(u8)player_positions[selected_player].x][y])
          player_positions[selected_player].y--;
      } else {
        while(0 <= y && (board[(u8)player_positions[selected_player].x][y] == false)) {
          y--;
        }
        platform_final_position.y = y + 1;
      }
    }
    if(IsKeyPressed(KEY_S)) {
      s8 y = player_positions[selected_player].y + 1;
      if(selected_platform == -1) {
        if(y < 7 && board[(u8)player_positions[selected_player].x][y])
          player_positions[selected_player].y++;
      } else {
        while(y < 7 && board[(u8)player_positions[selected_player].x][y] == false) {
          y++;
        }
        platform_final_position.y = y - 1;
      }
    }
    if(IsKeyPressed(KEY_A)) {
      s8 x = player_positions[selected_player].x - 1;
      if(selected_platform == -1) {
        if(0 <= x && board[x][(u8)player_positions[selected_player].y])
          player_positions[selected_player].x--;
      } else {
        while(0 <= x && (board[x][(u8)player_positions[selected_player].y] == false)) {
          x--;
        }
        platform_final_position.x = x + 1;
      }
    }
    if(IsKeyPressed(KEY_D)) {
      s8 x = player_positions[selected_player].x + 1;
      if(selected_platform == -1) {
        if(x < 7 && board[x][(u8)player_positions[selected_player].y])
          player_positions[selected_player].x++;
      } else {
        while(x < 7 && (board[x][(u8)player_positions[selected_player].y] == false)) {
          x++;
        }
        platform_final_position.x = x - 1;
      }
    }

    if(IsKeyPressed(KEY_SPACE)) {
      if(selected_platform != -1) {
        selected_platform = -1;
      } else {
        u8 index = 0;
        for(auto platform_position : platform_positions) {
          if((u8)player_positions[selected_player].x == (u8)platform_position.x
          && (u8)player_positions[selected_player].y == (u8)platform_position.y) {
            selected_platform = index;
            break;
          }
          index++;
        }
      }
    }

    if(selected_platform != -1) {
      /// @todo: this is shared between players, need to separate!!!
      f32 animation_dt = ease_in_out_quart(animation_current_time / animation_duration);

      if(!FloatEquals(platform_final_position.x, 0)) {
        /// @todo: maybe some floating point rounding error?
        player_positions[selected_player].x = Lerp(player_positions[selected_player].x, platform_final_position.x, animation_dt);
        if(FloatEquals(player_positions[selected_player].x, platform_final_position.x)) {
          selected_player = (selected_player + 1) % 4;
        }

        platform_positions[selected_platform].x = Lerp(platform_positions[selected_platform].x, platform_final_position.x, animation_dt);
        if(FloatEquals(platform_positions[selected_platform].x, platform_final_position.x)) {
          selected_platform = -1;
          platform_final_position *= 0;
          for(auto &row : board)
            for(auto &column : row)
              column = false;

          for(auto platform_position : platform_positions) {
            board[(u8)platform_position.x][(u8)platform_position.y] = true;
          }
        }
      }

      if(!FloatEquals(platform_final_position.y, 0)) {
        player_positions[selected_player].y = Lerp(player_positions[selected_player].y, platform_final_position.y, animation_dt);
        if(FloatEquals(player_positions[selected_player].y, platform_final_position.y)) {
          selected_player = (selected_player + 1) % 4;
        }

        platform_positions[selected_platform].y = Lerp(platform_positions[selected_platform].y, platform_final_position.y, animation_dt);
        if(FloatEquals(platform_positions[selected_platform].y, platform_final_position.y)) {
          selected_platform = -1;
          platform_final_position *= 0;
          for(auto &row : board)
            for(auto &column : row)
              column = false;

          for(auto platform_position : platform_positions) {
            board[(u8)platform_position.x][(u8)platform_position.y] = true;
          }
        }
      }

      animation_current_time += dt;
    } else {
      animation_current_time = 0;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    
    draw_texture(board_texture, board_position, board_texture_scale);

    for(auto platform_position : platform_positions) {
      Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, move_distance, platform_position);
      draw_texture(platform_texture, screen_position, board_texture_scale);
    }

    if(selected_platform != -1) {
      Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, move_distance, platform_positions[selected_platform]);
      f32 platform_frame_texture_scale = 1.5;
      draw_texture(platform_frame_texture, screen_position, platform_frame_texture_scale);
    }

    u8 player_index = 0;
    for(auto player_position : player_positions) {
      Vector2 player_position_screen = convert_board_position_to_screen_position_player(board_top_left, player_offset, move_distance, player_position);
      draw_texture(player_textures[player_index++], player_position_screen);
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