# saok2c
---
A single-file tool to convert files into C arrays. Useful for final builds

### Usage:
`saok2c <header_name> <input_file1> [input\_file2] ... [input_filen] -- [output_file]`

&emsp;`header_name`: name of the final header name WITHOUT extension

&emsp;`input_file`(s): names of the input files

&emsp;`output_file`: name of the output file (*optional: default 'stdout'*)

**NOTE:** if you choose to use stdout, you may redirect it to a file while errors keep getting printed to the console by using this:

`saok2c <header_name> <input_file1> [input_file2] ... [input_filen] 1> output_file`

### Building:
Just run *make* (or *mingw32-make* if you're using W\*\*\*ows). Default compiler is gcc. Specify the CC argument to change it (*make CC=compiler*)

## Future plans
For now, it's just a CLI tool, but there'll also be a header file option

- [ ] Virtual filesystem (switch between files and arrays)
- [ ] Support for WASM (since WASM builds can't access files, I wish this solution could simulate that)

