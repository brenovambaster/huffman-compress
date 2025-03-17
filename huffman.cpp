#include "huffman.hpp"
#include <fstream>
#include <iostream>
#include <fstream>
#include <array>
#include <map>
#include <string>
#include <queue>
#include <vector>
#include <stdexcept>

No::No(unsigned char c, int f) : caractere(c), frequencia(f), esquerda(nullptr), direita(nullptr) {}
No::No(int f, No *e, No *d) : caractere(0), frequencia(f), esquerda(e), direita(d) {}

bool Comparador::operator()(const No *a, const No *b) const
{
    return a->frequencia > b->frequencia;
}

std::array<int, TAMANHO_ASCII> HuffmanCoder::contarFrequencias(const std::string &nomeArquivo)
{
    std::array<int, TAMANHO_ASCII> frequencias = {0};
    std::ifstream arquivo(nomeArquivo, std::ios::binary);
    if (!arquivo)
    {
        throw std::runtime_error("Não foi possível abrir o arquivo de entrada.");
    }
    char c;
    while (arquivo.get(c))
    {
        frequencias[static_cast<unsigned char>(c)]++;
    }
    return frequencias;
}

No *HuffmanCoder::construirArvoreHuffman(const std::array<int, TAMANHO_ASCII> &frequencias)
{
    std::priority_queue<No *, std::vector<No *>, Comparador> fila;
    for (int i = 0; i < TAMANHO_ASCII; ++i)
    {
        if (frequencias[i] > 0)
        {
            fila.push(new No(static_cast<unsigned char>(i), frequencias[i]));
        }
    }
    if (fila.empty())
        return nullptr;
    while (fila.size() > 1)
    {
        No *esquerda = fila.top();
        fila.pop();
        No *direita = fila.top();
        fila.pop();
        No *pai = new No(esquerda->frequencia + direita->frequencia, esquerda, direita);
        fila.push(pai);
    }
    return fila.top();
}

void HuffmanCoder::gerarCodigos(No *no, const std::string &codigo, std::map<unsigned char, std::string> &codigos)
{
    if (no == nullptr)
        return;
    if (no->esquerda == nullptr && no->direita == nullptr)
    {
        codigos[no->caractere] = codigo.empty() ? "0" : codigo;
    }
    else
    {
        gerarCodigos(no->esquerda, codigo + "0", codigos);
        gerarCodigos(no->direita, codigo + "1", codigos);
    }
}

void HuffmanCoder::serializarArvore(No *no, std::ofstream &arquivo)
{
    if (no == nullptr)
        return;
    if (no->esquerda == nullptr && no->direita == nullptr)
    {
        arquivo.put('1');
        arquivo.put(no->caractere);
    }
    else
    {
        arquivo.put('0');
        serializarArvore(no->esquerda, arquivo);
        serializarArvore(no->direita, arquivo);
    }
}

No *HuffmanCoder::desserializarArvore(std::ifstream &arquivo)
{
    char bit;
    if (!arquivo.get(bit))
        return nullptr;
    if (bit == '1')
    {
        char c;
        arquivo.get(c);
        return new No(static_cast<unsigned char>(c), 0);
    }
    else
    {
        No *esquerda = desserializarArvore(arquivo);
        No *direita = desserializarArvore(arquivo);
        return new No(0, esquerda, direita);
    }
}

void HuffmanCoder::deleteArvore(No *no)
{
    if (no == nullptr)
        return;
    deleteArvore(no->esquerda);
    deleteArvore(no->direita);
    delete no;
}

void HuffmanCoder::comprimir(const std::string &nomeArquivoEntrada, const std::string &nomeArquivoSaida)
{
    // Contar frequências
    auto frequencias = contarFrequencias(nomeArquivoEntrada);
    No *raiz = construirArvoreHuffman(frequencias);

    // Gerar códigos
    std::map<unsigned char, std::string> codigos;
    gerarCodigos(raiz, "", codigos);

    std::ifstream entrada(nomeArquivoEntrada, std::ios::binary);
    std::ofstream saida(nomeArquivoSaida, std::ios::binary);
    if (!saida)
    {
        deleteArvore(raiz);
        throw std::runtime_error("Não foi possível abrir o arquivo de saída.");
    }

    // Serializar a árvore
    serializarArvore(raiz, saida);

    // Escrever o número total de caracteres
    int numCaracteres = 0;
    for (const auto &freq : frequencias)
    {
        numCaracteres += freq;
    }
    saida.write(reinterpret_cast<const char *>(&numCaracteres), sizeof(numCaracteres));

    // Comprimir os dados
    std::string buffer;
    char c;
    while (entrada.get(c))
    {
        buffer += codigos[static_cast<unsigned char>(c)];
        while (buffer.size() >= 8)
        {
            std::string byteStr = buffer.substr(0, 8);
            buffer = buffer.substr(8);
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteStr, nullptr, 2));
            saida.put(byte);
        }
    }
    // Escrever os bits restantes (completar com zeros, se necessário)
    if (!buffer.empty())
    {
        while (buffer.size() < 8)
        {
            buffer += "0";
        }
        unsigned char byte = static_cast<unsigned char>(std::stoi(buffer, nullptr, 2));
        saida.put(byte);
    }
    // Liberar memória
    deleteArvore(raiz);
}

void HuffmanCoder::abrirArquivosDescompressao(
    const std::string &nomeArquivoComprimido,
    const std::string &nomeArquivoSaida,
    std::ifstream &entrada,
    std::ofstream &saida)
{
    entrada.open(nomeArquivoComprimido, std::ios::binary);
    if (!entrada)
    {
        throw std::runtime_error("Não foi possível abrir o arquivo comprimido.");
    }

    saida.open(nomeArquivoSaida, std::ios::binary);
    if (!saida)
    {
        throw std::runtime_error("Não foi possível abrir o arquivo de saída.");
    }
}

int HuffmanCoder::lerNumeroCaracteres(std::ifstream &entrada)
{
    int numCaracteres;
    entrada.read(reinterpret_cast<char *>(&numCaracteres), sizeof(numCaracteres));
    return numCaracteres;
}

bool HuffmanCoder::processarByte(
    char byte,
    No *raiz,
    No *&atual,
    std::ofstream &saida,
    int &caracteresLidos,
    int numCaracteres)
{
    const short int BIT_MAIS_SIGNIFICATIVO = 7;

    for (int i = BIT_MAIS_SIGNIFICATIVO; i >= 0 && caracteresLidos < numCaracteres; --i)
    {
        bool bit = (byte & (1 << i)) != 0;
        atual = bit ? atual->direita : atual->esquerda;

        if (atual->esquerda == nullptr && atual->direita == nullptr)
        {
            saida.put(atual->caractere);
            atual = raiz;
            caracteresLidos++;

            if (caracteresLidos >= numCaracteres)
            {
                return true;
            }
        }
    }

    return false;
}


void HuffmanCoder::decodificarDados(std::ifstream &entrada, std::ofstream &saida, No *raiz, int numCaracteres)
{
    No *atual = raiz;
    char byte;
    int caracteresLidos = 0;

    while (entrada.get(byte) && caracteresLidos < numCaracteres)
    {
        if (processarByte(byte, raiz, atual, saida, caracteresLidos, numCaracteres))
        {
            break;
        }
    }
}


void HuffmanCoder::descomprimir(const std::string &nomeArquivoComprimido, const std::string &nomeArquivoSaida)
{
    std::ifstream entrada;
    std::ofstream saida;

    // Abre os arquivos de entrada e saída
    abrirArquivosDescompressao(nomeArquivoComprimido, nomeArquivoSaida, entrada, saida);

    // Desserializa a árvore
    No *raiz = desserializarArvore(entrada);
    if (!raiz)
    {
        deleteArvore(raiz);
        return; // Arquivo vazio
    }

    // Lê o número de caracteres
    int numCaracteres = lerNumeroCaracteres(entrada);

    // Decodifica os dados
    decodificarDados(entrada, saida, raiz, numCaracteres);

    // Libera memória
    deleteArvore(raiz);
}