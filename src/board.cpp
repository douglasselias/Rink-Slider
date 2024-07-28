bool board[7][7] = {};
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

void clear_board_positions() {
  for(u8 row = 0; row < 7; row++)
    for(u8 column = 0; column < 7; column++)
    board[row][column] = false;
}

void update_board_positions(Vector2 platform_positions[12]) {
  for(u8 i = 0; i < 12; i++) {
    Vector2 platform_position = platform_positions[i];
    board[(u8)platform_position.y][(u8)platform_position.x] = true;
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