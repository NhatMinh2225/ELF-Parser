# ELF Parser

A small ELF parser written to learn and get more familiar with the ELF file format, pointers, and Linux internals.

This project uses `Elf64_Ehdr` from `<elf.h>` and `fread()` to read and inspect ELF binaries.

Built mainly as a learning project and an excuse to get more comfortable with low-level C and Linux.

## What it does

- Takes a file path as a command line argument
- Opens the file and reads its ELF header
- Checks the first 4 bytes to see if it's actually an ELF file (magic number: `0x7f 'E' 'L' 'F'`)
- If it's not an ELF file, it prints "Not ELF" and returns 1
- If it is an ELF file, it prints:
  - `e_machine` — the machine/architecture type
  - `e_entry` — the entry point address (in hex)

## Usage

Compile it:

```bash
gcc -o elfparser elfparser.c
```

Run it on a file:

```bash
./elfparser /bin/ls
```

Example output:

machine: 62

entry: 0x401020

If the file isn't ELF or doesn't exist, it'll just say so and exit with code 1.

## Notes

- Only takes exactly one argument (the file to check)
- `e_machine` is printed as a plain number for now, not the actual name (like x86-64), might add that later
- This is still a work in progress, mostly for learning purposes
