#ifndef BASELINE_H
#define BASELINE_H

#include <vector>
#include "chamado.h"
#include "geohash.h"

class Baseline {
private:
    std::vector<Chamado> chamados;

public:
    Baseline() = default;

    bool registrarChamado(const Chamado& c, double raio_dedup) {
        for (Chamado& existente : chamados) {
            if (existente.tipo == c.tipo &&
                GeoHash::distance(existente.local, c.local, GeoHash::EARTH_METERS) <= raio_dedup) {
                existente.relevancia++;
                return true; 
            }
        }
        chamados.push_back(c);
        return false;
    }

    std::vector<Chamado*> consultarRaio(GeoHash::Point centro, double raio) {
        std::vector<Chamado*> resultados;
        for (Chamado& c : chamados) {
            if (GeoHash::distance(centro, c.local, GeoHash::EARTH_METERS) <= raio) {
                resultados.push_back(&c);
            }
        }
        return resultados;
    }
};

#endif