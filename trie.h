#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <utility>

using namespace std;

struct NoTrie {
    NoTrie *filhos[36];
    string meuHash = "";
    int prioridade = 0;
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
    
    void clear(NoTrie *no);
    
public:
    Trie();
    
    ~Trie();
    
    //bool adicionarReport(NoTrie *raiz, string hash, int pos, string onde = "");
    void adicionarReport(const string &hash, const Chamado &c);
    void adicionarReport(const string_view &hash, const Chamado &c);
    
    void procurarReport(const string &hash);
    void procurarReport(const string_view &hash);
    
    void procurarPrefixo(const string &pfx);
    void procurarPrefixoV(const string_view &pfx);
};

#endif
