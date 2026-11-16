#pragma once
#include "geohash.h"

using namespace std;

enum class Tipo {
	Iluminacao,
	Agua,
	Buraco
};

struct Chamado {
    int id;
	Tipo tipo;
    GeoHash::Point local;
    int relevancia = 1;
};