u8 PLAYER   = 1;
u8 PLATFORM = 2;

u8 board[7][7] = {};

Vector2 initial_platform_positions[12] = {
  // Top left
  {0,0},
  {1,0},
  {0,1},

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

// Triple diagonal
// Vector2 initial_platform_positions[12] = {
//   // Top left
//   {0,0},
//   {3,3},
//   {3,3},

//   // Top right
//   {1,1},
//   {3,3},
//   {3,3},

//   // Bottom left
//   {0,6},
//   {0,5},
//   {1,6},

//   // Bottom right
//   {6,6},
//   {3,3},
//   {3,3},
// };

// Double Diagonal
// Vector2 initial_platform_positions[12] = {
//   // Top left
//   {0,0},
//   {3,3},
//   {3,3},

//   // Top right
//   {1,1},
//   {3,3},
//   {3,3},

//   // Bottom left
//   {0,6},
//   {3,3},
//   {3,3},

//   // Bottom right
//   {6,6},
//   {3,3},
//   {3,3},
// };

// Skewed position
// Vector2 initial_platform_positions[12] = {
//   // Top left
//   {0,0},
//   {3,3},
//   {3,3},

//   // Top right
//   {1,1},
//   {3,3},
//   {3,3},

//   // Bottom left
//   {0,6},
//   {1,6},
//   {3,3},

//   // Bottom right
//   {6,6},
//   {3,3},
//   {3,3},
// };

// Middle trap
// Vector2 initial_platform_positions[12] = {
//   // Top left
//   {0,0},
//   {3,3},
//   {3,3},

//   // Top right
//   {2,3},
//   {3,3},
//   {3,3},

//   // Bottom left
//   {0,3},
//   {1,6},
//   {3,3},

//   // Bottom right
//   {0,6},
//   {3,3},
//   {3,3},
// };

Vector2 platform_positions[12] = {};
Vector2 platform_final_position = {-1,-1};
s8 selected_platform = -1;

Texture2D board_texture;
Vector2 board_position;
u8 board_texture_scale = 2;
u8 board_border_thickness = 5;
Vector2 board_top_left;

Texture2D platform_texture;
Texture2D platform_frame_texture;
u8 move_distance;

f32 animation_current_time = 0;
f32 animation_duration = 1;

void reset_platform_positions() {
  for(u8 i = 0; i < 12; i++) {
    platform_positions[i] = initial_platform_positions[i];
  }
}

void update_board_positions() {
  for(u8 i = 0; i < 12; i++) {
    Vector2 p = platform_positions[i];
    board[cast_u8(p.y)][cast_u8(p.x)] |= PLATFORM;
  }
}

void init_board() {
  #if BUNDLE
  board_texture = load_texture_board();
  platform_frame_texture = load_texture_platform_frame();
  platform_texture = load_texture_platform();
  #else
  board_texture = LoadTexture("gfx/board.png");
  platform_frame_texture = LoadTexture("gfx/platform_frame.png");
  platform_texture = LoadTexture("gfx/platform.png");
  #endif
  board_position = {
    screen_center.x - (f32)board_texture.width,
    screen_center.y - (f32)board_texture.height,
  };
  board_top_left = board_position + board_border_thickness * board_texture_scale;
  move_distance = cast_u8(platform_texture.width) * board_texture_scale;

  reset_platform_positions();
  update_board_positions();
}

void clear_board_positions() {
  for(u8 row = 0; row < 7; row++)
    for(u8 column = 0; column < 7; column++)
      board[row][column] = 0;
}

void update_board_players(Vector2 positions[], u8 n_of_player) {
  for(u8 row = 0; row < 7; row++)
    for(u8 column = 0; column < 7; column++)
      board[row][column] &= ~PLAYER;

  for(u8 i = 0; i < n_of_player; i++) {
    Vector2 p = positions[i];
    board[cast_u8(p.y)][cast_u8(p.x)] |= PLAYER;
  }
}

Vector2 convert_board_position_to_screen_position(Vector2 offset, Vector2 position) {
  return offset + position * move_distance;
}

void draw_board() {
  draw_texture(board_texture, board_position, board_texture_scale);
}

void draw_platforms() {
  for(auto platform_position : platform_positions) {
    Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_position);
    draw_texture(platform_texture, screen_position, board_texture_scale);
  }
}

void draw_platform_frame() {
  Vector2 screen_position = convert_board_position_to_screen_position(board_top_left, platform_positions[selected_platform]);
  f32 platform_frame_texture_scale = 1.5;
  draw_texture(platform_frame_texture, screen_position, platform_frame_texture_scale);
}