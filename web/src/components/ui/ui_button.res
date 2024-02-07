type variant = 
  | @as("solid")Solid
  | @as("outline")Outline
  | @as("destructive")Destructive
  | @as("link")Link
  | @as("ghost")Ghost
  | @as("secondary")Secondary
  | @as("default")None

type size =
  | @as("sm")Small
  | @as("lg")Large
  | @as("icon")Icon
  | @as("default")None

@react.component @module("./button")

external make: (
  ~children: React.element=?,
  ~className: string=?,
  ~variant: variant=?,
  ~size: size=?,
  ~onClick: ReactEvent.Mouse.t => unit =?,
  ~onFocus: ReactEvent.Mouse.t => unit =?
) => React.element = "Button"


