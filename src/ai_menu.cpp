Texture2D arrow_left;
Texture2D arrow_right;

Vector2 arrow_left_position  = {};
Vector2 arrow_right_position = {};

bool arrow_left_colliding = false;
bool arrow_right_colliding = false;

void init_ai_menu() {
  #if BUNDLE
  arrow_left  = load_texture_arrow_left();
  arrow_right = load_texture_arrow_right();
  #else
  arrow_left  = LoadTexture("gfx/arrow_left.png");
  arrow_right = LoadTexture("gfx/arrow_right.png");
  #endif
  arrow_left_position  = screen_center - (Vector2){(f32)100 + arrow_left.width, 0};
  arrow_right_position = screen_center + (Vector2){100, 0};
}

void check_mouse_collision_with_arrows(Vector2 mouse_position) {
  arrow_left_colliding = CheckCollisionPointTriangle(mouse_position, arrow_left_position + (Vector2){0, (f32)arrow_left.height/2}, arrow_left_position + (Vector2){(f32)arrow_left.width, 0}, arrow_left_position + (Vector2){(f32)arrow_left.width, (f32)arrow_left.height});
  arrow_right_colliding = CheckCollisionPointTriangle(mouse_position, arrow_right_position, arrow_right_position + (Vector2){0, (f32)arrow_right.height}, arrow_right_position + (Vector2){(f32)arrow_right.width, (f32)arrow_right.height/2});
}

u8 number_of_ai_players = 0;
void draw_ai_menu() {
  f32 base_y = screen_center.y - 130;
  draw_text_centered("How many AI players?", base_y, GOLD, true);
  draw_texture(arrow_left, arrow_left_position);
  draw_text_centered(TextFormat("%d", number_of_ai_players), base_y + 120, GOLD, true);
  draw_texture(arrow_right, arrow_right_position);
}