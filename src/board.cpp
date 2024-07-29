u8 PLAYER   = 1;
u8 PLATFORM = 2;

u8 board[7][7] = {};
// {//0,1,2,3,4,5,6
//   {x,x,x,x,x,x,x}, // 0
//   {x,x,x,x,x,x,x}, // 1
//   {x,x,x,x,x,x,x}, // 2
//   {x,x,x,x,x,x,x}, // 3
//   {x,x,x,x,x,x,x}, // 4
//   {x,x,x,x,x,x,x}, // 5
//   {x,x,x,x,x,x,x}, // 6
// }
// [1][3]

#define number_of_players 4
Vector2 player_positions[number_of_players] = {
  // Top left
  {0,0},
  // Top right
  {6,0},
  // Bottom left
  {0,6},
  // Bottom right
  {6,6},
};

void clear_board_positions() {
  for(u8 row = 0; row < 7; row++)
    for(u8 column = 0; column < 7; column++)
    board[row][column] = 0;
}

void update_board_positions(Vector2 platform_positions[12]) {
  for(u8 i = 0; i < 12; i++) {
    Vector2 p = platform_positions[i];
    board[cast_u8(p.y)][cast_u8(p.x)] |= PLATFORM;
  }

  // for(u8 i = 0; i < 2; i++) {
  //   Vector2 p = player_positions[i];
  //   board[cast_u8(p.y)][cast_u8(p.x)] |= PLAYER;
  //   // log("board", board[(u8)p.y][(u8)p.x]);
  //   // log("has player", board[(u8)p.y][(u8)p.x] & PLAYER);
  // }
}

void update_board_players(Vector2 positions[number_of_players]) {
  for(u8 row = 0; row < 7; row++)
    for(u8 column = 0; column < 7; column++)
      board[row][column] &= ~PLAYER;

  for(u8 i = 0; i < number_of_players; i++) {
    Vector2 p = positions[i];
    board[cast_u8(p.y)][cast_u8(p.x)] |= PLAYER;
  }
}

/// @todo: probably a better function would be this one below, since both are coupled by time
// void update_board() {
//   clear_board_positions();
//   update_board_positions();
// }

Vector2 convert_board_position_to_screen_position(Vector2 offset, Vector2 position, u8 move_distance) {
  return offset + position * move_distance;
}

bool convert_player_position() {
  // s8 x = ((s8)player_positions[selected_player].x) + 1;
  // board[x][(u8)player_positions[selected_player].y];
  return false;
}