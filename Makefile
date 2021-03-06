CC=emcc
CFLAGS=-Weverything

main:
	$(CC) -Weverything  -s STRICT=1\
	                   -s WASM=1\
	                   -s MODULARIZE=1\
	                   -s EXPORT_ES6=1\
	                   -s USE_ES6_IMPORT_META=0\
	                   -s ALLOW_MEMORY_GROWTH=1\
	                   -s NO_EXIT_RUNTIME=1\
	                   -s EXPORTED_FUNCTIONS='["_main","_test", "_solve_sudoku", "_calloc"]'\
	                   -s "EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap', 'ccall']"\
	                   src/main.c -Os -o src/sudoku.js
