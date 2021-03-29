# Summary of code conventions
- English, comment if needed to understand.
- Full words and omit object (`position` over `pos`, `Update()` over `UpdatePlayer()`).
- `variableAbc`, `FunctionAbc()`, `Class&Struct&EnumAbc`, `ENUM_ELEMENTS_&_MACROS`.
- Variables declared **not** initialized **neither** stacked with `,`.
- Class&Struct: `public` -> `private`, **variables** -> **mainFunctions** -> **utilFunctions**.
- `switch` over multiple `else if ()`.
- `++var` over `var++`.
- Whitespace to improve readability. `if ((A - B) >= 0)` over `if((A-B)>=0)`.

```C++
| WRONG                  | NEITHER          | PERFECT        |
| ---------------------- | ---------------- | -------------- |
|    if (A)              |    if (E) {      |    if (G)      |
|        B();            |        F(); }    |    {           |
|    else if (C) D();    |                  |        H();    |
|                        |                  |    }           |
```
