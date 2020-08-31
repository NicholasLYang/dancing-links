module Styles = {
  open Css;
    let tile = isValid =>
      style([
        width(px(50)),
        height(px(50)),
        border(px(1), solid, isValid ? black : red),
        boxSizing(borderBox),
        fontSize(px(24)),
        textAlign(center),
        focus([
          border(px(1), solid, lightblue),
          outline(px(0), `none, black),
        ]),
      ]);
}

[@react.component]
let make = (~value, ~isValid, ~handleChange) => {
 <input
               key={value}
               type_="text"
               onChange={handleChange}
               className={Styles.tile(isValid)}
               value={value}
             />
}