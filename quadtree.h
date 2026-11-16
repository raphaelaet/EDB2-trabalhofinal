#pragma once

#include "geohash.h"
#include "chamado.h"
#include <memory>
#include <vector>
#include <cmath>

class QuadTree {
private:
    struct Node {
        GeoHash::Rectangle limite;
        vector<Chamado> chamados;
        bool dividida = false;
        unique_ptr<Node> nw, ne, sw, se;

        Node(GeoHash::Rectangle lim) : limite(lim) {}
    };

    unique_ptr<Node> raiz;
    size_t capacidade; 
    size_t prof_maxima;

    // Verifica se uma coordenada (lat, lon) está dentro dos limites do retângulo r
    bool contem(const GeoHash::Rectangle& r, const GeoHash::Point& p) const {
        return (p.lat >= r.sw.lat && p.lat <= r.ne.lat && p.lon >= r.sw.lon && p.lon <= r.ne.lon);
    }

    // Verifica se dois retângulos se sobrepõem
    bool intercepta(const GeoHash::Rectangle& a, const GeoHash::Rectangle& b) const {
        return !(a.ne.lon < b.sw.lon || a.sw.lon > b.ne.lon || a.ne.lat < b.sw.lat || a.sw.lat > b.ne.lat);
    }

    // Divide o nó atual em 4 sub-quadrantes e redistribui seus chamados
    void subdividir(Node* node) {
        double midLat = (node->limite.sw.lat + node->limite.ne.lat) / 2.0;
        double midLon = (node->limite.sw.lon + node->limite.ne.lon) / 2.0;

        node->nw = make_unique<Node>(GeoHash::Rectangle {
            {midLat, node->limite.sw.lon},
            {node->limite.ne.lat, midLon}
        });

        node->ne = make_unique<Node>(GeoHash::Rectangle {
            {midLat, midLon},
            {node->limite.ne.lat, node->limite.ne.lon}
        });

        node->sw = make_unique<Node>(GeoHash::Rectangle {
            {node->limite.sw.lat, node->limite.sw.lon},
            {midLat, midLon}
        });

        node->se = make_unique<Node>(GeoHash::Rectangle {
            {node->limite.sw.lat, midLon},
            {midLat, node->limite.ne.lon}
        });

        node->dividida = true;

        for (const auto& c : node->chamados) {
            if (contem(node->nw->limite, c.local)) {
                node->nw->chamados.push_back(c);
            }
            else if (contem(node->ne->limite, c.local)) {
                node->ne->chamados.push_back(c);
            }
            else if (contem(node->sw->limite, c.local)) {
                node->sw->chamados.push_back(c);
            }
            else if (contem(node->se->limite, c.local)) {
                node->se->chamados.push_back(c);
            }
        }

        node->chamados.clear();
        
    }

    // Insere recursivamente um chamado na folha correspondente subdividindo se necessário até atingir o limite de profundidade
    void inserir(Node* node, const Chamado& c, size_t prof) {
        if (node->dividida) {
            if (contem(node->nw->limite, c.local)) {
                inserir(node->nw.get(), c, prof + 1);
            }
            else if (contem(node->ne->limite, c.local)) {
                inserir(node->ne.get(), c, prof + 1);
            }
            else if (contem(node->sw->limite, c.local)) {
                inserir(node->sw.get(), c, prof + 1);
            }
            else if (contem(node->se->limite, c.local)) {
                inserir(node->se.get(), c, prof + 1);
            }
        }
        else if (!node->dividida && (node->chamados.size() < capacidade || prof >= prof_maxima)) {
            node->chamados.push_back(c);
        }
        else {
            subdividir(node);
            inserir(node, c, prof);
        }
    }

    // Percorre apenas os quadrantes que interceptam areaBusca e filtra os chamados pela distância geodésica até o ponto centro
    void buscar(Node* node, GeoHash::Rectangle areaBusca, GeoHash::Point centro, double raio, vector<Chamado*>& resultados) {
        if (!intercepta(node->limite, areaBusca)) {
            return;
        }
        else if (node->dividida) {
            buscar(node->nw.get(), areaBusca, centro, raio, resultados);
            buscar(node->ne.get(), areaBusca, centro, raio, resultados);
            buscar(node->sw.get(), areaBusca, centro, raio, resultados);
            buscar(node->se.get(), areaBusca, centro, raio, resultados);
        }
        else {
            for (auto& c : node->chamados) {
                if (GeoHash::distance(centro, c.local, GeoHash::EARTH_METERS) <= raio) {
                    resultados.push_back(&c);
                }
            }
        }
    };

    public:

        QuadTree(GeoHash::Rectangle limite_mapa, size_t cap = 8, size_t prof_max = 14) : 
            capacidade(cap), prof_maxima(prof_max) {
            raiz = make_unique<Node>(limite_mapa); // make_unique é como se fosse o new, descobri que funciona melhor nesse caso
        }

        vector<Chamado*> consultarRaio(GeoHash::Point centro, double raio) {
            vector<Chamado*> resultados;
            double dLat = raio / 111320.0; // fórmula de conversão de coordenadas
            double dLon = raio / (111320.0 * cos(centro.lat * M_PI / 180.0));

            GeoHash::Rectangle r = { {centro.lat - dLat, centro.lon - dLon}, {centro.lat + dLat, centro.lon + dLon} };

            buscar(raiz.get(), r, centro, raio, resultados);
            return resultados;
        }

        bool registrarChamado(const Chamado& c, double raio_dedup) {
            auto proximos = consultarRaio(c.local, raio_dedup);
            for (Chamado* existente : proximos) {
                if (existente->tipo == c.tipo) {
                    existente->relevancia++;
                    return true; 
                }
            }
            inserir(raiz.get(), c, 0);
            return false; 
        }
};
