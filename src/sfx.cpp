Sound win_sfx;
Sound move_sfx;
Sound move_player_sfx;
Sound select_sfx;
Sound sliding_sfx;
Sound select_platform_sfx;

void init_sfx() {
  #if BUNDLE
  move_sfx = load_sound_select();
  move_player_sfx = load_sound_move_player();
  select_sfx = load_sound_confirmation();
  win_sfx = load_sound_win();
  sliding_sfx = load_sound_sliding_platform();
  select_platform_sfx = load_sound_select_platform();
  #else
  move_sfx = LoadSound("sfx/select.ogg");
  move_player_sfx = LoadSound("sfx/move_player.ogg");
  select_sfx = LoadSound("sfx/confirmation.ogg");
  win_sfx = LoadSound("sfx/win.wav");
  sliding_sfx = LoadSound("sfx/sliding_platform.ogg");
  select_platform_sfx = LoadSound("sfx/select_platform.ogg");
  #endif

  SetSoundVolume(win_sfx, 2.0);
  SetSoundVolume(sliding_sfx, 1.4f);
  SetSoundVolume(select_platform_sfx, 0.5);

}