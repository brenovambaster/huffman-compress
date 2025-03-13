# Huffman Coding Implementation

This project implements the Huffman coding algorithm for file compression and decompression using C++. It offers a command-line interface to efficiently compress and decompress files.

- [Huffman Coding Implementation](#huffman-coding-implementation)
  - [Features](#features)
  - [Requirements](#requirements)
  - [Building the Project](#building-the-project)
  - [Usage](#usage)
    - [Compressing a File](#compressing-a-file)
    - [Decompressing a File](#decompressing-a-file)
  - [Project Structure](#project-structure)
  - [Cleaning Up](#cleaning-up)
  - [Notes](#notes)
  - [Contributing](#contributing)

## Features

- **Compression:** Compress files using Huffman coding.
- **Decompression:** Restore files to their original format.
- **Cross-Platform:** Supports Linux and Windows.
- **Modular Code:** Well-organized and maintainable codebase.

## Requirements

- A C++ compiler (e.g., g++).
- `make` utility for building the project.

## Building the Project

Run the following command in your terminal:

```bash
make
```

This command generates the executable:

- `huffman` on Linux.
- `huffman.exe` on Windows.

## Usage

### Compressing a File

To compress a file, invoke the program with the `-c` flag, specifying the input and output files:

```bash
./huffman -c input.txt compressed.huf
```

- `input.txt`: File to compress.
- `compressed.huf`: Compressed output file.

### Decompressing a File

To decompress a file, use the `-d` flag, followed by the compressed file and the desired output file:

```bash
./huffman -d compressed.huf output.txt
```

- `compressed.huf`: File to decompress.
- `output.txt`: Decompressed output file.

## Project Structure

- **huffman.hpp:** Header file containing the definitions for the HuffmanCoder class and related data structures.
- **huffman.cpp:** Implementation of the HuffmanCoder class, including the compression and decompression algorithms.
- **main.cpp:** Entry point managing command-line arguments and invoking the appropriate methods.
- **Makefile:** Build script compatible with both Linux and Windows systems.

## Cleaning Up

To remove generated object files and the executable, run:

```bash
make clean
```

This command deletes:

- All `.o` files.
- The executable (`huffman` or `huffman.exe`).

## Notes

- The program operates in binary mode, ensuring proper handling of all file types.
- Compression efficiency depends on the frequency of characters; files with repetitive data typically achieve better compression.
- Files with uniform character distribution might result in larger output files due to overhead from storing the Huffman tree.

## Contributing

Feel free to report issues or suggest improvements by opening an issue or submitting a pull request.

Happy Coding!
