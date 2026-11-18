#pragma once

#include "geohash.h"
#include "chamado.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

// QuadTree = "árvore de quatro filhos".
//
// Ideia: o mapa é um retângulo. Cada retângulo (nó) guarda até `capacidade`
// chamados. Quando passa disso, ele é dividido em 4 retângulos menores
// (noroeste, nordeste, sudoeste, sudeste) e os chamados vão para eles.
// Para buscar perto de um ponto, basta olhar os retângulos que encostam
// na área da busca, em vez de percorrer todos os chamados.
class QuadTree {
private:
    // Constantes
    static constexpr double METROS_POR_GRAU = 111320.0; // 1 grau de latitude ~ 111 km
    static constexpr double PI = 3.14159265358979323846;

    // Um nó = um retângulo do mapa
    struct No {
        GeoHash::Rectangle limite;               // área coberta por este nó
        std::vector<Chamado> chamados;           // chamados guardados aqui (só se for folha)
        std::unique_ptr<No> filhos[4];           // 0=NO, 1=NE, 2=SO, 3=SE (vazios se for folha)

        No(GeoHash::Rectangle area) : limite(area) {}

        bool ehFolha() const { return filhos[0] == nullptr; }
    };

    std::unique_ptr<No> raiz;
    size_t capacidade;   // máximo de chamados por folha antes de dividir
    size_t prof_maxima;  // limite de divisões (evita dividir para sempre)

    // ---------- Funções auxiliares ----------

    // O ponto está dentro do retângulo?
    static bool contem(const GeoHash::Rectangle& r, const GeoHash::Point& p) {
        return p.lat >= r.sw.lat && p.lat <= r.ne.lat &&
               p.lon >= r.sw.lon && p.lon <= r.ne.lon;
    }

    // Os dois retângulos se encostam/sobrepõem?
    static bool intercepta(const GeoHash::Rectangle& a, const GeoHash::Rectangle& b) {
        bool separados = a.ne.lon < b.sw.lon || a.sw.lon > b.ne.lon ||
                         a.ne.lat < b.sw.lat || a.sw.lat > b.ne.lat;
        return !separados;
    }

    // Em qual dos 4 filhos o ponto cai? Devolve 0..3, ou -1 se em nenhum.
    static int quadranteDo(const No* no, const GeoHash::Point& p) {
        for (int i = 0; i < 4; i++) {
            if (contem(no->filhos[i]->limite, p)) return i;
        }
        return -1;
    }

    // Divide um nó em 4 filhos e passa os chamados dele para os filhos.
    void dividir(No* no) {
        const auto& r = no->limite;
        double meioLat = (r.sw.lat + r.ne.lat) / 2.0;
        double meioLon = (r.sw.lon + r.ne.lon) / 2.0;

        // Rectangle = { canto sudoeste, canto nordeste }
        no->filhos[0] = std::make_unique<No>(GeoHash::Rectangle{ {meioLat, r.sw.lon}, {r.ne.lat, meioLon} }); // noroeste
        no->filhos[1] = std::make_unique<No>(GeoHash::Rectangle{ {meioLat, meioLon}, {r.ne.lat, r.ne.lon} }); // nordeste
        no->filhos[2] = std::make_unique<No>(GeoHash::Rectangle{ {r.sw.lat, r.sw.lon}, {meioLat, meioLon} }); // sudoeste
        no->filhos[3] = std::make_unique<No>(GeoHash::Rectangle{ {r.sw.lat, meioLon}, {meioLat, r.ne.lon} }); // sudeste

        // Os chamados antigos descem para o filho certo
        for (const Chamado& c : no->chamados) {
            int q = quadranteDo(no, c.local);
            if (q != -1) no->filhos[q]->chamados.push_back(c);
        }
        no->chamados.clear();
    }

    // Coloca um chamado na árvore. Devolve false se o ponto está fora do mapa.
    bool inserir(No* no, const Chamado& c, size_t profundidade) {
        if (!contem(no->limite, c.local)) {
            return false;
        }

        // Já foi dividido: desce para o filho certo
        if (!no->ehFolha()) {
            int q = quadranteDo(no, c.local);
            return inserir(no->filhos[q].get(), c, profundidade + 1);
        }

        // Folha com espaço (ou no limite de profundidade): guarda aqui
        if (no->chamados.size() < capacidade || profundidade >= prof_maxima) {
            no->chamados.push_back(c);
            return true;
        }

        // Folha cheia: divide e tenta de novo
        dividir(no);
        return inserir(no, c, profundidade);
    }

    // Procura chamados a até `raio` metros de `centro`
    // `area` é um retângulo em volta do centro, usado só para pular regiões longe demais
    void buscar(No* no, const GeoHash::Rectangle& area, const GeoHash::Point& centro,
                double raio, std::vector<Chamado*>& resultados) {
        // Se o retângulo está longe da busca ele é ignorado
        if (!intercepta(no->limite, area)) {
            return;
        }

        // Nó dividido: pergunta para os 4 filhos
        if (!no->ehFolha()) {
            for (auto& filho : no->filhos) {
                buscar(filho.get(), area, centro, raio, resultados);
            }
            return;
        }

        // Folha: confere a distância real de cada chamado
        for (Chamado& c : no->chamados) {
            if (GeoHash::distance(centro, c.local, GeoHash::EARTH_METERS) <= raio) {
                resultados.push_back(&c);
            }
        }
    }

public:
    // `limite_mapa` = região que a árvore cobre. Chamados fora dela não são guardados.
    QuadTree(GeoHash::Rectangle limite_mapa, size_t cap = 8, size_t prof_max = 14)
        : raiz(std::make_unique<No>(limite_mapa)), capacidade(cap), prof_maxima(prof_max) {}

    // Devolve os chamados a até `raio` metros de `centro`.
    std::vector<Chamado*> consultarRaio(GeoHash::Point centro, double raio) {
        // Converte o raio de metros para graus e monta um retângulo em volta do centro.
        double raioEmLat = raio / METROS_POR_GRAU;
        double raioEmLon = raio / (METROS_POR_GRAU * std::cos(centro.lat * PI / 180.0));

        GeoHash::Rectangle area = {
            {centro.lat - raioEmLat, centro.lon - raioEmLon},
            {centro.lat + raioEmLat, centro.lon + raioEmLon}
        };

        std::vector<Chamado*> resultados;
        buscar(raiz.get(), area, centro, raio, resultados);
        return resultados;
    }

    // Registra um chamado
    // Se já existe um do mesmo tipo a até `raio_dedup` metros, só aumenta a relevância dele
    // e devolve true. Caso contrário, guarda o chamado novo e devolve false.
    bool registrarChamado(const Chamado& c, double raio_dedup) {
        for (Chamado* existente : consultarRaio(c.local, raio_dedup)) {
            if (existente->tipo == c.tipo) {
                existente->relevancia++;
                return true;
            }
        }

        if (!inserir(raiz.get(), c, 0)) {
            std::cerr << "Aviso: chamado " << c.id << " esta fora do mapa da QuadTree e nao foi guardado.\n";
        }
        return false;
    }
};
