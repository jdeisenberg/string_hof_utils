# ReasonML String Higher Order Function Utilities

This module implements the following functions:

* `stringMap` and `stringKeep`, which apply a function to a string, character by character, returning a new string. 
* `map` and `keep`, which apply a function to a string, character by character, returning an array of `'a`.
* `reduce`, which applies a reducer function to a string, character by character. (All the other functions are defined in terms of `reduce`). I did not see a need for `stringReduce`

See file `src/Examples.re` for examples.

# Build
```
npm run build
```

# Build + Watch

```
npm run start
```

# Editor
If you use `vscode`, Press `Windows + Shift + B` it will build automatically
