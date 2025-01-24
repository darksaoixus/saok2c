# saok2c
---
A single-file tool to convert files into C arrays. Useful for final builds

### Usage:
*saok2c **\<header\_name\> \<input\_file1\>** [input\_file2] ... [input\_filen] -- [output\_file]*

&emsp;**header\_name**: name of the final header name WITHOUT extension

&emsp;**input\_file**(s): names of the input files

&emsp;*output\_file*: name of the output file (*optional: default 'stdout'*)

**NOTE:** if you choose to use stdout, you may redirect it to a file while errors keep getting printed to the console by using this:

*saok2c **\<header\_name\> \<input\_file1\>** [input\_file2] ... [input\_filen]* 1\> output\_file

### Building:
Just run *make* (or *mingw32-make* if you're using W\*\*\*ows). Default compiler is gcc. Specify the CC argument to change it (*make CC=compiler*)

## Future plans
For now, it's just a CLI tool, but there'll also be a header file option

- [ ] Virtual filesystem (switch between files and arrays)
- [ ] Support for WASM (since WASM builds can't access files, I wish this solution could simulate that)

