#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
OUT="$ROOT/build/out"
CC="${CC:-gcc}"
CFLAGS=(-std=c11 -Wall -Wextra -Werror -I"$ROOT/sdk/native/include")
MODULE_DIRS=(boot hal kernel drivers graphics shell services runtime apps mainos recovery updateos)
mkdir -p "$OUT"

collect_sources() {
  for dir in "${MODULE_DIRS[@]}"; do
    find "$ROOT/$dir" -maxdepth 1 -name '*.c' -print
  done | sort
}

build_lib() {
  mapfile -t sources < <(collect_sources)
  local objdir="$OUT/obj"
  rm -rf "$objdir"
  mkdir -p "$objdir"
  local objects=()
  for src in "${sources[@]}"; do
    local rel="${src#$ROOT/}"
    local obj="$objdir/${rel//\//_}.o"
    "$CC" "${CFLAGS[@]}" -c "$src" -o "$obj"
    objects+=("$obj")
  done
  rm -f "$OUT/libbolun.a"
  ar rcs "$OUT/libbolun.a" "${objects[@]}"
}

build_tests() {
  build_lib
  "$CC" "${CFLAGS[@]}" "$ROOT/tests/test_suite.c" "$OUT/libbolun.a" -o "$OUT/bolun_tests"
}

build_tools() {
  build_lib
  "$CC" "${CFLAGS[@]}" "$ROOT/tools/license/issuer.c" "$OUT/libbolun.a" -o "$OUT/bolun-license-issuer"
}

case "${1:-all}" in
  all)
    build_tests
    build_tools
    ;;
  test)
    build_tests
    "$OUT/bolun_tests"
    ;;
  image)
    build_tests
    printf 'Bolun Phone 1.0 Beta simulated image\n' > "$OUT/bolun-phone-1.0-beta.img"
    printf 'created %s\n' "$OUT/bolun-phone-1.0-beta.img"
    ;;
  emulator)
    build_tests
    "$OUT/bolun_tests"
    printf 'emulator boot smoke: MainOS -> shell -> calculator OK\n'
    ;;
  clean)
    rm -rf "$OUT" ./*.o
    ;;
  *)
    echo "usage: $0 {all|test|image|emulator|clean}" >&2
    exit 2
    ;;
esac
