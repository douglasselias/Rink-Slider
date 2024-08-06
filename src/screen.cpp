const char *game_title = "Rink Slider";
const u16 screen_width  = 1280;
const u16 screen_height = 720;
const u16 half_screen_width  = screen_width  / 2;
const u16 half_screen_height = screen_height / 2;
const Vector2 screen_center = {half_screen_width, half_screen_height};

void init_screen() {
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen_width, screen_height, game_title);
  SetTargetFPS(60);
  InitAudioDevice();
}

void draw_texture(Texture2D texture, Vector2 position, f32 scale = 1, Color tint = WHITE) {
  Rectangle source = {0, 0, (f32)texture.width, (f32)texture.height};
  Rectangle dest = {position.x, position.y, (f32)texture.width * scale, (f32)texture.height * scale};
  Vector2 origin = {0,0};
  u8 rotation = 0;
  DrawTexturePro(texture, source, dest, origin, rotation, tint);
}

f32 ease_in_out_quart(f32 x) {
  return x < 0.5f ? 8 * (f32)pow(x, 4) : 1 - (f32)pow(-2 * x + 2, 4) / 2;
}