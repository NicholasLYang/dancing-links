'use strict';

var Css = require("bs-css-emotion/src/Css.js");
var $$Array = require("bs-platform/lib/js/array.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Caml_array = require("bs-platform/lib/js/caml_array.js");
var Pervasives = require("bs-platform/lib/js/pervasives.js");
var Belt_Option = require("bs-platform/lib/js/belt_Option.js");
var Tile$DancingLinks = require("./Tile.bs.js");

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
      if (((1 << value$1) & Caml_array.caml_array_get(colMaps, col)) !== 0) {
        for(var i$1 = 0; i$1 <= 8; ++i$1){
          var v = Caml_array.caml_array_get(board, Math.imul(i$1, 9) + col | 0);
          if (typeof v !== "number" && !v.TAG) {
            Caml_array.caml_array_set(board, Math.imul(i$1, 9) + col | 0, {
                  TAG: /* Invalid */1,
                  _0: v._0
                });
          }
          
        }
      } else {
        Caml_array.caml_array_set(colMaps, col, Caml_array.caml_array_get(colMaps, col) | (1 << value$1));
      }
      if (((1 << value$1) & Caml_array.caml_array_get(rowMaps, row)) !== 0) {
        for(var i$2 = 0; i$2 <= 8; ++i$2){
          var v$1 = Caml_array.caml_array_get(board, Math.imul(row, 9) + i$2 | 0);
          if (typeof v$1 !== "number" && !v$1.TAG) {
            Caml_array.caml_array_set(board, Math.imul(row, 9) + i$2 | 0, {
                  TAG: /* Invalid */1,
                  _0: v$1._0
                });
          }
          
        }
      } else {
        Caml_array.caml_array_set(rowMaps, row, Caml_array.caml_array_get(rowMaps, row) | (1 << value$1));
      }
      if (((1 << value$1) & Caml_array.caml_array_get(squareMaps, square)) !== 0) {
        for(var i$3 = 0; i$3 <= 80; ++i$3){
          var col$1 = i$3 % 9;
          var row$1 = i$3 / 9 | 0;
          if (square === (Math.imul(3, row$1 / 3 | 0) + (col$1 / 3 | 0) | 0)) {
            var v$2 = Caml_array.caml_array_get(board, i$3);
            if (typeof v$2 !== "number" && !v$2.TAG) {
              Caml_array.caml_array_set(board, i$3, {
                    TAG: /* Invalid */1,
                    _0: v$2._0
                  });
            }
            
          }
          
        }
      } else {
        Caml_array.caml_array_set(squareMaps, square, Caml_array.caml_array_get(squareMaps, square) | (1 << value$1));
      }
    }
    
  }
  return board;
}

function App(Props) {
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
                  return validateBoard(board);
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
                                      value: String(value._0),
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

exports.Styles = Styles;
exports.validateBoard = validateBoard;
exports.make = make;
/* app Not a pure module */
