# BrainF>>k V1.3.3
Fully functional BF and BF-equivalent interpreter

###Build:
Run `make` in the top-level directory.

###Usage:
`bf [-help] [options] file`

###Specifics

| Options             |                                                                                       |
|:-------------------:|-------------------------------------------------------------------------------------- |
| `-d`                | Enable debugging and diagnostics                                                      |
| `-h`                | Print help text                                                                       |
| `-L <language>`     | Sets the language to interpret (BF is default)                                        |
| `-P <print mode>`   | Sets the print mode for output of the language (char is default)                      |
| `-T <lang1> <lang2>`| Translates the file given from `<lang1>` to `<lang2>`                                 |

| Languages         |                                                                                         |
|:-----------------:|---------------------------------------------------------------------------------------  |
| `BF`              |   [Brainf**k](https://esolangs.org/wiki/Brainfuck) (default)                            |
| `???`             |   [???](https://esolangs.org/wiki/%3F%3F%3F)                                            |
| `TinyBF`          |   [TinyBF](https://esolangs.org/wiki/TinyBF)                                            |

| Print Modes       |                                                                                         |
|:-----------------:|---------------------------------------------------------------------------------------  |
| `char`            | Print each output byte as a character (default)                                         |
| `num`             | Print each output byte as space-separated numerical values                              |
| `hex`             | Print each output byte as space-separated hexadecimal values                            |
| `smart`           | If the byte is printable ASCII, print as a character. Otherwise, as hexadecimal.        |
