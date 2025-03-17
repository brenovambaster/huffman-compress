#include "huffman.hpp"
#include <iostream>
#include <stdexcept>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Uso: " << argv[0] << " <C | D> arquivo_entrada arquivo_saida\n";
        return 1;
    }
    std::string modo = argv[1];
    std::string entrada = argv[2];
    std::string saida = argv[3];

    HuffmanCoder coder;
    try
    {
        if (modo == "C")
        {
            coder.comprimir(entrada, saida);
            std::cout << "Arquivo comprimido com sucesso.\n";
        }
        else if (modo == "D")
        {
            coder.descomprimir(entrada, saida);
            std::cout << "Arquivo descomprimido com sucesso.\n";
        }
        else
        {
            std::cerr << "Modo inválido. Use C para comprimir ou D para descomprimir.\n";
            return 1;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Erro: " << e.what() << '\n';
        return 1;
    }
    return 0;
}