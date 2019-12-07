/* Here are some examples of the string higher-order functions. */

let addDash = (s: string): string => s ++ "-";

let result = StringHof.stringMap({js|aßcд😺你|js}, addDash);
Js.log(result);

let toCode = (s: string): int => {
  switch (Js.String.codePointAt(0, s)) {
    | Some(n) => n
    | None => 0
  }
};

let result2 = StringHof.map({js|aßcд😺你|js}, toCode);
Js.log(result2);

let nonVowel = (s: string): bool =>
  !(s == "a" || s == "e" || s == "i" || s == "o" || s == "u");

let result3 = StringHof.stringKeep("cauliflower", nonVowel);
Js.log(result3);

let result4 = StringHof.keep("cauliflower", nonVowel);
Js.log(result4);

let reverser = (acc: string, item: string): string => item ++ acc;

let result5 = StringHof.reduce({js|aßcд😺你|js}, "", reverser);
Js.log(result5);

let addCharCodes = (total: float, s: string): float =>
  total +. Js.String.charCodeAt(0, s);

let result6 = StringHof.reduce("abc", 0.0, addCharCodes);
Js.log(result6);
