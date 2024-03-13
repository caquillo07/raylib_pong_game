cd build || exit
if [ ! -d "raylib" ]; then
  # todo use a specific release
  git clone https://github.com/raysan5/raylib.git
fi
cd raylib || exit
git pull
cd src || exit
# exit if EMSDK is not set
if [ -z "${EMSDK}" ]; then
  echo "EMSDK is not set"
  exit 1
fi
make EMSDK_PATH="${EMSDK}" PLATFORM=PLATFORM_WEB -B
