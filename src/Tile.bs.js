'use strict';

var Css = require("bs-css-emotion/src/Css.js");
var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");

function tile(isValid) {
  return Curry._1(Css.style, {
              hd: Css.width(Css.px(50)),
              tl: {
                hd: Css.height(Css.px(50)),
                tl: {
                  hd: Css.border(Css.px(1), Css.solid, isValid ? Css.black : Css.red),
                  tl: {
                    hd: Css.boxSizing(Css.borderBox),
                    tl: {
                      hd: Css.fontSize(Css.px(24)),
                      tl: {
                        hd: Css.textAlign(Css.center),
                        tl: {
                          hd: Css.focus({
                                hd: Css.border(Css.px(1), Css.solid, Css.lightblue),
                                tl: {
                                  hd: Css.outline(Css.px(0), "none", Css.black),
                                  tl: /* [] */0
                                }
                              }),
                          tl: /* [] */0
                        }
                      }
                    }
                  }
                }
              }
            });
}

var Styles = {
  tile: tile
};

function Tile(Props) {
  var value = Props.value;
  var isValid = Props.isValid;
  var handleChange = Props.handleChange;
  return React.createElement("input", {
              key: value,
              className: tile(isValid),
              type: "text",
              value: value,
              onChange: handleChange
            });
}

var make = Tile;

exports.Styles = Styles;
exports.make = make;
/* Css Not a pure module */
