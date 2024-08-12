flags := "-std=c++11 -Wall -Wextra -Wconversion -Wdouble-promotion -Werror=unused-comparison -Werror=pointer-bool-conversion -Wformat=2 -Wpadded -Wshadow -fno-exceptions -fno-rtti -fstrict-aliasing -Wstrict-aliasing=2 -fstack-protector-all -fsanitize=address,undefined -fsanitize-trap=all -O0 -g -o"

prod_flags := "-march=native -ffast-math -static -fno-exceptions -fno-rtti"
perf_debug := "-fopt-info-vec-optimized -fprofile-generate | -fprofile-use"

windows_flags := "-std=c++11 -Wall -Wshadow -fno-exceptions -fno-rtti -O0 -g3 -o"

just:
  mkdir -p build && clear && clang++ -DBUNDLE {{flags}} ./build/rink_slider main.cpp ./vendor/libraylib_linux.a && ./build/rink_slider

bundle:
	rm -r bundle && mkdir -p bundle && clear && clang++ {{flags}} ./build/bundler bundler.cpp ./vendor/libraylib_linux.a && ./build/bundler

release_linux:
	zip -r build/rink_slider_linux.zip build/rink_slider fonts/ sfx/8bit_bossa.mp3

release_windows:
	zip -r build/rink_slider_windows.zip build/rink_slider.exe fonts/ sfx/8bit_bossa.mp3

debug:
  gf2 ./build/rink_slider

test:
  mkdir -p build && clang -std=c99 -Wall -Wextra -g3 -lcriterion test.c -o ./build/test && ./build/test

windows:
  mkdir -p build && x86_64-w64-mingw32-gcc -DBUNDLE {{windows_flags}} ./build/rink_slider.exe main.cpp -I./vendor -L./vendor -lraylib_windows -lwinmm -lgdi32 ./vendor/libraylib_windows.a && sudo wine build/rink_slider.exe

icon:
	windres resource.rc -O coff -o resource.res
	gcc -o mygame.exe mygame.o resource.res -mwindows