#include "assert.h"
#include "raylib.h"
#include "raymath.h"

#include "std/types.cpp"
#include "std/log.cpp"
#include "std/macros.cpp"
#include "std/vector_overload.cpp"

#if BUNDLE
#include "bundle/bundle.cpp"
#endif

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

Game_State game_state = main_menu;
u8 number_of_players_playing = 4;
f32 total_transition_timeout = 0.3f;
f32 transition_timeout = 0;

void reset_game() {
  pause_menu_option_index = 0;
  game_state = main_menu;
  winner = -1;
  selected_player = 0;
  selected_platform = -1;
  reset_platform_positions();
  reset_player_positions();
  clear_board_positions();
  update_board_positions();
  update_board_players(player_positions, number_of_players_playing);
}

void select_option2() {
  menu_option_2_position = menu_option_2_position_default;
  hovering_2_players = false;
  menu_option_index = 0;
  scale_2 = 1;
  number_of_players_playing = 2;
  game_state = playing;
  transition_timeout = total_transition_timeout;
}

void select_option3() {
  menu_option_3_position = menu_option_3_position_default;
  hovering_3_players = false;
  scale_3 = 1;
  number_of_players_playing = 3;
  game_state = playing;
  transition_timeout = total_transition_timeout;
}

void select_option4() {
  menu_option_4_position = menu_option_4_position_default;
  hovering_4_players = false;
  scale_4 = 1;
  number_of_players_playing = 4;
  game_state = playing;
  transition_timeout = total_transition_timeout;
}

void select_next_player() {
  selected_player = (selected_player + 1) % number_of_players_playing;
}

bool player_can_move(Vector2 p) {
  bool can_move = false;
  u8 x = cast_u8(p.x);
  u8 y = cast_u8(p.y);

  if(0 < x - 1 && !(board[y][x - 1] & PLAYER)) can_move = true;
  if(x + 1 < 7 && !(board[y][x + 1] & PLAYER)) can_move = true;
  if(0 < y - 1 && !(board[y - 1][x] & PLAYER)) can_move = true;
  if(y + 1 < 7 && !(board[y + 1][x] & PLAYER)) can_move = true;

  return can_move;
}

enum Directions {
  NORTH = 1,
  SOUTH = 2,
  WEST  = 4,
  EAST  = 8,
};

u8 player_is_trapped(Vector2 p) {
  u8 is_trapped = 0;
  u8 x = cast_u8(p.x);
  u8 y = cast_u8(p.y);

  if(x - 1 < 0 || board[y][x - 1] & PLAYER) {
    is_trapped |= WEST;
  } else if(board[y][x - 1] & PLATFORM) {
    is_trapped |= player_can_move({(f32)(x - 1), (f32)y}) ? 0 : WEST;
  }

  if(6 < x + 1 || board[y][x + 1] & PLAYER) {
    is_trapped |= EAST;
  } else if(board[y][x + 1] & PLATFORM) {
    is_trapped |= player_can_move({(f32)(x + 1), (f32)y}) ? 0 : EAST;
  }

  if(y - 1 < 0 || board[y - 1][x] & PLAYER) {
    is_trapped |= NORTH;
  } else if(board[y - 1][x] & PLATFORM) {
    is_trapped |= player_can_move({(f32)x, (f32)(y - 1)}) ? 0 : NORTH;
  }

  if(6 < y + 1 || board[y + 1][x] & PLAYER) {
    is_trapped |= SOUTH;
  } else if(board[y + 1][x] & PLATFORM) {
    is_trapped |= player_can_move({(f32)x, (f32)(y + 1)}) ? 0 : SOUTH;
  }

  return is_trapped;
}

bool is_impossible_to_move() {
  Vector2 p = player_positions[selected_player];
  return player_is_trapped(p) == 15;
}

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

  // MARK: LOOP
  while (!WindowShouldClose()) {
    f32 dt = GetFrameTime();

    if(winner == -1) UpdateMusicStream(bgm);

    if(IsKeyPressed(KEY_SPACE)) {
      switch(game_state) {
        case main_menu: {
          PlaySound(select_sfx);
          if(menu_option_index == 0) {
            select_option2();
          } else if(menu_option_index == 1) {
            select_option3();
          } else if(menu_option_index == 2) {
            select_option4();
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
              reset_game();
              break;
            }
            case 2: CloseWindow(); break;
          }
          break;
        }
        case game_over: {
          reset_game();
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
        select_option2();
      } else if(hovering_3_players) {
        select_option3();
      } else if(hovering_4_players) {
        select_option4();
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
      f32 animation_dt = ease_in_out_quart(animation_current_time / animation_duration);

      if(!FloatEquals(platform_final_position.x, -1)) {
        f32 lerped_x = Lerp(player_positions[selected_player].x, platform_final_position.x, animation_dt);
        player_positions[selected_player].x     = lerped_x;
        platform_positions[selected_platform].x = lerped_x;

        if(FloatEquals(player_positions[selected_player].x, platform_final_position.x)) {
          platform_positions[selected_platform].x = cast_u8(lerped_x);
          Vector2 platform_position = platform_positions[selected_platform];
          if(cast_u8(platform_position.x) == 3
          && cast_u8(platform_position.y) == 3) {
            winner = (s8)selected_player;
            game_state = game_over;
            PlaySound(win_sfx);
          }
          select_next_player();
          selected_platform = -1;
          platform_final_position = {-1,-1};

          clear_board_positions();
          update_board_positions();
          update_board_players(player_positions, number_of_players_playing);

          if(is_impossible_to_move()) select_next_player();
        }

        animation_current_time += dt;
      }

      if(!FloatEquals(platform_final_position.y, -1)) {
        f32 lerped_y = Lerp(player_positions[selected_player].y, platform_final_position.y, animation_dt);
        player_positions[selected_player].y     = lerped_y;
        platform_positions[selected_platform].y = lerped_y;

        if(FloatEquals(player_positions[selected_player].y, platform_final_position.y)) {
          platform_positions[selected_platform].y = cast_u8(lerped_y);
          Vector2 platform_position = platform_positions[selected_platform];
          if(cast_u8(platform_position.x) == 3
          && cast_u8(platform_position.y) == 3) {
            winner = (s8)selected_player;
            game_state = game_over;
            PlaySound(win_sfx);
          }
          select_next_player();
          selected_platform = -1;
          platform_final_position = {-1,-1};

          clear_board_positions();
          update_board_positions();
          update_board_players(player_positions, number_of_players_playing);

          if(is_impossible_to_move()) select_next_player();
        }

        animation_current_time += dt;
      }

    } else {
      animation_current_time = 0;
    }

    update_shader(dt);

    // MARK: RENDER
    BeginDrawing();
    ClearBackground(BLACK);
    draw_bg();

    if((game_state == playing || game_state == game_over || game_state == paused) && FloatEquals(transition_timeout, 0)) {
      draw_board();
      draw_platforms();

      if(selected_platform != -1) {
        draw_platform_frame();
      }

      draw_players(number_of_players_playing);
    } else if(game_state == main_menu || 0 < transition_timeout) {
      draw_game_title();
      draw_menu_options(dt);
    } else if(game_state == ai_menu && FloatEquals(transition_timeout, 0)) {
      draw_ai_menu();
    }

    if(game_state == paused) {
      draw_pause_menu();
    }

    if(winner != -1) {
      draw_winner_text();
    }

    EndDrawing();
  }

  /// @todo: not really necessary
  UnloadFont(font);
  UnloadFont(font_title);
  UnloadMusicStream(bgm);
  #if BUNDLE
  #else
  UnloadSound(win_sfx);
  UnloadSound(move_sfx);
  UnloadSound(select_sfx);
  UnloadSound(sliding_sfx);
  UnloadSound(move_player_sfx);
  UnloadSound(select_platform_sfx);

  UnloadShader(player_outline);
  UnloadShader(bg_color_fs);

  for(auto player_texture : player_textures) {
    UnloadTexture(player_texture);
  }

  UnloadTexture(board_texture);
  UnloadTexture(platform_texture);
  UnloadTexture(platform_frame_texture);
  UnloadTexture(arrow_left);
  UnloadTexture(arrow_right);
  #endif

  CloseWindow();
  return 0;
}