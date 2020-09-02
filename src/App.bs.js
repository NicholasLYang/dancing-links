

import * as Css from "bs-css-emotion/src/Css.js";
import * as $$Array from "bs-platform/lib/es6/array.js";
import * as Curry from "bs-platform/lib/es6/curry.js";
import * as React from "react";
import * as Caml_array from "bs-platform/lib/es6/caml_array.js";
import * as Pervasives from "bs-platform/lib/es6/pervasives.js";
import * as Belt_Option from "bs-platform/lib/es6/belt_Option.js";
import * as SudokuLoader from "./sudokuLoader";
import * as Tile$DancingLinks from "./Tile.bs.js";

var factory = SudokuLoader.default;

var app = Curry._1(Css.style, {
      hd: Css.display("flex"),
      tl: {
        hd: Css.flexDirection(Css.column),
        tl: {
          hd: Css.alignItems(Css.center),
          tl: {
            hd: Css.width(Css.vw(100.0)),
            tl: {
              hd: Css.fontFamily({
                    NAME: "custom",
                    VAL: "Source Sans Pro"
                  }),
              tl: /* [] */0
            }
          }
        }
      }
    });

var board = Curry._1(Css.style, {
      hd: Css.display("flex"),
      tl: {
        hd: Css.width(Css.px(450)),
        tl: {
          hd: Css.flexWrap(Css.wrap),
          tl: {
            hd: Css.border(Css.px(1), Css.solid, Css.black),
            tl: /* [] */0
          }
        }
      }
    });

var Styles = {
  app: app,
  board: board
};

function checkEntry(mapArray, index, value) {
  var map = Caml_array.caml_array_get(mapArray, index);
  if (map !== undefined) {
    if (((1 << value) & map) !== 0) {
      return Caml_array.caml_array_set(mapArray, index, undefined);
    } else {
      return Caml_array.caml_array_set(mapArray, index, map | (1 << value));
    }
  }
  
}

function makeInvalid(tile) {
  if (typeof tile === "number") {
    return {
            TAG: /* Invalid */1,
            _0: undefined
          };
  } else if (tile.TAG) {
    return tile;
  } else {
    return {
            TAG: /* Invalid */1,
            _0: tile._0
          };
  }
}

function makeValid(tile) {
  if (typeof tile === "number") {
    return /* Empty */0;
  } else if (tile.TAG) {
    return Belt_Option.mapWithDefault(tile._0, /* Empty */0, (function (i) {
                  return {
                          TAG: /* Valid */0,
                          _0: i
                        };
                }));
  } else {
    return {
            TAG: /* Valid */0,
            _0: tile._0
          };
  }
}

function validateBoard(board) {
  var colMaps = Caml_array.caml_make_vect(9, 0);
  var rowMaps = Caml_array.caml_make_vect(9, 0);
  var squareMaps = Caml_array.caml_make_vect(9, 0);
  for(var i = 0; i <= 80; ++i){
    var col = i % 9;
    var row = i / 9 | 0;
    var square = Math.imul(3, row / 3 | 0) + (col / 3 | 0) | 0;
    var value = Caml_array.caml_array_get(board, i);
    if (typeof value !== "number" && !value.TAG) {
      var value$1 = value._0;
      checkEntry(colMaps, col, value$1);
      checkEntry(rowMaps, row, value$1);
      checkEntry(squareMaps, square, value$1);
    }
    
  }
  for(var i$1 = 0; i$1 <= 80; ++i$1){
    var col$1 = i$1 % 9;
    var row$1 = i$1 / 9 | 0;
    var square$1 = Math.imul(3, row$1 / 3 | 0) + (col$1 / 3 | 0) | 0;
    var isValid = Belt_Option.isSome(Caml_array.caml_array_get(colMaps, col$1)) && Belt_Option.isSome(Caml_array.caml_array_get(rowMaps, row$1)) && Belt_Option.isSome(Caml_array.caml_array_get(squareMaps, square$1));
    if (isValid) {
      Caml_array.caml_array_set(board, i$1, makeValid(Caml_array.caml_array_get(board, i$1)));
    } else {
      Caml_array.caml_array_set(board, i$1, makeInvalid(Caml_array.caml_array_get(board, i$1)));
    }
  }
  return board;
}

function App(Props) {
  React.useEffect((function () {
          factory.then(function (res) {
                console.log(Curry._1(res.default, undefined));
                return Promise.resolve(0);
              });
          
        }), []);
  var match = React.useState(function () {
        return Caml_array.caml_make_vect(81, /* Empty */0);
      });
  var setBoard = match[1];
  var handleChange = function (i, $$event) {
    $$event.persist();
    return Curry._1(setBoard, (function (board) {
                  Caml_array.caml_array_set(board, i, Belt_Option.mapWithDefault(Pervasives.int_of_string_opt($$event.target.value), /* Empty */0, (function (i) {
                              if (i < 10 && i > 0) {
                                return {
                                        TAG: /* Valid */0,
                                        _0: i
                                      };
                              } else {
                                return /* Empty */0;
                              }
                            })));
                  return $$Array.map((function (i) {
                                return i;
                              }), validateBoard(board));
                }));
  };
  return React.createElement("div", {
              className: app
            }, React.createElement("h1", undefined, "Sudoku"), React.createElement("div", {
                  className: board
                }, $$Array.mapi((function (i, value) {
                        if (typeof value === "number") {
                          return React.createElement(Tile$DancingLinks.make, {
                                      value: "",
                                      isValid: true,
                                      handleChange: (function (param) {
                                          return handleChange(i, param);
                                        })
                                    });
                        } else if (value.TAG) {
                          return React.createElement(Tile$DancingLinks.make, {
                                      value: Belt_Option.mapWithDefault(value._0, "", (function (prim) {
                                              return String(prim);
                                            })),
                                      isValid: false,
                                      handleChange: (function (param) {
                                          return handleChange(i, param);
                                        })
                                    });
                        } else {
                          return React.createElement(Tile$DancingLinks.make, {
                                      value: String(value._0),
                                      isValid: true,
                                      handleChange: (function (param) {
                                          return handleChange(i, param);
                                        })
                                    });
                        }
                      }), match[0])));
}

var make = App;

export {
  factory ,
  Styles ,
  checkEntry ,
  makeInvalid ,
  makeValid ,
  validateBoard ,
  make ,
  
}
/* factory Not a pure module */
