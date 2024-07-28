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

f32 ease_in_out_quart(f32 x) {
  return x < 0.5f ? 8 * (f32)pow(x, 4) : 1 - (f32)pow(-2 * x + 2, 4) / 2;
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
  u8 move_distance = cast_u8(platform_texture.width) * board_texture_scale;

  Vector2 platform_positions[12] = {
    // Top left
    {0,0},
    // {1,0},
    // {0,1},

    {4,0},
    {3,4},

    // {1,1},
    // Top right
    {6,0},
    {5,0},
    {6,1},
    // Bottom left
    {0,6},
    {0,5},
    {1,6},
    // Bottom right
    {6,6},
    {5,6},
    {6,5},
  };
  s8 selected_platform = -1;

  #define number_of_players 2
  Vector2 player_positions[number_of_players] = {
    // Top left
    {0,0},
    // Top right
    {6,0},
    // Bottom left
    // {0,6},
    // Bottom right
    // {6,6},
  };

  Texture2D platform_frame_texture = LoadTexture("gfx/platform_frame.png");
  update_board_positions(platform_positions);

  Texture2D player_textures[4] = {};
  const char* player_colors[4] = {"orange", "green", "blue", "purple"};
  u8 player_color_index = 0;
  for(auto &player_texture : player_textures) {
    player_texture = LoadTexture(TextFormat("gfx/player_%s.png", player_colors[player_color_index++]));
  }

  Vector2 player_offset = {(f32)player_textures[0].width / 4, 0};
  u8 selected_player = 0;

  Vector2 platform_final_position = {-1,-1};

  f32 animation_current_time = 0;
  f32 animation_duration = 1;

  s8 winner = -1;

  while (!WindowShouldClose()) {
    f32 dt = GetFrameTime();

    if(IsKeyPressed(KEY_W)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 y = cast_u8(player_positions[selected_player].y) - 1;
      u8 x = cast_u8(player_positions[selected_player].x);

      if(selected_platform == -1) {
        if(0 <= y && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].y--;
          update_board_players(player_positions);
        }
      } else {
        while(0 <= y && (board[y][x] == false)) {
          y--;
        }
        platform_final_position.y = y + 1;
      }
    }

    if(IsKeyPressed(KEY_S)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 y = cast_u8(player_positions[selected_player].y) + 1;
      u8 x = cast_u8(player_positions[selected_player].x);

      if(selected_platform == -1) {
        if(y < 7 && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].y++;
          update_board_players(player_positions);
        }
      } else {
        while(y < 7 && board[y][x] == false) {
          y++;
        }
        platform_final_position.y = y - 1;
      }
    }

    if(IsKeyPressed(KEY_A)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 x = cast_u8(player_positions[selected_player].x) - 1;
      u8 y = cast_u8(player_positions[selected_player].y);

      if(selected_platform == -1) {
        if(0 <= x && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].x--;
          update_board_players(player_positions);
        }
      } else {
        while(0 <= x && (board[y][x] == false)) {
          x--;
        }
        platform_final_position.x = x + 1;
      }
    }

    if(IsKeyPressed(KEY_D)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 x = cast_u8(player_positions[selected_player].x) + 1;
      u8 y = cast_u8(player_positions[selected_player].y);

      if(selected_platform == -1) {
        if(x < 7 && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].x++;
          update_board_players(player_positions);
        }
      } else {
        while(x < 7 && (board[y][x] == false)) {
          x++;
        }
        platform_final_position.x = x - 1;
      }
    }

    if(IsKeyPressed(KEY_SPACE)
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      if(selected_platform != -1) {
        selected_platform = -1;
      } else {
        s8 index = 0;
        for(auto platform_position : platform_positions) {
          if(cast_u8(player_positions[selected_player].x) == cast_u8(platform_position.x)
          && cast_u8(player_positions[selected_player].y) == cast_u8(platform_position.y)) {
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
        f32 x = Lerp(player_positions[selected_player].x, platform_final_position.x, animation_dt);
        player_positions[selected_player].x     = x;
        platform_positions[selected_platform].x = x;

        if(FloatEquals(player_positions[selected_player].x, platform_final_position.x)) {
          platform_positions[selected_platform].x = cast_u8(x);
          Vector2 platform_position = platform_positions[selected_platform];
          if(cast_u8(platform_position.x) == 3
          && cast_u8(platform_position.y) == 3) {
            winner = (s8)selected_player;
          }
          selected_player = (selected_player + 1) % number_of_players;
          selected_platform = -1;
          platform_final_position = {-1,-1};

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          clear_board_positions();
          update_board_positions(platform_positions);
          update_board_players(player_positions);
        }

        animation_current_time += dt;
      }

      if(!FloatEquals(platform_final_position.y, -1)) {
        f32 y = Lerp(player_positions[selected_player].y, platform_final_position.y, animation_dt);
        player_positions[selected_player].y     = y;
        platform_positions[selected_platform].y = y;

        if(FloatEquals(player_positions[selected_player].y, platform_final_position.y)) {
          platform_positions[selected_platform].y = cast_u8(y);
          Vector2 platform_position = platform_positions[selected_platform];
          if(cast_u8(platform_position.x) == 3
          && cast_u8(platform_position.y) == 3) {
            winner = (s8)selected_player;
          }
          selected_player = (selected_player + 1) % number_of_players;
          selected_platform = -1;
          platform_final_position = {-1,-1};

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          clear_board_positions();
          update_board_positions(platform_positions);
          update_board_players(player_positions);
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

      if(board[cast_u8(platform_position.y)][cast_u8(platform_position.x)]) {
        DrawCircleV(screen_position, 7, MAGENTA);
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

    u8 y = 0;
    for(auto &row : board) {
      u8 x = 0;
      for(auto column : row) {
        Vector2 center = convert_board_position_to_screen_position(board_top_left, (Vector2){(f32)0.5+(f32)1*x,(f32)0.5+y*1}, move_distance);
        if(column & PLATFORM) {
          DrawCircleV(center, 15, LIME);
        }
        if(column & PLAYER) {
          DrawCircleV(center, 10, GOLD);
        }
        if(column == false) {
          DrawCircleV(center, 15, VIOLET);
        }
        x++;
      }
      y++;
    }

    if(winner == -1) {
      const char* text = TextFormat("Congrats player %d!", (selected_player + 1));
      Vector2 text_size = measure_text(text);
      Vector2 text_position = {screen_center.x - text_size.x/2, screen_center.y};
      // draw_text(text, (Vector2){text_position.x - 5, text_position.y}, WHITE);
      // draw_text(text, (Vector2){text_position.x + 5, text_position.y}, WHITE);
      // draw_text(text, (Vector2){text_position.x, text_position.y - 5}, WHITE);
      draw_text(text, (Vector2){text_position.x + 5, text_position.y + 5}, WHITE);

      draw_text(text, text_position, BLACK);
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