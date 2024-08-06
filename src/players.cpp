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

  player_outline = LoadShader(0, "shaders/outline.fs");
  s32 texture_size_location = GetShaderLocation(player_outline, "textureSize");
  f32 texture_size_value[2] = { (f32)player_textures[0].width, (f32)player_textures[0].height };
  SetShaderValue(player_outline, texture_size_location, texture_size_value, SHADER_UNIFORM_VEC2);
}
