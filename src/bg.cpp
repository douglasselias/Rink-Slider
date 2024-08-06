Texture2D bg_texture;
static f32 time = 0;
f32 bg_movement_speed = 0.1f;

void init_bg() {
  bg_texture = LoadTexture("gfx/bg.png");
  GenTextureMipmaps(&bg_texture);
  SetTextureWrap(bg_texture, TEXTURE_WRAP_REPEAT);
  SetTextureFilter(bg_texture, TEXTURE_FILTER_TRILINEAR);
}

void draw_bg() {
  u64 bg_column_chunk = (u64)screen_width  / (u64)bg_texture.width;
  u64 bg_row_chunk    = (u64)screen_height / (u64)bg_texture.height;

  BeginShaderMode(bg_color_fs);
  for(u64 i = 0; i < bg_column_chunk; i++) {
    for(u64 j = 0; j <= bg_row_chunk; j++) {
      DrawTextureV(bg_texture, {(f32)i*bg_texture.width,(f32)j*bg_texture.height}, palette[palette_index]);
    }
  }
  EndShaderMode();
}