#define number_of_players 4
Vector2 initial_positions[number_of_players] = {
  // Top left
  {0,0},
  // Bottom right
  {6,6},
  // Top right
  {6,0},
  // Bottom left
  {0,6},
};

// Corner Trap
// Vector2 initial_positions[number_of_players] = {
//   // Top left
//   {0,0},
//   // Bottom right
//   {6,6},
//   // Top right
//   {1,1},
//   // Bottom left
//   {0,6},
// };

// Middle Trap
// Vector2 initial_positions[number_of_players] = {
//   // Top left
//   {0,0},
//   // Bottom right
//   {0,6},
//   // Top right
//   {2,3},
//   // Bottom left
//   {0,3},
// };

Vector2 player_positions[number_of_players] = {};
s8 winner = -1;
u8 selected_player = 0;

Texture2D player_textures[4] = {};
const char* player_colors[4] = {"orange", "green", "blue", "purple"};
Vector2 player_offset;

Shader player_outline;

void reset_player_positions() {
  for(u8 i = 0; i < number_of_players; i++) {
    player_positions[i] = initial_positions[i];
  }
}

void init_players() {
  u8 player_color_index = 0;
  for(auto &player_texture : player_textures) {
    player_texture = LoadTexture(TextFormat("gfx/player_%s.png", player_colors[player_color_index++]));
  }
  player_offset = {(f32)player_textures[0].width / 4, 0};

  reset_player_positions();

  player_outline = LoadShader(0, "shaders/player_outline.fs");
  s32 texture_size_location = GetShaderLocation(player_outline, "textureSize");
  f32 texture_size_value[2] = { (f32)player_textures[0].width, (f32)player_textures[0].height };
  SetShaderValue(player_outline, texture_size_location, texture_size_value, SHADER_UNIFORM_VEC2);
}

void draw_players(u8 number_of_players_playing) {
  for(u8 i = 0; i < number_of_players_playing; i++) {
    Vector2 player_position_screen = convert_board_position_to_screen_position(board_top_left + player_offset, player_positions[i]);
    if(selected_player == i) BeginShaderMode(player_outline);
    DrawTextureV(player_textures[i], player_position_screen, WHITE);
    if(selected_player == i) EndShaderMode();
  }
}

void draw_winner_text() {
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