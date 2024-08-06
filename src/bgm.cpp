Music bgm;

void init_bgm() {
  bgm = LoadMusicStream("sfx/8bit_bossa.mp3");
  // bgm = LoadMusicStream("sfx/peachtea_somewhere_in_the_elevator.ogg");
  PlayMusicStream(bgm);
}
