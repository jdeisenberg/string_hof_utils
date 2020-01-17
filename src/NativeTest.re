// Test strings

let allAscii = "abcde";
let simpleUni = "áβçð";
let complexUni = "aβçд😺你"; // contains 1, 2, 3, and 4-byte Unicode
let french = "Français";

/*
  Utility routine for displaying an array.
  First parameter is the array to display,
  Second parameter is a function that tells how to convet an item to string
*/
let displayArray = (arr: array('a), stringify: ('a) => string): string => {
  let rec joiner = (accumulator: string, index:int): string => {
    if (index == Array.length(arr) - 1) {
      accumulator ++ stringify(arr[index])
    } else {
      accumulator ++ stringify(arr[index]) ++ ", " ++ joiner(accumulator, index + 1);
    }
  };
  "[" ++ joiner("", 0) ++ "]"
};

//============= Test stringMap()

let addDash = (s: string): string => {
  s ++ "-";
};

let dashed = UTF8Hof.stringMap(complexUni, addDash);
let () = print_endline("Map dashes into " ++ complexUni ++ ": " ++ dashed);

//============= Test map()

let asciiCode = (s: string): int => {
  Char.code(String.get(s, 0));
};

let codes = UTF8Hof.map(simpleUni, asciiCode);
print_endline("ASCII values of " ++ allAscii ++ ": " ++ displayArray(codes, string_of_int));

//============= Test stringKeep() and keep() 

let nonVowel = (s: string): bool => {
  !(s == "a" || s == "e" || s == "i" || s == "o" || s == "u");
};

print_endline("Keep non-vowels in " ++ french ++ ": " ++
  UTF8Hof.stringKeep(french, nonVowel));
print_endline("Array of non-vowels in " ++ french ++ ": " ++
  displayArray(UTF8Hof.keep(french, nonVowel), (x)=>x));

//============= Test reduce()

let addCharCodes = (total: int, s: string): int =>
  total + Char.code(String.get(s, 0));

let sum = UTF8Hof.reduce(allAscii, 0, addCharCodes);
print_endline("Sum of char codes in " ++ allAscii ++ ": " ++ string_of_int(sum));

//============= Test isValidUTF8()

print_endline(complexUni ++ " valid? " ++ string_of_bool(UTF8Hof.isValidUTF8(complexUni)));

// insufficient number of bytes to be valid
let badUni1 = String.sub(complexUni, 0, String.length(complexUni) - 2);
print_endline(badUni1 ++ " valid? " ++ string_of_bool(UTF8Hof.isValidUTF8(badUni1)));

// change character that should begin with bits 10xx
let badUni2 = {
  let bytes = Bytes.of_string(french);
  Bytes.set(bytes, 5, 'x');
  Bytes.to_string(bytes);
};
print_endline(badUni2 ++ " valid? " ++ string_of_bool(UTF8Hof.isValidUTF8(badUni2)));


