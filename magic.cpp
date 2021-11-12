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

  if (data == (void*)-1) {
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

  Elf64_Shdr *symtab;
  Elf64_Shdr *strtab;
  for (int i = 0; i < (int)(elf_header->e_shnum); i++)
  {

    unsigned char *data = (unsigned char *)elf_header;
    unsigned char *sec_header_data = data + elf_header->e_shoff;
    Elf64_Shdr *curr_section = (Elf64_Shdr*)(sec_header_data + elf_header->e_shentsize * i);

    printf("Section header %d: ", i);
    Elf64_Shdr *section_name_table = (Elf64_Shdr*)(elf_header->e_shentsize * elf_header->e_shstrndx + sec_header_data);
    printf("name=%s, ", (char *)data + section_name_table->sh_offset + curr_section->sh_name);
    printf("type=%lx, ", (long unsigned int)curr_section->sh_type);
    printf("offset=%lx, ", curr_section->sh_offset);
    printf("size=%lx", curr_section->sh_size);
    printf("\n");

    if (strcmp((char*)(data + section_name_table->sh_offset + curr_section->sh_name), ".symtab") == 0) symtab = curr_section;
    if (strcmp((char*)(data + section_name_table->sh_offset + curr_section->sh_name), ".strtab") == 0) strtab = curr_section;
  }

  Elf64_Xword entry_size = symtab->sh_entsize;
  Elf64_Xword total_size = symtab->sh_size;
  Elf64_Xword total_entry = total_size/entry_size;
 
  for (int i=0; i< (int)total_entry; i++) {
    Elf64_Sym *symtab_object = (Elf64_Sym *)((unsigned char*)data + symtab->sh_offset + i*entry_size);
    printf("Symbol %d: ", i);
    printf("name=%s, ", (char *)data + strtab->sh_offset + symtab_object->st_name);
    printf("size=%lx, ", (long unsigned int)symtab_object->st_size);
    printf("info=%lx, ", (long unsigned int)symtab_object->st_info);
    printf("other=%lx", (long unsigned int)symtab_object->st_other);
    printf("\n");
  }
}
