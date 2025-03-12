#include <iostream>
#include <fstream>
#include <array>
#include <map>
#include <string>
#include <queue>
#include <vector>
#include <stdexcept>

// Estrutura para os nós da árvore de Huffman
struct No
{
    unsigned char caractere; // Caractere armazenado (se for folha)
    int frequencia;          // Frequência do caractere ou soma dos filhos
    No *esquerda;            // Filho à esquerda
    No *direita;             // Filho à direita
    No(unsigned char c, int f) : caractere(c), frequencia(f), esquerda(nullptr), direita(nullptr) {}
    No(int f, No *e, No *d) : caractere(0), frequencia(f), esquerda(e), direita(d) {}
};

// Comparador para a fila de prioridade (min-heap)
struct Comparador
{
    bool operator()(const No *a, const No *b) const
    {
        return a->frequencia > b->frequencia;
    }
};

long long obterTamanhoArquivo(const std::string &nomeArquivo)
{
    std::ifstream arquivo(nomeArquivo, std::ios::binary | std::ios::ate);
    if (arquivo)
    {
        long long tamanho = arquivo.tellg();
        arquivo.close();
        return tamanho;
    }
    return 0; // Caso o arquivo não possa ser aberto
}

// Função para contar a frequência de cada caractere no arquivo
std::array<int, 256> contarFrequencias(const std::string &nomeArquivo)
{
    std::array<int, 256> frequencias = {0};
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

// Função para liberar a memória da árvore
void deleteArvore(No *no)
{
    if (no == nullptr)
        return;
    deleteArvore(no->esquerda);
    deleteArvore(no->direita);
    delete no;
}
// Função para construir a árvore de Huffman
No *construirArvoreHuffman(const std::array<int, 256> &frequencias)
{
    std::priority_queue<No *, std::vector<No *>, Comparador> fila;
    for (int i = 0; i < 256; ++i)
    {
        if (frequencias[i] > 0)
        {
            fila.push(new No(static_cast<unsigned char>(i), frequencias[i]));
        }
    }
    if (fila.empty())
    {
        return nullptr; // Arquivo vazio
    }
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

// Função para gerar os códigos de Huffman
void gerarCodigos(No *no, const std::string &codigo, std::map<unsigned char, std::string> &codigos)
{
    if (no == nullptr)
        return;
    if (no->esquerda == nullptr && no->direita == nullptr)
    {                                                           // Nó folha
        codigos[no->caractere] = codigo.empty() ? "0" : codigo; // Caso especial para um único caractere
    }
    else
    {
        gerarCodigos(no->esquerda, codigo + "0", codigos);
        gerarCodigos(no->direita, codigo + "1", codigos);
    }
}

// Função para serializar a árvore de Huffman no arquivo comprimido
void serializarArvore(No *no, std::ofstream &arquivo)
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

// Função para comprimir o arquivo
void comprimir(const std::string &nomeArquivoEntrada, const std::string &nomeArquivoSaida)
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
        // Obter tamanhos
        auto tamanhoOriginal = obterTamanhoArquivo(nomeArquivoEntrada);
        auto tamanhoComprimido = obterTamanhoArquivo(nomeArquivoSaida);

        // Calcular e exibir a taxa de compressão
        if (tamanhoOriginal > 0)
        {
            double taxaCompressao = (1.0 - static_cast<double>(tamanhoComprimido) / tamanhoOriginal) * 100.0;
            std::cout << "Taxa de Compressão: " << taxaCompressao << "%\n";
        }
        else
        {
            std::cout << "Erro: Arquivo original vazio ou inválido.\n";
        }
    // Liberar memória
    deleteArvore(raiz);
}

// Função para desserializar a árvore de Huffman
No *desserializarArvore(std::ifstream &arquivo)
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

// Função para descomprimir o arquivo
void descomprimir(const std::string &nomeArquivoComprimido, const std::string &nomeArquivoSaida)
{
    std::ifstream entrada(nomeArquivoComprimido, std::ios::binary);
    if (!entrada)
    {
        throw std::runtime_error("Não foi possível abrir o arquivo comprimido.");
    }
    std::ofstream saida(nomeArquivoSaida, std::ios::binary);
    if (!saida)
    {
        throw std::runtime_error("Não foi possível abrir o arquivo de saída.");
    }

    // Desserializar a árvore
    No *raiz = desserializarArvore(entrada);
    if (!raiz)
    {
        deleteArvore(raiz);
        return; // Arquivo vazio
    }

    // Ler o número de caracteres
    int numCaracteres;
    entrada.read(reinterpret_cast<char *>(&numCaracteres), sizeof(numCaracteres));

    // Decodificar os bits
    No *atual = raiz;
    char byte;
    int caracteresLidos = 0;
    while (entrada.get(byte) && caracteresLidos < numCaracteres)
    {
        for (int i = 7; i >= 0 && caracteresLidos < numCaracteres; --i)
        {
            bool bit = (byte & (1 << i)) != 0;
            atual = bit ? atual->direita : atual->esquerda;
            if (atual->esquerda == nullptr && atual->direita == nullptr)
            {
                saida.put(atual->caractere);
                atual = raiz;
                caracteresLidos++;
            }
        }
    }

    // Liberar memória
    deleteArvore(raiz);
}

// Função principal
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Uso: " << argv[0] << " [-c|-d] arquivo_entrada arquivo_saida\n";
        return 1;
    }
    std::string modo = argv[1];
    std::string entrada = argv[2];
    std::string saida = argv[3];

    try
    {
        if (modo == "-c")
        {
            comprimir(entrada, saida);
            std::cout << "Arquivo comprimido com sucesso.\n";
        }
        else if (modo == "-d")
        {
            descomprimir(entrada, saida);
            std::cout << "Arquivo descomprimido com sucesso.\n";
        }
        else
        {
            std::cerr << "Modo inválido. Use -c para comprimir ou -d para descomprimir.\n";
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