/*
** Copyright © 2025 darksaoixus
**
** Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
** (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
** publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
** so, subject to the following conditions:
** 
** The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
** 
** THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
** FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SAOK2C_MEM_CAP 8 * 1024

static char   SAOK2C_MEM[SAOK2C_MEM_CAP] = {0};
static size_t SAOK2C_mem_count = 0;

static void* SAOK2C_malloc(size_t size);

char*  upper(char *s);
size_t get_file_size(FILE *file);
void   parse_file(char *input_name, FILE *output_fd);

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <header_name> <input_file1> [input_file2] ... [input_filen] -- [outputfile]\n", argv[0]);
    printf("\theader_name: name of the final header name WITHOUT extension\n");
    printf("\tinput_file(s): names of the input files\n");
    printf("\toutputfile: name of the output file (optional: default 'stdout')\n");
    return 0;
  }

  int output_index = -1;
  for (int i = 3; i < argc; ++i) {
    if (strcmp(argv[i], "--") == 0 && i + 1 < argc) {
      output_index = i + 1;
      break;
    }
  }

  FILE *output_fd = stdout;
  if (output_index > 0) {
    char *output_name = argv[output_index];
    FILE *file = fopen(output_name, "w");

    if (file == NULL) {
      fprintf(stderr, "It was not possible to open %s. Fallbacking to stdout...", output_name);
    } else {
      output_fd = file;
    }
  }

  char *header_upper = upper(argv[1]);

  fprintf(output_fd, "#ifndef %s_H_\n", header_upper);
  fprintf(output_fd, "#define %s_H_\n", header_upper);

  fprintf(output_fd, "\n");

  if (output_index > 0) {
    for (int input_index = 2; input_index < output_index - 1; ++input_index) {
      parse_file(argv[input_index], output_fd);
    }
  } else {
    for (int input_index = 2; input_index < argc; ++input_index) {
      parse_file(argv[input_index], output_fd);
    }
  }

  fprintf(output_fd, "#endif // %s_H_\n", header_upper);


  fclose(output_fd);
  return 0;
}

static void* SAOK2C_malloc(size_t size) {
  if (SAOK2C_mem_count + size >= SAOK2C_MEM_CAP) {
    fprintf(stderr, "You ran out of memory. How could use use so much memory, huh?!\n");
    exit(1);
  }

  void *ptr = SAOK2C_MEM + SAOK2C_mem_count;
  SAOK2C_mem_count += size;
  return ptr;
}

char* upper(char *s) {
  size_t s_len = strlen(s);

  char *out = (char *)SAOK2C_malloc(s_len + 1);

  for (int i = 0; i < s_len; ++i) {
    if (s[i] >= 'a' && s[i] <= 'z') {
      out[i] = s[i] - 'a' + 'A';
    } else {
      out[i] = s[i];
    }
  }

  out[s_len] = '\0';
  return out;
} 

size_t get_file_size(FILE *file) {
  fseek(file, 0L, SEEK_END);
  size_t input_size = ftell(file);
  rewind(file);

  return input_size;
}

void parse_file(char *input_name, FILE *output_fd) {
  FILE *input_file = fopen(input_name, "r");
  if (input_file == NULL) {
    perror("Error opening input file");
    return;
  }

  size_t input_name_size = strlen(input_name);
  size_t const_name_size = 0;

  while (const_name_size < input_name_size && input_name[const_name_size] != '.') {
    const_name_size++;
  }

  char *const_name = (char *)SAOK2C_malloc(const_name_size + 1);
  strncpy(const_name, input_name, const_name_size);
  const_name[const_name_size] = '\0';

  char *const_name_upper = upper(const_name);

  size_t input_size = get_file_size(input_file);

  fprintf(output_fd, "#define %s_SIZE %zu\n\n", const_name_upper, input_size);
  fprintf(output_fd, "const char %s[%s_SIZE] = {\n", const_name, const_name_upper);

  unsigned char line_buffer[16] = {0};
  size_t n = fread(line_buffer, 1, 16, input_file);

  while (n != 0) {
    fprintf(output_fd, "  ");
    for (size_t i = 0; i < n; ++i) {
      fprintf(output_fd, "'\\x%02x', ", line_buffer[i]);
    }
    n = fread(line_buffer, 1, 16, input_file);
    fprintf(output_fd, "\n");
  }

  fprintf(output_fd, "};\n");
  fprintf(output_fd, "\n");

  fclose(input_file);
}

