#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <utility>

using namespace std;

#include "trie.h"

void Trie::clear(NoTrie *no){
    if (!no) return;
        
    for(int i = 0; i < 36; i++){
        if(no->filhos[i]){
               clear(no->filhos[i]);
        }
    }
}
    
Trie::Trie(){
    raiz = new NoTrie();
}
    
Trie::~Trie(){
    clear(raiz);
}

void Trie::adicionarReport(const string &hash, const Chamado &c){
    NoTrie *atual = raiz;
    int pos = -1;

    for(char c : hash){
        //Se for realmente um num...
        pos = c - '0';
        //Se não...
        if(pos >= 10) {
            pos = c - 'a' + 10;
        }
        
        if(atual->filhos[pos] == nullptr){
            atual->filhos[pos] = new NoTrie();
        }
        
        atual = atual->filhos[pos];
        //atual->meuHash = 
    }
    
    switch(c.tipo){
        case Tipo::Iluminacao:
            atual->prioridadeIluminacao += 1;
        break;
        case Tipo::Agua:
            atual->prioridadeAgua += 1;
        break;
        case Tipo::Buraco:
            atual->prioridadeBuraco += 1;
        break;
        default: cout << "Houve erro." << endl;
    }
    
    atual->prioridade += 1;
    atual->meuHash = hash;
    atual->completo = true;
}

void Trie::adicionarReport(const string_view &hash, const Chamado &c){
    NoTrie *atual = raiz;
    int pos = -1;

    for(char c : hash){
        //Se for realmente um num...
        pos = c - '0';
        //Se não...
        if(pos >= 10) {
            pos = c - 'a' + 10;
        }
        
        if(atual->filhos[pos] == nullptr){
            atual->filhos[pos] = new NoTrie();
        }
        
        atual = atual->filhos[pos];
        //atual->meuHash = 
    }
    
    switch(c.tipo){
        case Tipo::Iluminacao:
            atual->prioridadeIluminacao += 1;
        break;
        case Tipo::Agua:
            atual->prioridadeAgua += 1;
        break;
        case Tipo::Buraco:
            atual->prioridadeBuraco += 1;
        break;
        default: cout << "Houve erro." << endl;
    }
    
    atual->prioridade += 1;
    atual->meuHash = hash;
    atual->completo = true;
}




void Trie::procurarReport(const string &hash){
    NoTrie *atual = raiz;
    int pos;
    
    for(char c : hash){
        //Se for realmente um num...
        pos = c - '0';
        //Se não...
        if(pos >= 10) {
            pos = c - 'a' + 10;
        }
        
        if(atual->filhos[pos] == nullptr){
            cout << "Reclamações para " << hash << " não existem.\n"; 
        }
        atual = atual->filhos[pos];
    }
    
    if(atual != nullptr && atual->completo == true){
        cout << "Número de reclamções para " << hash << ": " << atual->prioridade << endl;
    }else cout << "Reclamações para " << hash << " não existem.\n";
}

void Trie::procurarReport(const string_view &hash){
    NoTrie *atual = raiz;
    int pos;
    
    for(char c : hash){
        //Se for realmente um num...
        pos = c - '0';
        //Se não...
        if(pos >= 10) {
            pos = c - 'a' + 10;
        }
        
        if(atual->filhos[pos] == nullptr){
            cout << "Reclamações para " << hash << " não existem.\n"; 
        }
        atual = atual->filhos[pos];
    }
    
    if(atual != nullptr && atual->completo == true){
        cout << "Número de reclamções para " << hash << ": " << atual->prioridade << endl;
    }else cout << "Reclamações para " << hash << " não existem.\n";
}



void Trie::procurarPrefixo(const string &pfx){
    NoTrie *atual = raiz;
    int pos;
    
    for(char c : pfx){
        //Se for realmente um num...
        pos = c - '0';
        //Se não...
        if(pos >= 10) {
            pos = c - 'a' + 10;
        }
        
        if(atual->filhos[pos] == nullptr){
            cout << "Não existem reclamações para a área de prefixo: " << pfx << " \n"; 
        }
        atual = atual->filhos[pos];
    }
    
    if(atual != nullptr) {
    cout << "Existe pelo menos uma reclamação para a área de prefixo: " << pfx << " \n";
    }
}

void Trie::procurarPrefixoV(const string_view &pfx){
    NoTrie *atual = raiz;
    int pos;
    
    for(char c : pfx){
        //Se for realmente um num...
        pos = c - '0';
        //Se não...
        if(pos >= 10) {
            pos = c - 'a' + 10;
        }
        
        if(atual->filhos[pos] == nullptr){
            cout << "Não existem reclamações para a área de prefixo: " << pfx << " \n"; 
        }
        atual = atual->filhos[pos];
    }
    
    if(atual != nullptr) {
    cout << "Existe pelo menos uma reclamação para a área de prefixo: " << pfx << " \n";
    }
}
