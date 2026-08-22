
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <file>\n";
        return 1;
    }

    std::ifstream file(argv[1], std::ios::binary);
    if (!file) {
        std::cout << "Could not open file: " << argv[1] << "\n";
        return 1;
    }

    unsigned char ident[16];
    file.read((char*)ident, 16);
    if (!file) {
        std::cout << "File too small, not an ELF file.\n";
        return 1;
    }

    if (ident[0] != 0x7F || ident[1] != 'E' ||
        ident[2] != 'L'  || ident[3] != 'F') {
        std::cout << "NOT an ELF file (bad magic number)\n";
        return 1;
    }
    std::cout << "Magic number: OK (7F 45 4C 46)\n";

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

    unsigned char ei_data = ident[5];
    if (ei_data == 1) {
        std::cout << "Endianness: little-endian\n";
    } else if (ei_data == 2) {
        std::cout << "Endianness: big-endian\n";
    } else {
        std::cout << "Endianness: unknown\n";
    }


    uint16_t e_type;
    file.read((char*)&e_type, 2);

    std::cout << "Type: ";
    if (e_type == 1) std::cout << "REL (relocatable)\n";
    else if (e_type == 2) std::cout << "EXEC (executable)\n";
    else if (e_type == 3) std::cout << "DYN (shared object / PIE)\n";
    else if (e_type == 4) std::cout << "CORE (core dump)\n";
    else std::cout << "unknown (" << e_type << ")\n";

    uint16_t e_machine;
    file.read((char*)&e_machine, 2);

    std::cout << "Machine: ";
    if (e_machine == 62) std::cout << "x86-64\n";
    else if (e_machine == 3) std::cout << "x86 (32-bit)\n";
    else if (e_machine == 183) std::cout << "ARM64\n";
    else if (e_machine == 40) std::cout << "ARM\n";
    else std::cout << "other (code = " << e_machine << ")\n";

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
