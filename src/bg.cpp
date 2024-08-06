Texture2D bg_texture;
static f32 time = 0;
f32 bg_movement_speed = 0.1f;

void init_bg() {
  bg_texture = LoadTexture("gfx/bg.png");
  GenTextureMipmaps(&bg_texture);
  SetTextureWrap(bg_texture, TEXTURE_WRAP_REPEAT);
  SetTextureFilter(bg_texture, TEXTURE_FILTER_TRILINEAR);
}