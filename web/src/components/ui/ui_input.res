type types =
  | @as("text")Text
  | @as("number")Number
  | @as("password")Password
  | @as("email")Email
  | @as("search")Search
  | @as("tel")Tel
  | @as("url")Url
  | @as("date")Date
  | @as("time")Time
  | @as("range")Range
  | @as("color")Color
  | @as("file")File
  | @as("hidden")Hidden
  | @as("text")None

@react.component @module("./input")

external make: (
  ~value: string=?,
  ~_type: types=?,
  ~placeholder: string=?,
  ~onChange: ReactEvent.Form.t => unit =?
) => React.element = "Input"

