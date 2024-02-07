type variant = 
  | @as("outline")Outline
  | @as("destructive")Destructive
  | @as("secondary")Secondary
  | @as("default")None

@react.component @module("./badge")

external make: (
  ~children: React.element=?,
  ~variant: variant=?,
  ~className: string=?
) => React.element = "Badge"



