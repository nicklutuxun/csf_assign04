/*
 * Analyzing ELF files
 * CSF Assignment 4
 * 
 * Nick Lu
 * tlu32@jhu.edu
 * 
 * Jiarui Chen
 * jchen324@jhu.edu
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>

#include <iostream>

#include "elf_names.h"

using std::cout;
using std::endl;


int main(int argc, char **argv) {
  char* filename = argv[1];
  
  if (argc != 2) {
    cout << "Wrong command argument" << endl;
    return 1;
  }

  int fd = open(filename, O_RDONLY);

  if (fd < 0) {
    // error
    cout << "The file cannot be opened." << endl;
    return 1;
  }


  struct stat statbuf;
  size_t file_size;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    // error
    cout << "An error occurred when calling fstat." << endl;
    return 1;
  } 
  else file_size = statbuf.st_size;

  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);

  if (data == NULL) {
    cout << "Error with memory mapping" << endl;
    return 1;
  }


  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;

  if (!(elf_header->e_ident[0] == 0x7f && elf_header->e_ident[1] == 'E' && elf_header->e_ident[2] == 'L' &&  elf_header->e_ident[3] == 'F' )) {
    cout << "Not an ELF file" << endl;
    exit(0);
  }

  cout << "Object file type: " << get_type_name(elf_header->e_type) << endl;
  cout << "Instruction set: " << get_machine_name(elf_header->e_machine) << endl;
  cout << "Endianness: " << get_endian(elf_header->e_ident[EI_DATA]) << endl;
  
  // iterate through sections
  for (int i = 0; i < elf_header->e_shnum; i++)
  {
    printf("Section header %d: ", i);
    print_section_name(elf_header, i);
    print_section_type(elf_header, i);
    print_section_offset(elf_header, i);
    print_section_size(elf_header, i);
    

    printf("\n");
  }
  
}
