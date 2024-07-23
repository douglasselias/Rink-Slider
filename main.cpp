#include "assert.h"
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
#include "src/board.cpp"

void draw_texture(Texture2D texture, Vector2 position, f32 scale = 1) {
  Rectangle source = {0, 0, (f32)texture.width, (f32)texture.height};
  Rectangle dest = {position.x, position.y, (f32)texture.width * scale, (f32)texture.height * scale};
  Vector2 origin = {0,0};
  u8 rotation = 0;
  Color tint = WHITE;
  DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

Vector2 convert_board_position_to_screen_position(Vector2 offset, Vector2 position, u8 move_distance) {
  return offset + position * move_distance;
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

  #define number_of_players 2
  Vector2 player_positions[number_of_players] = {
    /// @note: Top left
    {0,0},
    /// @note: Top right
    {6,0},
    /// @note: Bottom left
    // {0,6},
    /// @note: Bottom right
    // {6,6},
  };
  u8 selected_player = 0;

  Vector2 platform_final_position = {-1,-1};

  f32 animation_current_time = 0;
  f32 animation_duration = 1;

  while (!WindowShouldClose()) {
    f32 dt = GetFrameTime();
    log("Final", platform_final_position.x);

    if(IsKeyPressed(KEY_W)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      log("Y", player_positions[selected_player].y);
      assert((player_positions[selected_player].y - 1 >= 0) && "Y is less than zero");
      s8 y = player_positions[selected_player].y - 1;
      if(selected_platform == -1) {
        if(0 <= y && board[(u8)player_positions[selected_player].x][y]) {
          player_positions[selected_player].y--;
        }
      } else {
        while(0 <= y && (board[(u8)player_positions[selected_player].x][y] == false)) {
          y--;
        }
        // log("W", 0);
        // log("Y", y);
        // log("Final Y", y + 1);
        platform_final_position.y = y + 1;
      }
    }

    if(IsKeyPressed(KEY_S)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 y = player_positions[selected_player].y + 1;
      if(selected_platform == -1) {
        if(y < 7 && board[(u8)player_positions[selected_player].x][y]) {
          player_positions[selected_player].y++;
          // log("S");
          // log("Y", y);
          // log("Final Y", y - 1);
        }
      } else {
        while(y < 7 && board[(u8)player_positions[selected_player].x][y] == false) {
          y++;
        }
        // log("S", 0);
        // log("Y", y);
        // log("Final Y", y + 1);
        platform_final_position.y = y - 1;
      }
    }

    if(IsKeyPressed(KEY_A)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 x = player_positions[selected_player].x - 1;
      if(selected_platform == -1) {
        if(0 <= x && board[x][(u8)player_positions[selected_player].y]) {
          player_positions[selected_player].x--;
          // log("A");
          // log("Y", y);
          // log("Final Y", y - 1);
        }
      } else {
        while(0 <= x && (board[x][(u8)player_positions[selected_player].y] == false)) {
          x--;
        }
        // log("A", 0);
        // log("X", x);
        // log("Final X", x + 1);
        platform_final_position.x = x + 1;
      }
    }

    if(IsKeyPressed(KEY_D)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 x = player_positions[selected_player].x + 1;
      if(selected_platform == -1) {
        if(x < 7 && board[x][(u8)player_positions[selected_player].y]) {
          player_positions[selected_player].x++;
          // log("D");
          // log("X", x);
          // log("Final x", x - 1);
        }
      } else {
        while(x < 7 && (board[x][(u8)player_positions[selected_player].y] == false)) {
          x++;
        }
        // log("D", 0);
        // log("X", x);
        // log("Final X", x + 1);
        platform_final_position.x = x - 1;
      }
    }

    if(IsKeyPressed(KEY_SPACE)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
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

      if(!FloatEquals(platform_final_position.x, -1)) {
        /// @todo: maybe some floating point rounding error?
        f32 x = Lerp(player_positions[selected_player].x, platform_final_position.x, animation_dt);
        player_positions[selected_player].x     = x;
        platform_positions[selected_platform].x = x;

        if(FloatEquals(player_positions[selected_player].x, platform_final_position.x)) {
          selected_player = (selected_player + 1) % number_of_players;
          selected_platform = -1;
          platform_final_position *= 0;
          platform_final_position += -1;

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          // for(auto &row : board)
          //   for(auto &column : row)
          //     column = false;
          clear_board_positions();
          update_board_positions(platform_positions);

          // for(auto platform_position : platform_positions) {
          //   board[(u8)platform_position.x][(u8)platform_position.y] = true;
          // }
        }

        animation_current_time += dt;
      }

      if(!FloatEquals(platform_final_position.y, -1)) {
        f32 y = Lerp(player_positions[selected_player].y, platform_final_position.y, animation_dt);
        player_positions[selected_player].y     = y;
        platform_positions[selected_platform].y = y;

        if(FloatEquals(player_positions[selected_player].y, platform_final_position.y)) {
          selected_player = (selected_player + 1) % 4;
          selected_platform = -1;
          platform_final_position *= 0;
          platform_final_position += -1;

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          // for(auto &row : board)
          //   for(auto &column : row)
          //     column = false;
          clear_board_positions();
          update_board_positions(platform_positions);

          // for(auto platform_position : platform_positions) {
          //   board[(u8)platform_position.x][(u8)platform_position.y] = true;
          // }
        }

        animation_current_time += dt;
      }

    } else {
      animation_current_time = 0;
    }

    BeginDrawing();
    ClearBackground(BLACK);
    
    draw_texture(board_texture, board_position, board_texture_scale);

    for(auto platform_position : platform_positions) {
      Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_position, move_distance);
      draw_texture(platform_texture, screen_position, board_texture_scale);

      if(board[(u8)platform_position.x][(u8)platform_position.y]) {
        DrawCircleV(screen_position, 5, MAGENTA);
      }
    }

    if(selected_platform != -1) {
      Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_positions[selected_platform], move_distance);
      f32 platform_frame_texture_scale = 1.5;
      draw_texture(platform_frame_texture, screen_position, platform_frame_texture_scale);
    }

    u8 player_index = 0;
    for(auto player_position : player_positions) {
      Vector2 player_position_screen = convert_board_position_to_screen_position(board_top_left + player_offset, player_position, move_distance);
      draw_texture(player_textures[player_index++], player_position_screen);
    }

    // for(auto row : board) {
    //   for(auto column : row) {
    //     if(column) {
    //       Vector2 center = convert_board_position_to_screen_position(board_top_left, move_distance, platform_positions[selected_platform]);
    //       DrawCircleV(center, 3, RED);
    //     }
    //   }
    // }

    EndDrawing();
  }

  /// @todo: not really necessary
  unload_font();
  UnloadTexture(platform_texture);
  UnloadTexture(board_texture);

  CloseWindow();
  return 0;
}