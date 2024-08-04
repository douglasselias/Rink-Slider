u8 pause_menu_option_index = 0;

void draw_pause_menu() {
  f32 rect_width = 400;
  /// @todo: there is a Fade fn from raylib
  Color dark_gray = {80, 80, 80, 255-20};
  /// @todo: hardcoded 14, should get from board_position.y
  /// @todo: hardcoded 346, should get from board_texture.height * scale(which is 2)
  Rectangle rect = {screen_center.x - rect_width/2, 14, rect_width, 346*2};
  f32 roundness = 0.1f;
  s32 segments = 0;
  DrawRectangleRounded(rect, roundness, segments, dark_gray);
  f32 thickness = 10;
  f32 gap = 2;
  rect.x      += thickness - gap;
  rect.y      += thickness - gap;
  rect.width  -= thickness - gap;
  rect.height -= thickness - gap;
  DrawRectangleRoundedLines(rect, roundness, segments, thickness, Fade(GOLD, 0.9f));

  const char* text = "Paused";
  Vector2 text_size = measure_text(text);
  Vector2 text_position = {screen_center.x - text_size.x/2, 30};
  /// @todo: maybe use a global constant for 6, as a default shadow position
  draw_text(text, text_position + 6, BLACK);
  draw_text(text, text_position, GOLD);

  // Draw options
  // draw_text("", text_position, GOLD);
  f32 base_y = screen_center.y - (text_size.y / 2) - 100;
  draw_text_centered("Resume", base_y, GOLD, pause_menu_option_index == 0);
  draw_text_centered("Main Menu", base_y + text_size.y + 20, GOLD, pause_menu_option_index == 1);
  draw_text_centered("Quit", base_y + (text_size.y + 20) * 2, GOLD, pause_menu_option_index == 2);
}