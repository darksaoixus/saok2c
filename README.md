# saok2c
---
A single-file tool to convert files into C arrays. Useful for final builds

### Usage:
*saok2c **\<header\_name\> \<input\_file1\>** [input\_file2] ... [input\_filen] -- [outputfile]*

&emsp;**header\_name**: name of the final header name WITHOUT extension

&emsp;**input\_file**(s): names of the input files

&emsp;*outputfile*: name of the output file (*optional: default 'stdout'*)

### Building:
Just run *make* (or *mingw32-make* if you're using W\*\*\*ows). Default compiler is gcc. Specify the CC argument to change it (*make CC=compiler*)

## Future plans
- [ ] Virtual filesystem (switch between files and arrays)
- [ ] Support for WASM (since WASM builds can't access files, I wish this solution could simulate that)

