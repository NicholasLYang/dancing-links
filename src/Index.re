// Entry point

[@bs.val] external document: Js.t({..}) = "document";

// All 4 examples.
ReactDOMRe.render(
  <App />,
  document##getElementById("root")
);