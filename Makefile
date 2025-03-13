# Detectar o sistema operacional
ifeq ($(OS),Windows_NT)
    RM = del
    EXEC = huffman.exe
else
    RM = rm -f
    EXEC = huffman
endif

# Configurações de compilação
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

# Alvo padrão
all: $(EXEC)

# Linkar o executável
$(EXEC): main.o huffman.o
	$(CXX) $(CXXFLAGS) -o $(EXEC) main.o huffman.o

# Compilar main.cpp
main.o: main.cpp huffman.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

# Compilar huffman.cpp
huffman.o: huffman.cpp huffman.hpp
	$(CXX) $(CXXFLAGS) -c huffman.cpp

# Limpar arquivos gerados
clean:
	$(RM) *.o $(EXEC)