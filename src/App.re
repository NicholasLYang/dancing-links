[@bs.module "./sudokuLoader"] external solve: array(int) => unit = "default";

module Styles = {
  open Css;
  let app =
    style([
      display(`flex),
      flexDirection(column),
      alignItems(center),
      width(vw(100.0)),
      fontFamily(`custom("Source Sans Pro")),
    ]);
  let board =
    style([
      display(`flex),
      width(px(450)),
      flexWrap(wrap),
      border(px(1), solid, black),
    ]);
};

type tile =
 | Empty
 | Valid(int)
 | Invalid(option(int));

let checkEntry = (mapArray, index, value) => {
  switch (mapArray[index]) {
          | Some(map) => {
            if (1 lsl value land map != 0) {
              mapArray[index] = None;
            } else {
              mapArray[index] = Some(map lor 1 lsl value);
            };
          }
          | None => ()
        }
};

let makeInvalid = tile => {
 switch (tile) {
   | Empty => Invalid(None)
   | Valid(i) => Invalid(Some(i))
   | Invalid(_) => tile
 }
}

let makeValid = tile => {
 switch (tile) {
   | Empty => Empty
   | Valid(i) => Valid(i)
   | Invalid(i) => Belt.Option.mapWithDefault(i, Empty, i => Valid(i))
 }
}

// We only need to check the row/col/box of the new index
let validateBoard = (board: array(tile)) => {
  let colMaps = Array.make(9, Some(0));
  let rowMaps = Array.make(9, Some(0));
  let squareMaps = Array.make(9, Some(0));
  for (i in 0 to 80) {
    let col = i mod 9;
    let row = i / 9;
    let square = 3 * (row / 3) + col / 3;
    switch (board[i]) {
    | Valid(value) => {
      checkEntry(colMaps, col, value);
      checkEntry(rowMaps, row, value);
      checkEntry(squareMaps, square, value);
    }
    | Empty => ()
    | Invalid(_) => ()
    };
  };
  for (i in 0 to 80) {
    let col = i mod 9;
    let row = i / 9;
    let square = 3 * (row / 3) + col / 3;
    let isValid = Belt.Option.isSome(colMaps[col])
      && Belt.Option.isSome(rowMaps[row])
      && Belt.Option.isSome(squareMaps[square]);
    if (isValid) {
        board[i] = makeValid(board[i]);
    } else {
        board[i] = makeInvalid(board[i]);
    }
  }
  board
};

[@react.component]
let make = () => {
  React.useEffect1(() => {
     solve([|1|])
     None
  }, [||])
  let (board, setBoard) = React.useState(() => Array.make(81, Empty));
  let handleChange = (i, event) => {
    ReactEvent.Form.persist(event);
    setBoard(board => {
      board[i] =
        ReactEvent.Form.target(event)##value
        ->int_of_string_opt
        ->Belt.Option.mapWithDefault(Empty, i => i < 10 && i > 0 ? Valid(i) : Empty);
       Array.map(i => i, validateBoard(board))
    });
  };
  <div className=Styles.app>
    <h1> {React.string("Sudoku")} </h1>
    <div className={Styles.board}>
      {React.array(
         Array.mapi(
           (i, value) =>
             switch (value) {
               | Invalid(value) => <Tile key=string_of_int(i) handleChange=handleChange(i) isValid=false value=Belt.Option.mapWithDefault(value, "", string_of_int) />
               | Valid(value) => <Tile key=string_of_int(i) handleChange=handleChange(i) isValid=true value=string_of_int(value) />
               | Empty => <Tile key=string_of_int(i) handleChange=handleChange(i) isValid=true value="" />
             },
           board,
         ),
       )}
    </div>
  </div>;
};
