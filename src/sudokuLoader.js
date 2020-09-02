const module = await import("./sudoku");
const wasmModule = module.default();
function solve(board) {
  wasmModule.then(module => {
    const typedArray = new Uint8Array(81);
    board.forEach((item, i) => {
      typedArray[i] = item;
    })
    const buf = module._calloc(81);
    module.HEAPU8.set(typedArray, buf);
    module.ccall("solve_sudoku", "string", ["string"], [buf]);
  })
}
export default solve;
