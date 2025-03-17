/**
 * @file huffman.hpp
 * @brief Definição das classes e estruturas para o algoritmo de compressão de Huffman
 * @author Autor Original
 * @date Março 2025
 *
 * Este arquivo de cabeçalho define as interfaces para implementação do algoritmo
 * de compressão e descompressão de Huffman.
 */

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP
#define TAMANHO_ASCII 256

#include <string>
#include <map>
#include <queue>
#include <vector>
#include <array>
#include <fstream>

/**
 * @brief Estrutura que representa um nó da árvore de Huffman
 *
 * Cada nó pode ser uma folha (contendo um caractere) ou um nó interno
 * (com dois filhos). Os nós folha armazenam caracteres e suas frequências,
 * enquanto os nós internos armazenam a soma das frequências de seus filhos.
 */
struct No
{
    unsigned char caractere; /**< Caractere armazenado no nó (para nós folha) */
    int frequencia;          /**< Frequência do caractere ou soma das frequências dos filhos */
    No *esquerda;            /**< Ponteiro para o filho esquerdo */
    No *direita;             /**< Ponteiro para o filho direito */

    /**
     * @brief Construtor para nós folha
     * @param c Caractere a ser armazenado
     * @param f Frequência do caractere
     */
    No(unsigned char c, int f);

    /**
     * @brief Construtor para nós internos
     * @param f Soma das frequências dos filhos
     * @param e Ponteiro para o filho esquerdo
     * @param d Ponteiro para o filho direito
     */
    No(int f, No *e, No *d);
};

/**
 * @brief Comparador para ordenar nós na fila de prioridade
 *
 * Utilizado pela fila de prioridade para construir a árvore de Huffman
 * priorizando os nós com menor frequência.
 */
struct Comparador
{
    /**
     * @brief Operador de comparação para ordenar nós
     * @param a Primeiro nó a ser comparado
     * @param b Segundo nó a ser comparado
     * @return true se a frequência de a for maior que a de b
     *
     * Este operador inverte a comparação natural para criar uma fila
     * de prioridade mínima (os nós com menor frequência terão maior prioridade).
     */
    bool operator()(const No *a, const No *b) const;
};

/**
 * @brief Classe principal para codificação e decodificação usando o algoritmo de Huffman
 *
 * Fornece métodos para compressão e descompressão de arquivos utilizando
 * o algoritmo de codificação de Huffman, que atribui códigos de tamanho
 * variável para caracteres com base em suas frequências.
 */
class HuffmanCoder
{
public:
    /**
     * @brief Comprime um arquivo utilizando codificação de Huffman
     * @param nomeArquivoEntrada Caminho para o arquivo a ser comprimido
     * @param nomeArquivoSaida Caminho para o arquivo comprimido a ser gerado
     * @throw std::runtime_error Se ocorrer erro na abertura dos arquivos
     *
     * O arquivo comprimido contém:
     * 1. A árvore de Huffman serializada
     * 2. O número total de caracteres no arquivo original
     * 3. Os dados comprimidos usando a codificação gerada
     */
    void comprimir(const std::string &nomeArquivoEntrada, const std::string &nomeArquivoSaida);

    /**
     * @brief Descomprime um arquivo previamente comprimido com o algoritmo de Huffman
     * @param nomeArquivoComprimido Nome do arquivo comprimido
     * @param nomeArquivoSaida Nome do arquivo descomprimido a ser gerado
     * @throw std::runtime_error Se não for possível abrir os arquivos
     */
    void descomprimir(const std::string &nomeArquivoComprimido, const std::string &nomeArquivoSaida);

private:
    /**
     * @brief Conta a frequência de cada byte em um arquivo
     * @param nomeArquivo Nome do arquivo a ser analisado
     * @return Array com a contagem de frequências para cada byte possível (0-255)
     * @throw std::runtime_error Se não for possível abrir o arquivo
     */
    std::array<int, TAMANHO_ASCII> contarFrequencias(const std::string &nomeArquivo);

    /**
     * @brief Constrói a árvore de Huffman baseada nas frequências dos caracteres
     * @param frequencias Array com as frequências de cada caractere
     * @return Ponteiro para a raiz da árvore de Huffman construída
     *
     * Utiliza uma fila de prioridade para construir a árvore de forma eficiente,
     * combinando iterativamente os dois nós com menor frequência.
     */
    No *construirArvoreHuffman(const std::array<int, TAMANHO_ASCII> &frequencias);

    /**
     * @brief Gera os códigos de Huffman para cada caractere
     * @param no Nó atual da árvore (inicialmente a raiz)
     * @param codigo Código atual (inicialmente vazio)
     * @param codigos Mapa que será preenchido com os códigos para cada caractere
     *
     * Percorre a árvore recursivamente, adicionando '0' ao descer para a esquerda
     * e '1' ao descer para a direita, até chegar a um nó folha.
     */
    void gerarCodigos(No *no, const std::string &codigo, std::map<unsigned char, std::string> &codigos);

    /**
     * @brief Serializa a árvore de Huffman para um arquivo
     * @param no Nó atual da árvore (inicialmente a raiz)
     * @param arquivo Arquivo de saída onde a árvore será salva
     *
     * Formato de serialização:
     * - '0' para nós internos
     * - '1' seguido do caractere para nós folha
     */
    void serializarArvore(No *no, std::ofstream &arquivo);

    /**
     * @brief Desserializa a árvore de Huffman de um arquivo
     * @param arquivo Arquivo de entrada contendo a árvore serializada
     * @return Ponteiro para a raiz da árvore desserializada
     */
    No *desserializarArvore(std::ifstream &arquivo);

    /**
     * @brief Libera a memória alocada para a árvore de Huffman
     * @param no Nó atual da árvore (inicialmente a raiz)
     *
     * Percorre recursivamente a árvore e libera a memória de cada nó.
     */
    void deleteArvore(No *no);

    /**
     * @brief Abre os arquivos de entrada e saída para descompressão
     * @param nomeArquivoComprimido Caminho do arquivo comprimido
     * @param nomeArquivoSaida Caminho do arquivo de saída
     * @param entrada Referência ao stream de entrada a ser inicializado
     * @param saida Referência ao stream de saída a ser inicializado
     * @throw std::runtime_error Se houver erro ao abrir algum dos arquivos
     */
    void abrirArquivosDescompressao(
        const std::string &nomeArquivoComprimido,
        const std::string &nomeArquivoSaida,
        std::ifstream &entrada,
        std::ofstream &saida);

    /**
     * @brief Lê o número de caracteres do arquivo comprimido
     * @param entrada Stream do arquivo comprimido
     * @return Número de caracteres a serem descomprimidos
     */
    int lerNumeroCaracteres(std::ifstream &entrada);

    /**
     * @brief Processa um único byte do arquivo comprimido
     * @param byte Byte a ser processado
     * @param raiz Ponteiro para a raiz da árvore de Huffman
     * @param atual Referência ao nó atual na árvore
     * @param saida Stream do arquivo de saída
     * @param caracteresLidos Referência ao contador de caracteres lidos
     * @param numCaracteres Número total de caracteres a serem descomprimidos
     * @return true se todos os caracteres foram descomprimidos, false caso contrário
     */
    bool processarByte(
        char byte,
        No *raiz,
        No *&atual,
        std::ofstream &saida,
        int &caracteresLidos,
        int numCaracteres);

    /**
     * @brief Decodifica os dados do arquivo comprimido
     * @param entrada Stream do arquivo comprimido
     * @param saida Stream do arquivo de saída
     * @param raiz Ponteiro para a raiz da árvore de Huffman
     * @param numCaracteres Número total de caracteres a serem descomprimidos
     */
    void decodificarDados(std::ifstream &entrada, std::ofstream &saida, No *raiz, int numCaracteres);
};

#endif // HUFFMAN_HPP