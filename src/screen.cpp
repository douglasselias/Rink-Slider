const char *game_title = "Rink Slider";
const u16 screen_width  = 1280;
const u16 screen_height = 720;
const u16 half_screen_width  = screen_width  / 2;
const u16 half_screen_height = screen_height / 2;
const Vector2 screen_center = {half_screen_width, half_screen_height};

void init_screen() {
  SetTraceLogLevel(LOG_WARNING);
  InitWindow(screen_width, screen_height, game_title);
  /// @todo: option to change resolution/fullscreen
  // SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetTargetFPS(60);
}
