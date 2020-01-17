
/**
  [reduce str f] takes a string argument, initial value, and a reducer
  function [f] whose arguments are an accumulated value and a character from the string.

  [reduce] starts the accumulated value as the initial value, and updates it by
  applying the reducer function [f] to each character in the string.

  @example {[
    let addCharCodes = (total: int, s: string): int =>
      total +. Char.code(String.get(0));
    reduce("abc", 0, addCharCodes) == 294

    let reverser = (acc: string, item: string): string => item ++ acc;
    reduce("abcde", "", reverser) == "edcba"
  ]}
*/

/* 
  Returns tuple containing unicode character at position n
  and length in UTF-32 units (1, 2, 3, or 4). If invalid, presume
  it consumes one UTF-32 unit.

  This function only checks the first byte; it does not check that the
  subsequent bytes start with bits 10 (or that there are really enough).

  Bytes  Bits  Bit patterns
  1       7     0xxxxxxx
  2      11     110xxxxx  10xxxxxx
  3      16     1110xxxx  10xxxxxx  10xxxxxx
  4      21     11110xxx  10xxxxxx  10xxxxxx  10xxxxxx
*/
let unicodeCharAt = (s: string, n: int): (string, int) => {
  let byteCode = Char.code(String.get(s, n));
  let nMore = switch (byteCode) {
    | n when (n < 128) => 0
    | n when (n land 0xe0 == 0xc0) => 1
    | n when (n land 0xf0 == 0xe0) => 2
    | n when (n land 0xf8 == 0xf0) => 3
    | _ => -1
  };
  if (n >= 0) {
    (String.sub(s, n, nMore + 1), nMore + 1);
  } else {
    ("", 1);
  };
};

let reduce = (s: string, acc: 'a, f: ('a, string) => 'a): 'a => {
  let rec helper = (acc: 'a, index: int) =>
    switch (index) {
    | n when n >= String.length(s) => acc
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
  reduce(s, [||], (acc, item) => Array.append(acc, [|f(item)|]));

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
      Array.append(acc, [|item|]);
    } else {
      acc;
    }
  );


/**
  [isvalidUTF8 s] returns [true] if the input string is a valid UTF8 string,
  [false] otherwise.

  Because `reduce()` doesn't do thorough checking if the string is valid, you might
  want to run them through this function first.
*/

let isValidUTF8 = (s: string): bool => {

  let rec validate = (index: int, nFollowing: int) => {
    if (index == String.length(s)) { // at end of string, you should not need any more bytes
      nFollowing == 0;
    } else {
      let byteCode = Char.code(String.get(s, index));
      if (nFollowing == 0) { // if at beginning of a set of bytes,
        let needed = switch (byteCode) { // find out how many more are needed
          | n when (n < 128) => 0
          | n when (n land 0xe0 == 0xc0) => 1
          | n when (n land 0xf0 == 0xe0) => 2
          | n when (n land 0xf8 == 0xf0) => 3
          | _ => -1
        };
        if (needed >= 0) { 
          validate(index + 1, needed);
        } else {
          false;  // first char in sequence has bad bit pattern
        }
      }  else { 
        if (byteCode land 0xc0 == 0x80) {
          validate(index + 1, nFollowing - 1);
        } else {
          false; // part of sequence following first byte is invalid
        }
      }
    }
  };

  validate(0, 0);
};

