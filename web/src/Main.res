%%raw("import './tailwind.css'")
%%raw("import '@fontsource/plus-jakarta-sans/300.css'")
%%raw("import '@fontsource/plus-jakarta-sans/400.css'")
%%raw("import '@fontsource/plus-jakarta-sans/700.css'")

ReactDOM.querySelector("#root")
->Option.getExn
->ReactDOM.Client.createRoot
->ReactDOM.Client.Root.render(
  <React.StrictMode>
    <App />
  </React.StrictMode>,
)
