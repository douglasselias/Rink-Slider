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
#include "src/menu.cpp"

enum Game_State {
  main_menu,
  playing,
  game_over,
};

// MARK: main
s32 main() {
  init_screen();
  init_font();
  init_font_title();

  // Music bgm = LoadMusicStream("sfx/8bit_bossa.mp3");
  Music bgm = LoadMusicStream("sfx/peachtea_somewhere_in_the_elevator.ogg");
  SetMasterVolume(0.4f);
  PlayMusicStream(bgm);

  Sound move_sfx = LoadSound("sfx/select.ogg");

  Sound select_sfx = LoadSound("sfx/confirmation.ogg");
  Sound sliding_sfx = LoadSound("sfx/sliding.ogg");
  SetSoundVolume(sliding_sfx, 1);

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

  init_menu_options();

  Game_State game_state = main_menu;

  Texture2D bg_texture = LoadTexture("gfx/bg.png");
  GenTextureMipmaps(&bg_texture);
  SetTextureWrap(bg_texture, TEXTURE_WRAP_REPEAT);
  SetTextureFilter(bg_texture, TEXTURE_FILTER_TRILINEAR);

  f32 time = 0;
  f32 bg_movement_speed = 0.1f;

  u8 number_of_players_playing = 2;

  reset_player_positions();

  Shader player_outline = LoadShader(0, "shaders/outline.fs");
  s32 texture_size_location = GetShaderLocation(player_outline, "textureSize");
  f32 texture_size_value[2] = { (f32)player_textures[0].width, (f32)player_textures[0].height };
  SetShaderValue(player_outline, texture_size_location, texture_size_value, SHADER_UNIFORM_VEC2);

  // MARK: game loop
  while (!WindowShouldClose()) {
    f32 dt = GetFrameTime();
    // UpdateMusicStream(bgm);

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
      game_state = main_menu;
    }

    if(IsKeyPressed(KEY_A) && game_state == main_menu) {
      menu_option_index = (s8)Clamp(--menu_option_index, 0, 2);
    }

    if(IsKeyPressed(KEY_D) && game_state == main_menu) {
      menu_option_index = (s8)Clamp(++menu_option_index, 0, 2);
    }

    Vector2 mouse_position = GetMousePosition();
    check_mouse_collision_with_menu_options(mouse_position, dt);

    if((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) && game_state == main_menu) {
      /// @todo: merge with code below
      PlaySound(select_sfx);
      if(menu_option_index == 0) {
        menu_option_2_position = menu_option_2_position_default;
        /// @todo: is hovering necessary here?
        hovering_2_players = false;
        // menu_option_index = 0;
        scale_2 = 1;
        number_of_players_playing = 2;
        game_state = playing;
      } else if(menu_option_index == 1) {
        menu_option_3_position = menu_option_3_position_default;
        hovering_3_players = false;
        // menu_option_index = 1;
        scale_3 = 1;
        number_of_players_playing = 3;
        game_state = playing;
      } else if(menu_option_index == 2) {
        menu_option_4_position = menu_option_4_position_default;
        hovering_4_players = false;
        // menu_option_index = 2;
        scale_4 = 1;
        number_of_players_playing = 4;
        game_state = playing;
      }
    }

    if(IsMouseButtonPressed(0)
    && game_state == main_menu
    && (hovering_2_players || hovering_3_players || hovering_4_players)) {
      PlaySound(select_sfx);
      if(hovering_2_players) {
        menu_option_2_position = menu_option_2_position_default;
        hovering_2_players = false;
        menu_option_index = 0;
        scale_2 = 1;
        number_of_players_playing = 2;
        game_state = playing;
      } else if(hovering_3_players) {
        menu_option_3_position = menu_option_3_position_default;
        hovering_3_players = false;
        menu_option_index = 1;
        scale_3 = 1;
        number_of_players_playing = 3;
        game_state = playing;
      } else if(hovering_4_players) {
        menu_option_4_position = menu_option_4_position_default;
        hovering_4_players = false;
        menu_option_index = 2;
        scale_4 = 1;
        number_of_players_playing = 4;
        game_state = playing;
      }
    }

    if(IsKeyPressed(KEY_W)
    && game_state == playing
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 y = cast_u8(player_positions[selected_player].y) - 1;
      u8 x = cast_u8(player_positions[selected_player].x);

      if(selected_platform == -1) {
        if(0 <= y && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].y--;
          update_board_players(player_positions, number_of_players_playing);
          PlaySound(move_sfx);
        }
      } else {
        if(0 <= y && board[y][x] == false) {
          while(0 <= y && (board[y][x] == false)) {
            y--;
          }
          platform_final_position.y = y + 1;
          PlaySound(sliding_sfx);
        }
      }
    }

    if(IsKeyPressed(KEY_S)
    && game_state == playing
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 y = cast_u8(player_positions[selected_player].y) + 1;
      u8 x = cast_u8(player_positions[selected_player].x);

      if(selected_platform == -1) {
        if(y < 7 && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].y++;
          update_board_players(player_positions, number_of_players_playing);
          PlaySound(move_sfx);
        }
      } else {
        if(y < 7 && board[y][x] == false) {
          while(y < 7 && board[y][x] == false) {
            y++;
          }
          platform_final_position.y = y - 1;
          PlaySound(sliding_sfx);
        }
      }
    }

    if(IsKeyPressed(KEY_A)
    && game_state == playing
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 x = cast_u8(player_positions[selected_player].x) - 1;
      u8 y = cast_u8(player_positions[selected_player].y);

      if(selected_platform == -1) {
        if(0 <= x && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].x--;
          update_board_players(player_positions, number_of_players_playing);
          PlaySound(move_sfx);
        }
      } else {
        if(0 <= x && board[y][x] == false) {
          while(0 <= x && (board[y][x] == false)) {
            x--;
          }
          platform_final_position.x = x + 1;
          PlaySound(sliding_sfx);
        }
      }
    }

    if(IsKeyPressed(KEY_D)
    && game_state == playing
    && FloatEquals(platform_final_position.x, -1)
    && FloatEquals(platform_final_position.y, -1)) {
      s8 x = cast_u8(player_positions[selected_player].x) + 1;
      u8 y = cast_u8(player_positions[selected_player].y);

      if(selected_platform == -1) {
        if(x < 7 && (board[y][x] & PLATFORM) && !(board[y][x] & PLAYER)) {
          player_positions[selected_player].x++;
          update_board_players(player_positions, number_of_players_playing);
          PlaySound(move_sfx);
        }
      } else {
        if(x < 7 && board[y][x] == false) {
          while(x < 7 && (board[y][x] == false)) {
            x++;
          }
          platform_final_position.x = x - 1;
          PlaySound(sliding_sfx);
        }
      }
    }

/// @todo: is selecting the platform after game init
    if(IsKeyPressed(KEY_SPACE)
    && game_state == playing
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
            game_state = game_over;
          }
          selected_player = (selected_player + 1) % number_of_players_playing;
          selected_platform = -1;
          platform_final_position = {-1,-1};

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          clear_board_positions();
          update_board_positions(platform_positions);
          update_board_players(player_positions, number_of_players_playing);
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
            game_state = game_over;
          }
          selected_player = (selected_player + 1) % number_of_players_playing;
          selected_platform = -1;
          platform_final_position = {-1,-1};

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          clear_board_positions();
          update_board_positions(platform_positions);
          update_board_players(player_positions, number_of_players_playing);
        }

        animation_current_time += dt;
      }

    } else {
      animation_current_time = 0;
    }

    // if(IsKeyPressed(KEY_A)) {previous_color();}
    // if(IsKeyPressed(KEY_D)) {next_color();}
    time += dt * bg_movement_speed;
    SetShaderValue(bg_color_fs, time_location_shader, &time, SHADER_UNIFORM_FLOAT);

    BeginDrawing();
    ClearBackground(BLACK);

    u64 bg_column_chunk = (u64)screen_width  / (u64)bg_texture.width;
    u64 bg_row_chunk    = (u64)screen_height / (u64)bg_texture.height;
    // log("column", bg_column_chunk);

    BeginShaderMode(bg_color_fs);
    for(u64 i = 0; i < bg_column_chunk; i++) {
      for(u64 j = 0; j <= bg_row_chunk; j++) {
        DrawTextureV(bg_texture, {(f32)i*bg_texture.width,(f32)j*bg_texture.height}, palette[palette_index]);
      }
    }
    EndShaderMode();
    // draw_text(TextFormat("Color %d", palette_index), {5, 5}, MAGENTA);

    if(game_state == playing) {
      // draw_text(TextFormat("Player turn %d", selected_player + 1), {5, 5}, GOLD);
      draw_texture(board_texture, board_position, board_texture_scale);
      for(auto platform_position : platform_positions) {
        Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_position, move_distance);
        draw_texture(platform_texture, screen_position, board_texture_scale);

        // if(board[cast_u8(platform_position.y)][cast_u8(platform_position.x)]) {
        //   DrawCircleV(screen_position, 7, MAGENTA);
        // }
      }

      if(selected_platform != -1) {
        Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_positions[selected_platform], move_distance);
        f32 platform_frame_texture_scale = 1.5;
        draw_texture(platform_frame_texture, screen_position, platform_frame_texture_scale);
      }

      u8 player_index = 0;
      for(u8 i = 0; i < number_of_players_playing; i++) {
        Vector2 player_position_screen = convert_board_position_to_screen_position(board_top_left + player_offset, player_positions[i], move_distance);
        if(selected_player == i) {
          BeginShaderMode(player_outline);
        }
        DrawTextureV(player_textures[player_index++], player_position_screen, WHITE);
        if(selected_player == i) {
          EndShaderMode();
        }
      }
    } else if(game_state == main_menu) {
      Vector2 text_size = measure_text_title(game_title);
      Vector2 text_position = {screen_center.x - text_size.x/2, text_size.y};
      draw_text_title(game_title, text_position + (Vector2){6, 6}, BLACK);
      draw_text_title(game_title, text_position, GOLD);
      draw_menu_options(dt);
    }


    // u8 y = 0;
    // for(auto &row : board) {
    //   u8 x = 0;
    //   for(auto column : row) {
    //     Vector2 center = convert_board_position_to_screen_position(board_top_left, (Vector2){(f32)0.5+(f32)1*x,(f32)0.5+y*1}, move_distance);
    //     if(column & PLATFORM) {
    //       DrawCircleV(center, 15, LIME);
    //     }
    //     if(column & PLAYER) {
    //       DrawCircleV(center, 10, GOLD);
    //     }
    //     if(column == false) {
    //       DrawCircleV(center, 15, VIOLET);
    //     }
    //     x++;
    //   }
    //   y++;
    // }

    if(winner != -1) {
      const char* text = TextFormat("Congrats player %d!", winner + 1);
      Vector2 text_size = measure_text(text);
      Vector2 text_position = {screen_center.x - text_size.x/2, screen_center.y};
      // draw_text(text, (Vector2){text_position.x - 5, text_position.y}, WHITE);
      // draw_text(text, (Vector2){text_position.x + 5, text_position.y}, WHITE);
      // draw_text(text, (Vector2){text_position.x, text_position.y - 5}, WHITE);
      draw_text(text, text_position + 3, BLACK);
      // draw_text(text, text_position - 5, BLACK);

      draw_text(text, text_position, ORANGE);
    }

    EndDrawing();
  }

  /// @todo: not really necessary
  unload_font();
  UnloadTexture(platform_texture);
  UnloadTexture(board_texture);
  UnloadMusicStream(bgm);

  CloseWindow();
  return 0;
}