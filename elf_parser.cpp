// elf_parser_basic.cpp
//
// The simplest possible version: open the file, read the needed bytes
// by hand, no structs, no helper functions, no polymorphism.
// Everything lives in main() so it reads top-to-bottom like the bytes
// on disk.
//
// Build: g++ -std=c++17 -O2 -o elf_parser_basic elf_parser_basic.cpp
// Run:   ./elf_parser_basic <path-to-file>

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    // Open the file in binary mode
    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
        std::cout << "Could not open file: " << argv[1] << "\n";
        return 1;
    }

    // Read the first 16 bytes (e_ident) into a raw byte array
    unsigned char ident[16];
    file.read((char*)ident, 16);
    if (!file) {
        std::cout << "File too small, not an ELF file.\n";
        return 1;
    }

    // ----- Step 1: check the magic number -----
    // The first 4 bytes must be: 0x7F 'E' 'L' 'F'
    if (ident[0] != 0x7F || ident[1] != 'E' ||
        ident[2] != 'L'  || ident[3] != 'F') {
        std::cout << "NOT an ELF file (bad magic number)\n";
        return 1;
    }
    std::cout << "Magic number: OK (7F 45 4C 46)\n";

    // ----- Step 2: byte 5 (index 4) tells us 32-bit or 64-bit -----
    unsigned char ei_class = ident[4];
    bool is64 = (ei_class == 2); // 1 = ELF32, 2 = ELF64

    if (ei_class == 1) {
        std::cout << "Class: ELF32 (32-bit)\n";
    } else if (ei_class == 2) {
        std::cout << "Class: ELF64 (64-bit)\n";
    } else {
        std::cout << "Class: unknown\n";
        return 1;
    }

    // ----- Step 3: byte 6 (index 5) tells us little-endian or big-endian -----
    unsigned char ei_data = ident[5];
    if (ei_data == 1) {
        std::cout << "Endianness: little-endian\n";
    } else if (ei_data == 2) {
        std::cout << "Endianness: big-endian\n";
    } else {
        std::cout << "Endianness: unknown\n";
    }

    // ----- Step 4: read e_type (2 bytes) -----
    // Right after the 16-byte e_ident, e_type comes next in the header
    uint16_t e_type;
    file.read((char*)&e_type, 2);

    std::cout << "Type: ";
    if (e_type == 1) std::cout << "REL (relocatable)\n";
    else if (e_type == 2) std::cout << "EXEC (executable)\n";
    else if (e_type == 3) std::cout << "DYN (shared object / PIE)\n";
    else if (e_type == 4) std::cout << "CORE (core dump)\n";
    else std::cout << "unknown (" << e_type << ")\n";

    // ----- Step 5: read e_machine (2 bytes) -----
    uint16_t e_machine;
    file.read((char*)&e_machine, 2);

    std::cout << "Machine: ";
    if (e_machine == 62) std::cout << "x86-64\n";
    else if (e_machine == 3) std::cout << "x86 (32-bit)\n";
    else if (e_machine == 183) std::cout << "ARM64\n";
    else if (e_machine == 40) std::cout << "ARM\n";
    else std::cout << "other (code = " << e_machine << ")\n";

    // ----- Step 6: skip e_version (4 bytes), then read the entry point -----
    file.seekg(4, std::ios::cur); // skip e_version

    if (is64) {
        // ELF64: e_entry is 8 bytes
        uint64_t entry;
        file.read((char*)&entry, 8);
        std::cout << "Entry point: 0x" << std::hex << entry << std::dec << "\n";
    } else {
        // ELF32: e_entry is 4 bytes
        uint32_t entry;
        file.read((char*)&entry, 4);
        std::cout << "Entry point: 0x" << std::hex << entry << std::dec << "\n";
    }

    return 0;
}
