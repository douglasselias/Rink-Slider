#include "assert.h"
#include "stdio.h"
#include "raylib.h"
#include "raymath.h"

#include "std/types.cpp"
#include "std/log.cpp"
#include "std/macros.cpp"
#include "std/vector_overload.cpp"

s32 main() {
  FILE* bundle_file = fopen("bundle/bundle.cpp", "w");

  fprintf(bundle_file,
    "#define Img(name) \\\n"
    "\tImage name##_img = { \\\n"
      "\t\t.data    = name##_DATA, \\\n"
      "\t\t.width   = name##_WIDTH, \\\n"
      "\t\t.height  = name##_HEIGHT, \\\n"
      "\t\t.mipmaps = 1, \\\n"
      "\t\t.format  = name##_FORMAT, \\\n"
    "\t}\n\n");

  FilePathList file_path_list = LoadDirectoryFiles("gfx");
  for(u8 i = 0; i < file_path_list.count; i++) {
    char* file_path = file_path_list.paths[i];
    const char* file_name = GetFileNameWithoutExt(file_path);
    const char* file_export = TextFormat("%s.cpp", file_name);

    Image img = LoadImage(file_path);
    bool success = ExportImageAsCode(img, TextFormat("bundle/%s", file_export));
    if(success) log("Exported succesfully", file_export);
    else        log("Something went wrong while exporting.", file_export);
    UnloadImage(img);

    const char* file_name_wo_ext = GetFileNameWithoutExt(file_export);
    fprintf(bundle_file, "#include \"%s\"\n", file_export);
    fprintf(bundle_file, "Img(%s);\n", TextToUpper(file_name_wo_ext));
    fprintf(bundle_file, "Texture2D load_texture_%s() { return LoadTextureFromImage(%s_img); }\n\n", file_name_wo_ext, TextToUpper(file_name_wo_ext));
  }

  UnloadDirectoryFiles(file_path_list);
  fclose(bundle_file);
}