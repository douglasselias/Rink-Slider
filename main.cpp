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
#include "src/pause_menu.cpp"
#include "src/ai_menu.cpp"
#include "src/bgm.cpp"
#include "src/sfx.cpp"
#include "src/players.cpp"
#include "src/bg.cpp"

enum Game_State {
  main_menu,
  ai_menu,
  playing,
  paused,
  game_over,
};

// MARK: MAIN
s32 main() {
  init_screen();
  init_font_title();
  init_font();
  init_bgm();
  init_sfx();
  init_board();
  init_players();
  init_menu_options();
  init_ai_menu();
  init_bg();

  Game_State game_state = main_menu;
  u8 number_of_players_playing = 4;

  f32 total_transition_timeout = 0.3f;
  f32 transition_timeout = 0;

  // MARK: LOOP
  while (!WindowShouldClose()) {
    f32 dt = GetFrameTime();

    if(winner == -1) UpdateMusicStream(bgm);

    if(IsKeyPressed(KEY_SPACE)) {
      switch(game_state) {
        case main_menu: {
          /// @todo: merge with code below
          PlaySound(select_sfx);
          if(menu_option_index == 0) {
            menu_option_2_position = menu_option_2_position_default;
            /// @todo: is hovering necessary here?
            hovering_2_players = false;
            scale_2 = 1;
            number_of_players_playing = 2;
            game_state = ai_menu;
            transition_timeout = total_transition_timeout;
          } else if(menu_option_index == 1) {
            menu_option_3_position = menu_option_3_position_default;
            hovering_3_players = false;
            scale_3 = 1;
            number_of_players_playing = 3;
            game_state = ai_menu;
            transition_timeout = total_transition_timeout;
          } else if(menu_option_index == 2) {
            menu_option_4_position = menu_option_4_position_default;
            hovering_4_players = false;
            scale_4 = 1;
            number_of_players_playing = 4;
            game_state = ai_menu;
            transition_timeout = total_transition_timeout;
          }
          break;
        }
        case ai_menu: {
          game_state = playing;
          PlaySound(select_sfx);
          break;
        }
        case playing: {
          if(FloatEquals(platform_final_position.x, -1)
          && FloatEquals(platform_final_position.y, -1)) {
            if(selected_platform != -1) {
              selected_platform = -1;
              PlaySound(select_platform_sfx);
            } else {
              s8 index = 0;
              for(auto platform_position : platform_positions) {
                if(cast_u8(player_positions[selected_player].x) == cast_u8(platform_position.x)
                && cast_u8(player_positions[selected_player].y) == cast_u8(platform_position.y)) {
                  selected_platform = index;
                  PlaySound(select_platform_sfx);
                  break;
                }
                index++;
              }
            }
          }
          break;
        }
        case paused: {
          switch(pause_menu_option_index) {
            case 0: game_state = playing; PlaySound(select_sfx); break;
            case 1: {
              PlaySound(select_sfx);
              pause_menu_option_index = 0;
              /// @todo: extract this into a fn, same for the code below
              game_state = main_menu;
              winner = -1;
              selected_player = 0;
              reset_platform_positions();
              reset_player_positions();
              clear_board_positions();
              update_board_positions();
              update_board_players(player_positions, number_of_players_playing);
              break;
            }
            case 2: CloseWindow(); break;
          }
          break;
        }
        case game_over: {
          pause_menu_option_index = 0;
          game_state = main_menu;
          winner = -1;
          selected_player = 0;
          reset_platform_positions();
          reset_player_positions();
          clear_board_positions();
          update_board_positions();
          update_board_players(player_positions, number_of_players_playing);
          break;
        }
      }
    }

    if(IsKeyPressed(KEY_ENTER)) {
      if(game_state == playing)     game_state = paused;
      else if(game_state == paused) game_state = playing;
    }

    if(IsKeyPressed(KEY_A) && game_state == ai_menu) {
      if(-1 < number_of_ai_players - 1) {
        number_of_ai_players--;
        PlaySound(move_sfx);
      }
    }

    if(IsKeyPressed(KEY_D) && game_state == ai_menu) {
      if(number_of_ai_players + 1 < number_of_players_playing) {
        number_of_ai_players++;
        PlaySound(move_sfx);
      }
    }

    if(IsMouseButtonPressed(0) && game_state == ai_menu) {
      if(arrow_left_colliding) {
        if(-1 < number_of_ai_players - 1) {
          number_of_ai_players--;
          PlaySound(move_sfx);
        }
      }
    }

    if(IsMouseButtonPressed(0) && game_state == ai_menu) {
      if(arrow_right_colliding) {
        if(number_of_ai_players + 1 < number_of_players_playing) {
          number_of_ai_players++;
          PlaySound(move_sfx);
        }
      }
    }

    if(IsKeyPressed(KEY_W) && game_state == paused) {
      if(-1 < pause_menu_option_index - 1) {
        pause_menu_option_index--;
        PlaySound(move_sfx);
      }
    }

    if(IsKeyPressed(KEY_S) && game_state == paused) {
      if(pause_menu_option_index + 1 < 3) {
        pause_menu_option_index++;
        PlaySound(move_sfx);
      }
    }

    if(IsKeyPressed(KEY_A) && game_state == main_menu) {
      if(-1 < menu_option_index - 1) {
        menu_option_index--;
        PlaySound(move_sfx);
      }
    }

    if(IsKeyPressed(KEY_D) && game_state == main_menu) {
      if(menu_option_index + 1 < 3) {
        menu_option_index++;
        PlaySound(move_sfx);
      }
    }

    Vector2 mouse_position = GetMousePosition();
    check_mouse_collision_with_menu_options(mouse_position, dt);
    check_mouse_collision_with_arrows(mouse_position);

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
        game_state = ai_menu;
        transition_timeout = total_transition_timeout;
      } else if(hovering_3_players) {
        menu_option_3_position = menu_option_3_position_default;
        hovering_3_players = false;
        menu_option_index = 1;
        scale_3 = 1;
        number_of_players_playing = 3;
        game_state = ai_menu;
        transition_timeout = total_transition_timeout;
      } else if(hovering_4_players) {
        menu_option_4_position = menu_option_4_position_default;
        hovering_4_players = false;
        menu_option_index = 2;
        scale_4 = 1;
        number_of_players_playing = 4;
        game_state = ai_menu;
        transition_timeout = total_transition_timeout;
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
          PlaySound(move_player_sfx);
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
          PlaySound(move_player_sfx);
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
          PlaySound(move_player_sfx);
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
          PlaySound(move_player_sfx);
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

    // MARK: UPDATE
    transition_timeout -= dt;
    if(transition_timeout < 0) {
      transition_timeout = 0;
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
            PlaySound(win_sfx);
          }
          selected_player = (selected_player + 1) % number_of_players_playing;
          selected_platform = -1;
          platform_final_position = {-1,-1};

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          clear_board_positions();
          update_board_positions();
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
            PlaySound(win_sfx);
          }
          selected_player = (selected_player + 1) % number_of_players_playing;
          selected_platform = -1;
          platform_final_position = {-1,-1};

          /// @todo: simplify this, could set to false when select the platform and then set to true here, with the new position
          clear_board_positions();
          update_board_positions();
          update_board_players(player_positions, number_of_players_playing);
        }

        animation_current_time += dt;
      }

    } else {
      animation_current_time = 0;
    }

    time += dt * bg_movement_speed;
    SetShaderValue(bg_color_fs, time_location_shader, &time, SHADER_UNIFORM_FLOAT);

    BeginDrawing();
    ClearBackground(BLACK);
    draw_bg();

    if(game_state == playing || game_state == game_over || game_state == paused) {
      // draw_text(TextFormat("Player turn %d", selected_player + 1), {5, 5}, GOLD);
      draw_texture(board_texture, board_position, board_texture_scale);
      for(auto platform_position : platform_positions) {
        Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_position);
        draw_texture(platform_texture, screen_position, board_texture_scale);
      }

      if(selected_platform != -1) {
        Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_positions[selected_platform]);
        f32 platform_frame_texture_scale = 1.5;
        draw_texture(platform_frame_texture, screen_position, platform_frame_texture_scale);
      }

      u8 player_index = 0;
      for(u8 i = 0; i < number_of_players_playing; i++) {
        Vector2 player_position_screen = convert_board_position_to_screen_position(board_top_left + player_offset, player_positions[i]);
        if(selected_player == i) BeginShaderMode(player_outline);
        DrawTextureV(player_textures[player_index++], player_position_screen, WHITE);
        if(selected_player == i) EndShaderMode();
      }
    } else if(game_state == main_menu || 0 < transition_timeout) {
      Vector2 text_size = measure_text_title(game_title);
      Vector2 text_position = {screen_center.x - text_size.x/2, text_size.y};
      draw_text_title(game_title, text_position + 6, BLACK);
      draw_text_title(game_title, text_position, GOLD);
      draw_menu_options(dt);
    } else if(game_state == ai_menu && FloatEquals(transition_timeout, 0)) {
      draw_ai_menu();
    }

    if(game_state == paused) {
      draw_pause_menu();
    }

    f32 y = 0;
    for(auto &row : board) {
      f32 x = 0;
      for(auto column : row) {
        Vector2 center = convert_board_position_to_screen_position(board_top_left, (Vector2){x,y} + 0.5f);
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

    if(winner != -1) {
      const char* text = TextFormat("Congrats player %d!", winner + 1);
      Vector2 text_size = measure_text(text);
      Vector2 text_position = {screen_center.x - text_size.x/2, screen_center.y};
      draw_text(text, text_position + 3, BLACK);
      Color player_color = ORANGE;
      switch(winner) {
        case 1: player_color = GREEN;  break;
        case 2: player_color = BLUE;   break;
        case 3: player_color = PURPLE; break;
      }
      draw_text(text, text_position, player_color);
    }

    EndDrawing();
  }

  /// @todo: not really necessary
  UnloadFont(font);
  UnloadFont(font_title);

  UnloadShader(player_outline);

  for(auto player_texture : player_textures) {
    UnloadTexture(player_texture);
  }

  UnloadTexture(board_texture);
  UnloadTexture(platform_texture);
  UnloadTexture(platform_frame_texture);
  UnloadTexture(arrow_left);
  UnloadTexture(arrow_right);

  UnloadSound(win_sfx);
  UnloadSound(move_sfx);
  UnloadSound(select_sfx);
  UnloadSound(sliding_sfx);
  UnloadSound(move_player_sfx);
  UnloadSound(select_platform_sfx);

  UnloadMusicStream(bgm);

  CloseWindow();
  return 0;
}