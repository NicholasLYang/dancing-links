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
 | Invalid(int);


// We only need to check the row/col/box of the new index
let validateBoard = board => {
  let colMaps = Array.make(9, 0);
  let rowMaps = Array.make(9, 0);
  let squareMaps = Array.make(9, 0);
  for (i in 0 to 80) {
    let col = i mod 9;
    let row = i / 9;
    let square = 3 * (row / 3) + col / 3;
    switch (board[i]) {
    | Valid(value) =>
      if (1 lsl value land colMaps[col] != 0) {
        for (i in 0 to 8) {
          switch (board[i * 9 + col]) {
            | Valid(v) => {
              board[i * 9 + col] = Invalid(v);
            }
            | Empty => ()
            | Invalid(_) => ()
          };
        }
      } else {
        colMaps[col] = colMaps[col] lor 1 lsl value;
      };
      if (1 lsl value land rowMaps[row] != 0) {
        for (i in 0 to 8) {
          switch (board[row * 9 + i]) {
            | Valid(v) => {
              board[row * 9 + i] = Invalid(v);
            }
            | Empty => ()
            | Invalid(_) => ()
          }
        }
      } else {
        rowMaps[row] = rowMaps[row] lor 1 lsl value;
      };
      if (1 lsl value land squareMaps[square] != 0) {
        for (i in 0 to 80) {
          let col = i mod 9;
          let row = i / 9;
          if (square == 3 * (row/3) + col/3) {
            switch (board[i]) {
              | Valid(v) => {
                board[i] = Invalid(v)
              }
              | Empty => ()
              | Invalid(_) => ()
            }
          }
        }
      } else {
        squareMaps[square] = squareMaps[square] lor 1 lsl value;
      };
    | Empty => ()
    | Invalid(_) => ()
    };
  };
  board
};

[@react.component]
let make = () => {
  let (board, setBoard) = React.useState(() => Array.make(81, Empty));
  let handleChange = (i, event) => {
    ReactEvent.Form.persist(event);
    setBoard(board => {
      board[i] =
        ReactEvent.Form.target(event)##value
        ->int_of_string_opt
        ->Belt.Option.mapWithDefault(Empty, i => i < 10 && i > 0 ? Valid(i) : Empty);
       validateBoard(board)
    });
  };
  <div className=Styles.app>
    <h1> {React.string("Sudoku")} </h1>
    <div className={Styles.board}>
      {React.array(
         Array.mapi(
           (i, value) =>
             switch (value) {
               | Invalid(value) => <Tile handleChange=handleChange(i) isValid=false value=string_of_int(value) />
               | Valid(value) => <Tile handleChange=handleChange(i) isValid=true value=string_of_int(value) />
               | Empty => <Tile handleChange=handleChange(i) isValid=true value="" />
             },
           board,
         ),
       )}
    </div>
  </div>;
};
