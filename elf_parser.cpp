#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <inttypes.h>

int main(int argc, char **argv){
    if(argc != 2){
        printf("Include only 1 file. Please try again\n");
        return 1;
    }

    FILE* fp = fopen(argv[1], "rb");

    if(fp == NULL){
        return 1;
    }

    Elf64_Ehdr header;

    fread(&header, 1, sizeof(header), fp);

    if(header.e_ident[0] != 0x7f || header.e_ident[1] != 'E' || header.e_ident[2] != 'L' || header.e_ident[3] != 'F'){
        printf("Not ELF\n");
        return 1;
    }
    else{
        printf("machine: %d\n", header.e_machine);
        printf("entry:   0x%" PRIx64 "\n", header.e_entry);
    }
}
