#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <string_view>
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
    delete no;
}
    
Trie::Trie(size_t p){
    raiz = new NoTrie();
    precisao = p;
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
    
    /*
    switch(c.tipo){
        case Tipo::Iluminacao:
            atual->prioridadeIluminacao += c.relevancia;
        break;
        case Tipo::Agua:
            atual->prioridadeAgua += c.relevancia;
        break;
        case Tipo::Buraco:
            atual->prioridadeBuraco += c.relevancia;
        break;
        default: cout << "Houve erro." << endl;
    }
    */
    
    //atual->prioridade += 1;
    atual->meuHash = hash;
    atual->completo = true;
    atual->chamados.push_back(c);
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
    
    /*
    switch(c.tipo){
        case Tipo::Iluminacao:
            atual->prioridadeIluminacao += c.relevancia;
        break;
        case Tipo::Agua:
            atual->prioridadeAgua += c.relevancia;
        break;
        case Tipo::Buraco:
            atual->prioridadeBuraco += c.relevancia;
        break;
        default: cout << "Houve erro." << endl;
    }
    */
    
    //atual->prioridade += 1;
    atual->meuHash = hash;
    atual->completo = true;
    atual->chamados.push_back(c);
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
            return;
        }
        atual = atual->filhos[pos];
    }
    
    /*
    if(atual != nullptr && atual->completo == true){
        cout << "Número de reclamações total para " << hash << ": " << atual->prioridade << endl;
        cout << "Iluminação: " << atual->prioridadeIluminacao << " | Água: " << atual->prioridadeAgua << " | Buraco: " << atual->prioridadeBuraco << endl;
    }else cout << "Reclamações para " << hash << " não existem.\n";
    */
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
            return; 
        }
        atual = atual->filhos[pos];
    }
    
    /*
    if(atual != nullptr && atual->completo == true){
        cout << "Número de reclamações total para " << hash << ": " << atual->prioridade << endl;
        cout << "Iluminação: " << atual->prioridadeIluminacao << " | Água: " << atual->prioridadeAgua << " | Buraco: " << atual->prioridadeBuraco << endl;
    }else cout << "Reclamações para " << hash << " não existem.\n";
    */
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
            return;
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
            return;
        }
        atual = atual->filhos[pos];
    }
    
    if(atual != nullptr) {
    cout << "Existe pelo menos uma reclamação para a área de prefixo: " << pfx << " \n";
    }
}


bool Trie::procurarSilencioso(const string &hash){
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
            //cout << "Reclamações para " << hash << " não existem.\n"; 
            return false;
        }
        atual = atual->filhos[pos];
    }
    
    if(atual != nullptr && atual->completo == true){
        return true;
    }else return false;
}


bool Trie::procurarSilencioso(const string_view &hash){
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
            //cout << "Reclamações para " << hash << " não existem.\n";
            return false;
        }
        atual = atual->filhos[pos];
    }
    
    if(atual != nullptr && atual->completo == true){
        return true;
    }else return false;
}

void Trie::coletar(NoTrie* no, GeoHash::Point centro, double raio, vector<Chamado*>& resultados) {
    if (!no) {
        return;
    }

    for (Chamado& c : no->chamados) {
        if (GeoHash::distance(centro, c.local, GeoHash::EARTH_METERS) <= raio) {
            resultados.push_back(&c);
        }
    }

    for(int i = 0; i < 36; i++){
        if(no->filhos[i] != nullptr){
            coletar(no->filhos[i], centro, raio, resultados);
        }
    }
}

vector<Chamado*> Trie::consultarRaio(GeoHash::Point centro, double raio) {
    vector<Chamado*> resultados;
    GeoHash::HashVector hash = GeoHash::nearbyCells(centro, raio, GeoHash::EARTH_METERS);

    NoTrie *atual = raiz;
    int pos = -1;

    for(std::string_view c : hash){

        if (c.size() > precisao) {
            c = c.substr(0, precisao);
        }

        NoTrie* atual = raiz;

        for (char ch : c) {
            //Se for realmente um num...
            pos = ch - '0';
            //Se não...
            if(pos >= 10) {
                pos = ch - 'a' + 10;
            }
            
            if(atual->filhos[pos] == nullptr){
                atual = nullptr;
                break;
            }
            atual = atual->filhos[pos];
        }

        if (atual != nullptr) {
            coletar(atual, centro, raio, resultados);
        }
    }
    return resultados;
}

bool Trie::registrarChamado(const Chamado& c, double raio_dedup) {
    auto proximos = consultarRaio(c.local, raio_dedup);
    for (Chamado* existente : proximos) {
        if (existente->tipo == c.tipo) {
            existente->relevancia++;
            return true; 
        }
    }
    GeoHash::buffer_t buffer;
    auto hash = GeoHash::encode(c.local, precisao, buffer);
    adicionarReport(hash, c);
    return false; 
}