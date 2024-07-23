bool board[7][7] = {};

void clear_board_positions() {
  for(u8 i = 0; i < 7; i++)
    for(u8 j = 0; j < 7; j++)
    board[i][j] = false;
}

void update_board_positions(Vector2 platform_positions[12]) {
  for(u8 i = 0; i < 12; i++) {
    Vector2 platform_position = platform_positions[i];
    board[(u8)platform_position.x][(u8)platform_position.y] = true;
  }
}

/// @todo: probably a better function would be this one below, since both are coupled by time
// void update_board() {
//   clear_board_positions();
//   update_board_positions();
// }