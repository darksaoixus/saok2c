#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SAOK2C_MEM_CAP 8 * 1024
#define SAOK2C_MAX_OPEN_FILES 50

typedef struct {
  FILE *handle;
  size_t size;
  char *name;
  char *upper_name;
} SAOK2C_File;

static char SAOK2C_MEM[SAOK2C_MEM_CAP] = {0};
static size_t SAOK2C_mem_count = 0;

static void* SAOK2C_alloc(size_t size);

SAOK2C_File files[SAOK2C_MAX_OPEN_FILES] = {0};
size_t file_count = 0;

void files_append(FILE *file, size_t size, char *name, char *upper_name);

char* upper(char *s);
void parse_size(const char *input_name);
void parse_file(SAOK2C_File *file, FILE *output_fd);

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Usage: %s <header_name> <input_file1> [input_file2] ... [input_filen] -- [outputfile]\n", argv[0]);
    printf("\theader_name: name of the final header name WITHOUT extension\n");
    printf("\tinput_file(s): names of the input files\n");
    printf("\toutputfile: name of the output file (optional: default 'stdout')\n");
    return 0;
  }

  int input_index = 2;
  for (; input_index < argc && strcmp(argv[input_index], "--") != 0; ++input_index) {
    parse_size(argv[input_index]);
  }

  char *header_upper = upper(argv[1]);

  FILE *output_fd = stdout;
  if (input_index + 2 < argc) {
    char *output_name = argv[input_index + 2];
    FILE *file = fopen(output_name, "w");

    if (file == NULL) {
      fprintf(stderr, "It was not possible to open %s. Fallbacking to stdout...", output_name);
    } else {
      output_fd = file;
    }
  }
  

  fprintf(output_fd, "#ifndef %s_H_\n", header_upper);
  fprintf(output_fd, "#define %s_H_\n", header_upper);

  fprintf(output_fd, "\n");

  for (size_t i = 0; i < file_count; ++i) {
    parse_file(&files[i], output_fd);
  }

  fprintf(output_fd, "#endif // %s_H_\n", header_upper);


  fclose(output_fd);
  return 0;
}

static void* SAOK2C_alloc(size_t size) {
  if (SAOK2C_mem_count + size >= SAOK2C_MEM_CAP) {
    fprintf(stderr, "You ran out of memory. How could use use so much memory, huh?!\n");
    exit(1);
  }

  void *ptr = SAOK2C_MEM + SAOK2C_mem_count;
  SAOK2C_mem_count += size;
  return ptr;
}

void files_append(FILE *file, size_t size, char *name, char *upper_name) {
  if (file_count == SAOK2C_MAX_OPEN_FILES) {
    fprintf(stderr, "Max open files reached (%d)", SAOK2C_MAX_OPEN_FILES);
    exit(1);
  }

  files[file_count++] = (SAOK2C_File){
    .handle = file,
    .size = size,
    .name = name,
    .upper_name = upper_name,
  };
}

char* upper(char *s) {
  size_t s_len = strlen(s);

  char *out = (char *)SAOK2C_alloc(s_len + 1);

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

void parse_size(const char *input_name) {
  FILE *file = fopen(input_name, "r");
  if (file == NULL) {
    fprintf(stderr, "Input file '%s' does not exist\n", input_name);
    return;
  }

  fseek(file, 0L, SEEK_END);
  size_t input_size = ftell(file);
  rewind(file);

  const size_t input_name_size = strlen(input_name);
  size_t const_name_size = 0;
  for (size_t i = 0; i < input_name_size && input_name[i] != '.'; ++i) {
    const_name_size++;
  }

  char *const_name = (char *)SAOK2C_alloc(const_name_size + 1);

  strncpy(const_name, input_name, const_name_size);
  const_name[const_name_size] = '\0';


  char *const_upper = upper(const_name);

  files_append(file, input_size, const_name, const_upper);
}

void parse_file(SAOK2C_File *file, FILE *output_fd) {
 fprintf(output_fd, "#define %s_SIZE %zu\n\n", file->upper_name, file->size);
 fprintf(output_fd, "const char %s[%s_SIZE] = {\n", file->name, file->upper_name);

  unsigned char line_buffer[16] = {0};
  size_t n = fread(line_buffer, 1, 16, file->handle);

  while (n != 0) {
   fprintf(output_fd, "  ");
    for (size_t i = 0; i < n; ++i) {
     fprintf(output_fd, "'\\x%02x', ", line_buffer[i]);
    }
    n = fread(line_buffer, 1, 16, file->handle);
   fprintf(output_fd, "\n");
  }

 fprintf(output_fd, "};\n");
 fprintf(output_fd, "\n");

 fclose(file->handle);
}

