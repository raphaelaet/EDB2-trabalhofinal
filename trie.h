#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <utility>

#include "chamado.h"

using namespace std;

struct NoTrie {
    NoTrie *filhos[36];
    string meuHash = "";
    //int prioridade = 0;
    //int prioridadeIluminacao = 0;
    //int prioridadeAgua = 0;
    //int prioridadeBuraco = 0;
    vector<Chamado> chamados;
    bool completo;
    
    NoTrie() {
        completo = false;
        for (int i = 0; i < 36; i++){
            filhos[i] = nullptr;
        }
    }
};

class Trie {
private:
    NoTrie *raiz;
    size_t precisao;
    void clear(NoTrie *no);
    void coletar(NoTrie* no, GeoHash::Point centro, double raio, vector<Chamado*>& resultados);
    
public:
    Trie(size_t prec);
    
    ~Trie();
    
    //bool adicionarReport(NoTrie *raiz, string hash, int pos, string onde = "");
    void adicionarReport(const string &hash, const Chamado &c);
    void adicionarReport(const string_view &hash, const Chamado &c);
    
    void procurarReport(const string &hash);
    void procurarReport(const string_view &hash);
    
    void procurarPrefixo(const string &pfx);
    void procurarPrefixoV(const string_view &pfx);
    
    bool procurarSilencioso(const string &hash);
    bool procurarSilencioso(const string_view &hash);

    vector<Chamado*> consultarRaio(GeoHash::Point centro, double raio);
    bool registrarChamado(const Chamado& c, double raio_dedup);
};

#endif
