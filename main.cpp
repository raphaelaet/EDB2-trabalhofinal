#include "geohash.h"
#include "trie.h"
#include "quadtree.h"

#include <iostream>
#include <utility>
#include <random>

struct Place{
	GeoHash::Point		point;
	std::string_view	name;
	double			supposed_distance;

	GeoHash::buffer_t	buffer;

	std::string_view	hash = GeoHash::encode(point, 12, buffer);
};

Place const places[] {
	{ { 42.69210669738513, 23.32489318092748 }, "Level Up"			,   10 },
	{ { 42.69238509863727, 23.32476304836964 }, "Luchiano"			,   42 },
	{ { 42.69175842097124, 23.32523636032996 }, "Bohemian Hall"		,   42 },
	{ { 42.69269382372572, 23.32491086796810 }, "Umamido"			,   74 },
	{ { 42.69199298885176, 23.32411779452691 }, "McDonald's"		,   74 },
	{ { 42.69141486008349, 23.32517469905046 }, "Ceiba"			,   74 },
	{ { 42.69210516803149, 23.32626916568055 }, "Happy"			,  110 },
	{ { 42.69151957227103, 23.32366568780231 }, "Ugo"			,  130 },
	{ { 42.69151957227103, 23.32654779876320 }, "Il Theatro"		,  140 },
	{ { 42.69205716858902, 23.32266435016691 }, "Nicolas"			,  200 },
	{ { 42.68970992959111, 23.32455331244497 }, "Manastirska Magernitsa"	,  300 },
	{ { 42.69325388808822, 23.32118939756011 }, "The Hadjidragana Tavern"	,  330 },
	{ { 42.69174997126510, 23.32100561258516 }, "Boho"			,  330 },
	{ { 42.69205885495273, 23.33781274367048 }, "Stastlivetza"		,  400 },
	{ { 42.68999839888917, 23.31970366848597 }, "Franco's Pizza"		,  500 },
	{ { 42.69074017093236, 23.31785351529929 }, "Villa Rosiche"		,  620 },
	{ { 42.68342508736217, 23.31633975360111 }, "Chevermeto"		, 1200 }
};

constexpr GeoHash::Point me{ 42.69207945916779, 23.325029867108032 };

namespace{

	void mySearchNearby(GeoHash::Point me, double radius, GeoHash::GeoSphere const &sphere){
		auto const cells = GeoHash::nearbyCells(me, radius, sphere);

		for(auto &hash : cells){
			std::cout << "Searching cell: " << hash << '\n';

			// send query to DB
			for(auto &p : places){
				if (p.hash.size() < hash.size() || std::equal(std::begin(hash), std::end(hash), std::begin(p.hash)) == false)
					continue;

				auto const dist = distance(me, p.point, sphere);

				if (dist > radius)
					continue;

				std::cout << '\t' << p.name << '\t' << dist << ' ' << sphere.units << '\n';
			}
		}
	}

}

// if for some strange reason we want distances in decameters...
//constexpr auto UNITS = GeoHash::GeoSphere{ "Earth", "dm", GeoHash::EARTH_METERS.radius / 10 };
//constexpr auto UNITS = GeoHash::EARTH_YA;

constexpr auto UNITS = GeoHash::EARTH_METERS;

//Macros

void macroAdicionarTrie(Trie *raiz, GeoHash::Point point, int tests){
    cout << "Entrou.\n";
    string Hash, Hash2;
    GeoHash::buffer_t buffer;
    Chamado ilum = {0, Tipo::Iluminacao, {0, 0}};
	Chamado agua = {0, Tipo::Agua, {0, 0}};
	Chamado bur = {0, Tipo::Buraco, {0, 0}};
	
	Hash = GeoHash::encode(point, 12, buffer);
        
    raiz->adicionarReport(Hash, ilum);
    raiz->adicionarReport(Hash, agua);
    raiz->adicionarReport(Hash, bur);
	
	GeoHash::Point point2 = point;
    
    for(int i = 0; i < tests; i++){
        point.lat += 0.11;
        point.lon += 0.11;
        point2.lat -= 0.11;
        point2.lon -= 0.11;
        
        if(point.lat > 90.0) point.lat = (-90.0 + point.lat / 11.0);
        if(point.lon > 180.0) point.lon = (-180.0 + point.lon / 17.0);
        if(point.lat < -90.0) point.lat = (90.0 - point.lat / 11.0);
        if(point.lon < -180.0) point.lon = (180.0 - point.lon / 17.0);
        
        Hash = GeoHash::encode(point, 12, buffer);
        Hash2 = GeoHash::encode(point2, 12, buffer);
        
        raiz->adicionarReport(Hash, ilum);
        raiz->adicionarReport(Hash, agua);
        raiz->adicionarReport(Hash, bur);
        raiz->adicionarReport(Hash2, ilum);
        raiz->adicionarReport(Hash2, agua);
        raiz->adicionarReport(Hash2, bur);
    }
    
    cout << "Foi.\n";
}


int main(){
	// 80 m - 3x3
	// 60 m - 2x3
	// 50 m - 2x2
	// 12 m - 3x2
	// auto const radius = 80;
	
	/*
	Chamado c1 = {0, Tipo::Iluminacao, {42.69210669738513, 23.32489318092748}};
	Chamado c2 = {0, Tipo::Agua, {42.69210669738513, 23.32489318092748}};
	Chamado c3 = {0, Tipo::Buraco, {42.69210669738513, 23.32489318092748}};
	*/

	Trie trie(8);

	cout << "Resultados Trie" << endl;

	Chamado c4 = {1, Tipo::Buraco, {-5.833597, -35.203036}};
	Chamado c5 = {2, Tipo::Buraco, {-5.833795, -35.203038}}; // Esperado que sofra deduplicação por estar próximo do c4 e ser do mesmo tipo
	Chamado c6 = {3, Tipo::Buraco ,{-5.834980, -35.203048}};

	cout << trie.registrarChamado(c4, 30.0) << endl;
	cout << trie.registrarChamado(c5, 30.0) << endl;
	cout << trie.registrarChamado(c6, 30.0) << endl;

	cout << "Resultados QuadTree" << endl;

	QuadTree qt({{-6.0, -35.4}, {-5.7, -35.1}});
	
	cout << qt.registrarChamado(c4, 30.0) << endl;
	cout << qt.registrarChamado(c5, 30.0) << endl;
	cout << qt.registrarChamado(c6, 30.0) << endl;


	
	/*
	GeoHash::Point testPoint = { 42.69210669738513, 23.32489318092748 };
	GeoHash::buffer_t testbuffer;
	Trie raiz;
	*/
	
	//Podes tanto criar uma string_view quanto transformar em string (segunda linha).
	//A primeira linha é a criação de um GeoHash sem ser baseado no struct "place" (local).
	//string_view testHash = GeoHash::encode(testPoint, 12, testbuffer);
	//string tHashString = string(testHash);
	
	//raiz.adicionarReport(testHash, c1);
	//raiz.adicionarReport(testHash, c2);
	//raiz.adicionarReport(testHash, c3);
	//raiz.procurarReport(testHash); //Executar procurarReport sem ter nada adicionado causa problemas
	//Para poder usar escrita direta assim tive que diferenciar a busca entre string_view e string colocando "V" no final do nome do método para string_view
    //raiz.procurarPrefixo("sx8");
    
    //Limite atual testado: 325000
    //macroAdicionarTrie(&raiz, testPoint, 325000);
    //raiz.procurarReport(testHash);
	
	//cout << testHash << endl;

	//mySearchNearby(me, radius, UNITS);
}
