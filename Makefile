CC=clang
CFLAGS=-Weverything

main:
	$(CC) $(CFLAGS) src/main.c -g -o links
wasm:
	emcc -Weverything  -s STRICT=1 -s WASM=1 -s MODULARIZE=1 -s EXPORT_ES6=1 -s ALLOW_MEMORY_GROWTH=1 src/main.c -Os -o src/sudoku.js
