
/**
  [reduce str f] takes a string argument, initial value, and a reducer
  function [f] whose arguments are an accumulated value and a character from the string.

  [reduce] starts the accumulated value as the initial value, and updates it by
  applying the reducer function [f] to each character in the string.

  @example {[
    let addCharCodes = (total: float, s: string): float =>
      total +. Js.String.charCodeAt(0, s);
    reduce("abc", 0.0, addCharCodes) == 294.0

    let reverser = (acc: string, item: string): string => item ++ acc;
    reduce("abcde", "", reverser) == "edcba"
  ]}
*/

/* 
  Returns tuple containing unicode character at position n
  and length in UTF-16 units (1 or 2). If invalid, presume
  it consumes one UTF-16 unit.
*/
let unicodeCharAt = (s: string, n: int): (string, int) => {
  let cPoint = Js.String.codePointAt(n, s);
  switch (cPoint) {
    | Some(point) => (Js.String.fromCodePoint(point), (point > 65535) ? 2 : 1)
    | None => ("", 1)
  }
};

let reduce = (s: string, acc: 'a, f: ('a, string) => 'a): 'a => {
  let rec helper = (acc: 'a, index: int) =>
    switch (index) {
    | n when n >= Js.String.length(s) => acc
    | n => {
        let (theChar, nBytes) = unicodeCharAt(s, n);
        helper(f(acc, theChar), n + nBytes);
      }
    };

  helper(acc, 0);
};

/**
  [stringMap s f] applies a function [f] to each character in the given string [s],
  returning a new string with the concatenated results of the function calls.

  @example {[
    let addDash = (s: string): string => s ++ "-";
    stringMap("abcde", addDash) == "a-b-c-d-e-";
  ]}
*/

let stringMap = (s: string, f: string => string): string =>
  reduce(s, "", (acc, item) => acc ++ f(item));

/**
  [map s f] applies a function [f] to each character in the given string [s],
  returning an array containing the results of the function calls.

  @example {[
    let toCode = (s: string): float => Js.String.codePointAt(0, s);
    map("abcde", toCode) == [|97.0, 98.0, 99.0, 100.0, 101.0|];
  ]}
*/

let map = (s: string, f: string => 'a): array('a) =>
  reduce(s, [||], (acc, item) => Belt.Array.concat(acc, [|f(item)|]));

/**
  [stringKeep s f] applies a function [f] to each character in the given string [s].
  It returns a new string containing only the characters for which [f] returned [true].

  @example {[
    let nonVowel = (s: string): bool =>
      !(s == "a" || s == "e" || s == "i" || s == "o" || s == "u");

    stringKeep("cauliflower", nonVowel) == "clflwr";
  ]}
*/

let stringKeep = (s: string, f: string => bool): string =>
  reduce(s, "", (acc, item) =>
    if (f(item)) {
      acc ++ item;
    } else {
      acc;
    }
  );

/**
  [keep s f] applies a function [f] to each character in the given string [s].
  It returns an array containing only the characters for which [f] returned [true].

  @example {[
    let nonVowel = (s: string): bool =>
      !(s == "a" || s == "e" || s == "i" || s == "o" || s == "u");

    keep("cauliflower", nonVowel) == [|"c", "l", "f", "l", "w", "r"|];
  ]}
*/

let keep = (s: string, f: string => bool): array('a) =>
  reduce(s, [||], (acc, item) =>
    if (f(item)) {
      Belt.Array.concat(acc, [|item|]);
    } else {
      acc;
    }
  );

