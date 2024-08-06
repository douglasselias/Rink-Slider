u8 pause_menu_option_index = 0;

void draw_pause_menu() {
  f32 rect_width = 400;
  f32 thickness = 10;
  Rectangle rect = {screen_center.x - rect_width/2, 0, rect_width, screen_height-thickness};
  f32 roundness = 0.1f;
  s32 segments = 0;
  DrawRectangleRounded(rect, roundness, segments, Fade(DARKGRAY, 0.9f));

  f32 gap = 2;
  rect.x      += thickness - gap;
  rect.y      += thickness - gap;
  rect.width  -= thickness - gap;
  rect.height -= thickness - gap;
  DrawRectangleRoundedLines(rect, roundness, segments, thickness, Fade(GOLD, 0.9f));

  const char* text = "Paused";
  Vector2 text_size = measure_text(text);
  draw_text_centered(text, 30, GOLD, true);

  f32 base_y = screen_center.y - (text_size.y / 2) - 100;
  draw_text_centered("Resume", base_y, GOLD, pause_menu_option_index == 0);
  draw_text_centered("Main Menu", base_y + text_size.y + 20, GOLD, pause_menu_option_index == 1);
  draw_text_centered("Quit", base_y + (text_size.y + 20) * 2, GOLD, pause_menu_option_index == 2);
}