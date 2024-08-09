#include "assert.h"
#include "stdio.h"
#include "string.h"
#include "raylib.h"
#include "raymath.h"

#include "std/types.cpp"
#include "std/log.cpp"
#include "std/macros.cpp"
#include "std/vector_overload.cpp"

s32 main() {
  FILE* bundle_file = fopen("bundle/bundle.cpp", "w");

  InitWindow(100, 100, "Bundler");

  fprintf(bundle_file,
    "#define Img(name) \\\n"
    "\tImage name##_img = { \\\n"
      "\t\t.data    = name##_DATA, \\\n"
      "\t\t.width   = name##_WIDTH, \\\n"
      "\t\t.height  = name##_HEIGHT, \\\n"
      "\t\t.mipmaps = 1, \\\n"
      "\t\t.format  = name##_FORMAT, \\\n"
    "\t}\n\n");

  FilePathList file_path_list_gfx = LoadDirectoryFiles("gfx");
  for(u8 i = 0; i < file_path_list_gfx.count; i++) {
    char* file_path = file_path_list_gfx.paths[i];
    const char* file_name = GetFileNameWithoutExt(file_path);
    const char* file_export = TextFormat("%s.cpp", file_name);

    Image img = LoadImage(file_path);
    ExportImageAsCode(img, TextFormat("bundle/%s", file_export));
    log("------------------------");
    UnloadImage(img);

    const char* file_name_wo_ext = GetFileNameWithoutExt(file_export);
    fprintf(bundle_file, "#include \"%s\"\n", file_export);
    fprintf(bundle_file, "Img(%s);\n", TextToUpper(file_name_wo_ext));
    fprintf(bundle_file, "Texture2D load_texture_%s() { return LoadTextureFromImage(%s_img); }\n\n", file_name_wo_ext, TextToUpper(file_name_wo_ext));
  }
  UnloadDirectoryFiles(file_path_list_gfx);

  // Fonts
  #if 0
  FilePathList file_path_list_fonts = LoadDirectoryFiles("fonts");
  for(u8 i = 0; i < file_path_list_fonts.count; i++) {
    char* file_path = file_path_list_fonts.paths[i];
    const char* file_name = GetFileNameWithoutExt(file_path);
    const char* file_export = TextFormat("%s.cpp", file_name);
    
    if(file_name[1] == 'o') {
      s32 codepoint_count = 0;
      s32 *codepoints = LoadCodepoints("RinkSlider", &codepoint_count);
      Font font = LoadFontEx(file_path, 100, codepoints, codepoint_count);
      ExportFontAsCode(font, TextFormat("bundle/%s", file_export));
      UnloadCodepoints(codepoints);
      UnloadFont(font);
    } else {
      Font font = LoadFontEx(file_path, 100, NULL, 0);
      ExportFontAsCode(font, TextFormat("bundle/%s", file_export));
      UnloadFont(font);
    }
    fprintf(bundle_file, "#include \"%s\"\n", file_export);
    log("------------------------");
  }
  UnloadDirectoryFiles(file_path_list_fonts);
  #endif

  FilePathList file_path_list_shaders = LoadDirectoryFiles("shaders");
  for(u8 i = 0; i < file_path_list_shaders.count; i++) {
    char* file_path = file_path_list_shaders.paths[i];
    const char* file_name = GetFileNameWithoutExt(file_path);
    const char* file_export = TextFormat("%s.cpp", file_name);

    char* shader_text = LoadFileText(file_path);
    fprintf(bundle_file, "const char* %s_shader = ", file_name);
    for(char* line = strtok(shader_text, "\n"); line != NULL; line = strtok(NULL, "\n")) {
      fprintf(bundle_file, "\"%s\\n\"\n", line);
    }
    fprintf(bundle_file, ";\n");
    UnloadFileText(shader_text);
  }
  UnloadDirectoryFiles(file_path_list_shaders);

  fclose(bundle_file);
  CloseWindow();
}