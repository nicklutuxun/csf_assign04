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

#ifndef ELF_NAMES_H
#define ELF_NAMES_H

#ifdef __cplusplus
#include <cstdint>
#include <elf.h>
extern "C" {
#else
#include <stdint.h>
#endif

// translate value of e_type field to a string
const char *get_type_name(uint16_t value);

// translate value of e_machine field to a string
const char *get_machine_name(uint16_t value);

// translate value of e_indent[EI_DATA] field tp a string
const char *get_endian(uint16_t value);

void print_section_name(Elf64_Ehdr *elf_header, int i);

void print_section_type(Elf64_Ehdr *elf_header, int i);

void print_section_offset(Elf64_Ehdr *elf_header, int i);

void print_section_size(Elf64_Ehdr *elf_header, int i);

#ifdef __cplusplus
}
#endif

#endif // ELF_NAMES_H
