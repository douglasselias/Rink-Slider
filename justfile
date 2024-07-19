flags := "-std=c++11 -Wall -Wshadow -fno-exceptions -fno-rtti -fstack-protector-all -fsanitize=address,undefined -fsanitize-trap=all -O0 -g3 -o"
optional_flags := "-Wextra -Wconversion -Wdouble-promotion -Oi"

prod_flags := "-march=native -ffast-math | -static | -fprofile-generate -fprofile-use | -fopt-info-vec-optimized"
# [-Wliteral-conversion]

windows_flags := "-std=c++11 -Wall -Wshadow -fno-exceptions -fno-rtti -O0 -g3 -o"

just:
  mkdir -p build && clang++ {{flags}} ./build/main main.cpp ./vendor/libraylib.a && ./build/main

debug:
  gf2 ./build/main

test:
  mkdir -p build && clang -std=c99 -Wall -Wextra -g3 -lcriterion test.c -o ./build/test && ./build/test

windows:
  mkdir -p build && x86_64-w64-mingw32-gcc {{windows_flags}} ./build/main.exe main.cpp -I./vendor -L./vendor -lraylib_windows -lwinmm -lgdi32 ./vendor/libraylib_windows.a && sudo wine build/main.exe
