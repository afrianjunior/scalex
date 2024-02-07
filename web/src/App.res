type _unit = {
  label: string,
  value: float
}

type mode = {
  name: string,
  units: array<_unit>
}

let data = [
  {
    name: "Milk",
    units: [
      {
        label: "oz",
        value: 1.3
      },
      {
        label: "ml",
        value: 0.9
      },
      {
        label: "cup",
        value: 3.0
      }
    ]
  },
  {
    name: "Eggs",
    units: [
      {
        label: "egg",
        value: 45.0
      }
    ]
  }
]

let items = data->Belt.Array.map(item => {
  <div key={item.name} className="flex items-center justify-between shadow-zinc-300 shadow rounded px-4 py-2 mb-5">
    <div>
      <span className="font-bold text-zinc-600 block mb-2">
        {item.name->React.string}
      </span>
      <div className="text-xs font-medium">
        {
          React.array(item.units->Belt.Array.map(_unit => {
            <div key={_unit.label} className="mr-2 inline-block">
              <UI.Badge variant={Ui_badge.Outline}>
                <span className="font-light">
                  {_unit.label->React.string}
                  {" : "->React.string}
                  {_unit.value->React.float}
                </span>
              </UI.Badge>
            </div>
          }))
        }
      </div>
    </div>
    <UI.Button>
      <span className="text-2xs">
        {"Configure"->React.string}
      </span>
    </UI.Button>
  </div>
})


@react.component
let make = () => {
  <div className="absolute shadow-xl w-full h-screen">
    <div className="font-plus-jakarta max-w-[450px] m-auto">
      <UI.Meteors number={28} />
      <div className="text-center my-8">
        <span className="text-3xl font-bold block mb-1">
          {"ScaleX"->React.string}
        </span>
        <span className="block text-xs">
          {"a personalize kitchen scale"->React.string}
        </span>
      </div>

      <div className="mb-2">
        <span className="text-xs font-bold text-gray-300 tracking-wider">
          {"MODES"->React.string}
        </span>
      </div>

      <div>
        {React.array(items)}
      </div>

      <UI.Button variant={Ui_button.Destructive} className="w-full border border-dashed py-8">
        {"Add new mode"->React.string}
      </UI.Button>
    </div>

  </div>

}
