if [ ! -d "build/web" ]; then
  mkdir -p build/web
fi

rm -rf build/web/*

emcc -o build/web/index.html main.c -Os -Wall \
  ./build/raylib/src/libraylib.a \
  -I. -Ibuild/raylib/src \
  -L. -Lbuild/raylib/src/libraylib.a \
  -s USE_GLFW=3 \
  -s ASYNCIFY \
  --shell-file build/raylib/src/shell.html \
  -DPLATFORM_WEB

# create a zip file to upload to itch.io
zip -r build/game.zip build/web
