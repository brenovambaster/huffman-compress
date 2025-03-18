# Huffman Coding Implementation

This project implements the Huffman coding algorithm for file compression and decompression using C++. It offers a command-line interface to efficiently compress and decompress files.

- [Huffman Coding Implementation](#huffman-coding-implementation)
  - [Features](#features)
  - [Requirements](#requirements)
  - [Building the Project](#building-the-project)
  - [Usage](#usage)
    - [Compressing a File](#compressing-a-file)
    - [Decompressing a File](#decompressing-a-file)
  - [Running Tests](#running-tests)
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
- `valgrind` for memory leak detection (optional, used in tests).

## Building the Project

Run the following command in your terminal:

```bash
make
```

This command generates the executable named `main`.

## Usage

### Compressing a File

To compress a file, invoke the program with the `C` command, specifying the input and output files:

```bash
./main C input.txt compressed.huff
```

- `input.txt`: File to compress.
- `compressed.huff`: Compressed output file.

### Decompressing a File

To decompress a file, use the `D` command, followed by the compressed file and the desired output file:

```bash
./main D compressed.huff output.txt
```

- `compressed.huff`: File to decompress.
- `output.txt`: Decompressed output file.

## Running Tests

The project includes several predefined test cases:

- To compress test file 1:
  ```bash
  make exeC1
  ```

- To decompress test file 1:
  ```bash
  make exeD1
  ```

- Similar commands exist for test files 2 and 3 (`exeC2`, `exeD2`, `exeC3`, `exeD3`).

- To run all tests with integrity verification:
  ```bash
  make run-all-tests
  ```
  This will compress and decompress all test files, then compare the original and recovered files to ensure data integrity.

## Project Structure

- **huffman.hpp:** Header file containing the definitions for the HuffmanCoder class and related data structures.
- **huffman.cpp:** Implementation of the HuffmanCoder class, including the compression and decompression algorithms.
- **main.cpp:** Entry point managing command-line arguments and invoking the appropriate methods.
- **Makefile:** Build script with test automation support.
- **Test Files:** `input1.txt`, `input2.txt`, and `input3.txt` for testing compression and decompression.

## Cleaning Up

To remove generated object files, executables, and test outputs, run:

```bash
make clean
```

This command deletes:
- All `.o` files.
- The executable (`main`).
- Generated `.huff` files.
- Output text files from decompression.

## Notes

- The program operates in binary mode, ensuring proper handling of all file types.
- Compression efficiency depends on the frequency of characters; files with repetitive data typically achieve better compression.
- Files with uniform character distribution might result in larger output files due to overhead from storing the Huffman tree.
- Valgrind is used during testing to detect memory leaks.

## Contributing

Feel free to report issues or suggest improvements by opening an issue or submitting a pull request.

To create a zip archive of the project files:
```bash
make zip
```

