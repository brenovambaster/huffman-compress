# Compilador C++
CXX = g++

# Flags de compilação
CXXFLAGS = -Wall -Wextra -g -std=c++11

# Nome do executável
TARGET = main

# Arquivos fonte
SOURCES = main.cpp huffman.cpp 

# Arquivos de cabeçalho
HEADERS = huffman.hpp 

# Arquivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Arquivos de teste
TEST_FILES = input1.txt input2.txt input3.txt

# Regras de compilação
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regras para compressão e descompressão
# Compressão do arquivo de teste 1
exeC1:
	valgrind ./$(TARGET) C input1.txt output1.huff

# Compressão do arquivo de teste 2
exeC2:
	valgrind ./$(TARGET) C input2.txt output2.huff

# Compressão do arquivo de teste 3
exeC3:
	valgrind ./$(TARGET) C input3.txt output3.huff

# Descompressão do arquivo de teste 1
exeD1:
	valgrind ./$(TARGET) D output1.huff output1.txt

# Descompressão do arquivo de teste 2
exeD2:
	valgrind ./$(TARGET) D output2.huff output2.txt

# Descompressão do arquivo de teste 3
exeD3:
	valgrind ./$(TARGET) D output3.huff output3.txt


# Regra para realizar toda compressão e descompressão de uma vez
run-all-tests: $(TARGET)
	@echo "=== Iniciando testes completos ==="
	@echo "=== Comprimindo input1.txt ==="
	valgrind ./$(TARGET) C input1.txt output1.huff
	@echo "=== Descomprimindo output1.huff ==="
	valgrind ./$(TARGET) D output1.huff output1.txt
	@echo "=== Comprimindo input2.txt ==="
	valgrind ./$(TARGET) C input2.txt output2.huff
	@echo "=== Descomprimindo output2.huff ==="
	valgrind ./$(TARGET) D output2.huff output2.txt
	@echo "=== Comprimindo input3.txt ==="
	valgrind ./$(TARGET) C input3.txt output3.huff
	@echo "=== Descomprimindo output3.huff ==="
	valgrind ./$(TARGET) D output3.huff output3.txt
	@echo "=== Verificando integridade dos dados ==="
	@echo "Comparando input1.txt e output1.txt:"
	@diff input1.txt output1.txt > /dev/null && echo "OK: Arquivos idênticos" || echo "ERRO: Arquivos diferentes"
	@echo "Comparando input2.txt e output2.txt:"
	@diff input2.txt output2.txt > /dev/null && echo "OK: Arquivos idênticos" || echo "ERRO: Arquivos diferentes"
	@echo "Comparando input3.txt e output3.txt:"
	@diff input3.txt output3.txt > /dev/null && echo "OK: Arquivos idênticos" || echo "ERRO: Arquivos diferentes"
	@echo "=== Testes completos finalizados ==="

clean:
	rm -f $(OBJECTS) $(TARGET) *.huff output*.txt

zip:
	zip "SO TRABALHO - Breno Vambáster - Gustavo Henrique - Natã Teixeira.zip" $(SOURCES) $(HEADERS) Makefile $(TEST_FILES)

.PHONY: all clean run-all-tests exeC1 exeD1 exeC2 exeD2 exeC3 exeD3 zip