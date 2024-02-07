/* Importing required modules */
module Clsx = {
  @module("clsx") external clsx: 'a => string = "clsx";
};

module TailwindMerge = {
  @module("tailwind-merge") external twMerge: 'a => string = "twMerge";
};

/* Function definition */
let cn = (inputs: array<string>) => {
  TailwindMerge.twMerge(Clsx.clsx(inputs));
};

