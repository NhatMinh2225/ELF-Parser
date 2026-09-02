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

    unsigned char *header = malloc(64);
    fread(header, 1, 64, fp);

    if(header[0] != 0x7f || header[1] != 'E' || header[2] != 'L' || header[3] != 'F'){
        printf("Not ELF\n");
        free(header);
        return 1;
    }
    else{
        // Class (32/64-bit)
        printf("class:   %s\n", header[4] == 1 ? "ELF32" : (header[4] == 2 ? "ELF64" : "Unknown"));

        // Endianness
        printf("data:    %s\n", header[5] == 1 ? "Little-Endian" : (header[5] == 2 ? "Big-Endian" : "Unknown"));

        // Object Type
        uint16_t type = ((uint16_t)header[17] << 8) | header[16];
        char *type_str = "Unknown";
        if(type == 1) type_str = "REL (Relocatable file)";
        else if(type == 2) type_str = "EXEC (Executable file)";
        else if(type == 3) type_str = "DYN (Shared object file)";
        else if(type == 4) type_str = "CORE (Core file)";
        printf("type:    %s\n", type_str);

        // Machine
        uint16_t mc = ((uint16_t)header[19] << 8) | header[18];
        printf("machine: %u\n", mc);

        // Entry Point
        if (header[4] == 1) { // ELF32
            printf("entry:   0x%02X%02X%02X%02X\n",
                   header[27], header[26], header[25], header[24]);
        } 
        else if (header[4] == 2) { // ELF64
            printf("entry:   0x%02X%02X%02X%02X%02X%02X%02X%02X\n",
                   header[31], header[30], header[29], header[28],
                   header[27], header[26], header[25], header[24]);
        }
    }

    free(header);
    fclose(fp);
    return 0;
}
