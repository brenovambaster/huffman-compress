# Explicação do Makefile

**CXX = g++:** Define o compilador (neste caso, *g++*).

**CXXFLAGS = -std=c++11 -Wall -Wextra:** Flags de compilação que utilizam o padrão C++11 e ativam avisos extras.

**all: huffman:** Alvo padrão para gerar o executável *huffman*.

**huffman: main.o huffman.o:** Regra para linkar os arquivos objeto (*main.o* e *huffman.o*) e criar o executável *huffman*.

**main.o: main.cpp huffman.hpp:** Regra para compilar *main.cpp* em *main.o*, considerando a dependência em *huffman.hpp*.

**huffman.o: huffman.cpp huffman.hpp:** Regra para compilar *huffman.cpp* em *huffman.o*, considerando a dependência em *huffman.hpp*.

**clean:** Remove os arquivos objeto e o executável para limpar o diretório.

## Como Usar o Makefile

1. Salve o Makefile como "Makefile" (sem extensão) no mesmo diretório dos arquivos `.cpp` e `.hpp`.
2. No terminal, execute:
    - `make` para compilar o projeto e gerar o executável *huffman*.
    - `make clean` para remover os arquivos gerados.