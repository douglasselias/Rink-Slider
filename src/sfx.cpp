Sound win_sfx;
Sound move_sfx;
Sound move_player_sfx;
Sound select_sfx;
Sound sliding_sfx;
Sound select_platform_sfx;

void init_sfx() {
  win_sfx = LoadSound("sfx/win.wav");
  SetSoundVolume(win_sfx, 2.0);

  sliding_sfx = LoadSound("sfx/sliding_platform.ogg");
  SetSoundVolume(sliding_sfx, 1.4f);

  select_platform_sfx = LoadSound("sfx/select_platform.ogg");
  SetSoundVolume(select_platform_sfx, 0.5);

  move_sfx = LoadSound("sfx/select.ogg");
  move_player_sfx = LoadSound("sfx/move_player.ogg");
  select_sfx = LoadSound("sfx/confirmation.ogg");
}