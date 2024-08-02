Texture2D menu_option_2_players;
Texture2D menu_option_3_players;
Texture2D menu_option_4_players;

bool hovering_2_players = false;
bool hovering_3_players = false;
bool hovering_4_players = false;

f32 hover_scale = 1.1f;

Vector2 menu_option_2_position_default = {};
Vector2 menu_option_3_position_default = {};
Vector2 menu_option_4_position_default = {};

Vector2 menu_option_2_position = {};
Vector2 menu_option_3_position = {};
Vector2 menu_option_4_position = {};

Shader outline_fs;

s8 menu_option_index = 0;

void init_menu_options() {
  /// @todo: this belongs to bg texture
  outline_fs = LoadShader(0, "shaders/outline.fs");

  menu_option_2_players = LoadTexture("gfx/players_2.png");
  SetTextureFilter(menu_option_2_players, TEXTURE_FILTER_BILINEAR);
  menu_option_3_players = LoadTexture("gfx/players_3.png");
  SetTextureFilter(menu_option_3_players, TEXTURE_FILTER_BILINEAR);
  menu_option_4_players = LoadTexture("gfx/players_4.png");
  SetTextureFilter(menu_option_4_players, TEXTURE_FILTER_BILINEAR);

  s32 menu_gap = 30;
  s32 menu_x_offset = -menu_gap + (menu_option_2_players.width + menu_option_3_players.width + menu_option_4_players.width) / 3;

  menu_option_2_position_default = {(f32)menu_x_offset, screen_center.y - (f32)menu_option_2_players.height / 2};
  menu_option_3_position_default = {(f32)menu_x_offset + (f32)menu_gap + (f32)menu_option_2_players.width, screen_center.y - (f32)menu_option_3_players.height / 2};
  menu_option_4_position_default = {(f32)menu_x_offset + (f32)menu_gap * 2 + (f32)menu_option_2_players.width + (f32)menu_option_3_players.width, screen_center.y - (f32)menu_option_4_players.height / 2};

  menu_option_2_position = menu_option_2_position_default;
  menu_option_3_position = menu_option_3_position_default;
  menu_option_4_position = menu_option_4_position_default;
}

f32 lerp_scale = 25;
void check_mouse_collision_with_menu_options(Vector2 mouse_position, f32 dt) {
  f32 hover_scale_2 = hovering_2_players ? hover_scale : 1;
  hovering_2_players = CheckCollisionPointRec(mouse_position, (Rectangle){menu_option_2_position.x, menu_option_2_position.y, (f32)menu_option_2_players.width * hover_scale_2, (f32)menu_option_2_players.height * hover_scale_2});

  f32 hover_scale_3 = hovering_3_players ? hover_scale : 1;
  hovering_3_players = CheckCollisionPointRec(mouse_position, (Rectangle){menu_option_3_position.x, menu_option_3_position.y, (f32)menu_option_3_players.width * hover_scale_3, (f32)menu_option_3_players.height * hover_scale_3});

  f32 hover_scale_4 = hovering_4_players ? hover_scale : 1;
  hovering_4_players = CheckCollisionPointRec(mouse_position, (Rectangle){menu_option_4_position.x, menu_option_4_position.y, (f32)menu_option_4_players.width * hover_scale_4, (f32)menu_option_4_players.height * hover_scale_4});

  if(menu_option_index == 0 || hovering_2_players) {
    Vector2 final_position = menu_option_2_position_default - ((Vector2){(f32)menu_option_2_players.width, (f32)menu_option_2_players.height} * (hover_scale - 1)) / 2;
    menu_option_2_position = Vector2Lerp(menu_option_2_position, final_position, dt * lerp_scale);
  } else {
    menu_option_2_position = Vector2Lerp(menu_option_2_position, menu_option_2_position_default, dt * lerp_scale);
  }

  if(menu_option_index == 1 || hovering_3_players) {
    Vector2 final_position = menu_option_3_position_default - ((Vector2){(f32)menu_option_3_players.width, (f32)menu_option_3_players.height} * (hover_scale - 1)) / 2;
    menu_option_3_position = Vector2Lerp(menu_option_3_position, final_position, dt * lerp_scale);
  } else {
    menu_option_3_position = Vector2Lerp(menu_option_3_position, menu_option_3_position_default, dt * lerp_scale);
  }

  if(menu_option_index == 2 || hovering_4_players) {
    Vector2 final_position = menu_option_4_position_default - ((Vector2){(f32)menu_option_4_players.width, (f32)menu_option_4_players.height} * (hover_scale - 1)) / 2;
    menu_option_4_position = Vector2Lerp(menu_option_4_position, final_position, dt * lerp_scale);
  } else {
    menu_option_4_position = Vector2Lerp(menu_option_4_position, menu_option_4_position_default, dt * lerp_scale);
  }
}

/// @todo: I could also declare this as static inside the function to not polute the global scope...
f32 scale_2 = 1;
f32 scale_3 = 1;
f32 scale_4 = 1;
/// @todo: maybe join with the other lerp constant (lerp_scale), instead of having two separate constants
f32 lerp_hover_scale = 25;
void draw_menu_options(f32 dt) {
  // static f32 scale_2 = 1;
  // static f32 scale_3 = 1;
  // static f32 scale_4 = 1;
  scale_2 = (menu_option_index == 0 || hovering_2_players) ? Lerp(scale_2, hover_scale, dt * lerp_hover_scale) : Lerp(scale_2, 1, dt * lerp_hover_scale);
  draw_texture(menu_option_2_players, menu_option_2_position, scale_2, menu_option_index == 0 ? GOLD : WHITE);
  scale_3 = (menu_option_index == 1 || hovering_3_players) ? Lerp(scale_3, hover_scale, dt * lerp_hover_scale) : Lerp(scale_3, 1, dt * lerp_hover_scale);
  draw_texture(menu_option_3_players, menu_option_3_position, scale_3, menu_option_index == 1 ? GOLD : WHITE);
  scale_4 = (menu_option_index == 2 || hovering_4_players) ? Lerp(scale_4, hover_scale, dt * lerp_hover_scale) : Lerp(scale_4, 1, dt * lerp_hover_scale);
  draw_texture(menu_option_4_players, menu_option_4_position, scale_4, menu_option_index == 2 ? GOLD : WHITE);
}