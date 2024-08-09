s32 load_font_size = 100;
f32 font_spacing = 0;

Font font;
f32 font_size = 70;

Font font_title;
f32 font_title_size = 90;

void init_font() {
  #if BUNDLE
  font = LoadFontEx("fonts/russo_one_regular.ttf", load_font_size, NULL, 0);
  #endif
  SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);
}

void init_font_title() {
  #if BUNDLE
  s32 codepoint_count = 0;
  s32 *codepoints = LoadCodepoints("RinkSlider", &codepoint_count);
  font_title = LoadFontEx("fonts/rowdies_regular.ttf", load_font_size, codepoints, codepoint_count);
  /// @todo: not really necessary
  UnloadCodepoints(codepoints);
  #endif
  SetTextureFilter(font_title.texture, TEXTURE_FILTER_BILINEAR);
}

Vector2 measure_text(const char* text) {
  return MeasureTextEx(font, text, font_size, font_spacing);
}

void draw_text(const char* text, Vector2 position, Color color) {
  DrawTextEx(font, text, position, font_size, font_spacing, color);
}

Vector2 measure_text_title(const char* text) {
  return MeasureTextEx(font_title, text, font_title_size, font_spacing);
}

void draw_text_title(const char* text, Vector2 position, Color color) {
  DrawTextEx(font_title, text, position, font_title_size, font_spacing, color);
}

void draw_text_centered(const char* text, f32 y, Color color, bool shadow_text = false) {
  Vector2 text_size = measure_text(text);
  Vector2 text_position = {screen_center.x - text_size.x/2, y};
  if(shadow_text) draw_text(text, text_position + 6, BLACK);
  draw_text(text, text_position, color);
}