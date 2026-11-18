#include "geohash.h"
#include "trie.h"
#include "quadtree.h"
#include "baseline.h"

#include <iostream>
#include <utility>
#include <random>
#include <chrono>

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

template <typename Estrutura>
void macroAdicionar(Estrutura *estrutura, GeoHash::Point point, int tests){
    cout << "Entrou.\n";
    //string Hash, Hash2;
    GeoHash::buffer_t buffer;
    int id = 1;
    Chamado ilum = {id++, Tipo::Iluminacao, point, 1};
	Chamado agua = {id++, Tipo::Agua, point, 1};
	Chamado bur = {id++, Tipo::Buraco, point, 1};
	//Chamado ilum2 = {0, Tipo::Iluminacao, point};
	//Chamado agua2 = {0, Tipo::Agua, point};
	//Chamado bur2 = {0, Tipo::Buraco, point};
	
	//Hash = GeoHash::encode(point, 12, buffer);
        
    //raiz->adicionarReport(Hash, ilum);
    //raiz->adicionarReport(Hash, agua);
    //raiz->adicionarReport(Hash, bur);
    
    auto inicio = chrono::steady_clock::now();
	
	//Hash = GeoHash::encode(point, 12, buffer);
    
    estrutura->registrarChamado(ilum, 30.0);
    estrutura->registrarChamado(agua, 30.0);
    estrutura->registrarChamado(bur, 30.0);
	
	//GeoHash::Point point2 = point;
    
    for(int i = 0; i < tests; i++){
        point.lat += 0.00037;
        point.lon += 0.00053;
        //point2.lat -= 0.11;
        //point2.lon -= 0.11;
        
        if(point.lat > -5.70) point.lat -= 0.20;
        if(point.lon > -35.15) point.lon -= 0.14;
        //if(point.lat < -90.0) point.lat = (90.0 + point.lat / 11.0);
        //if(point.lon < -180.0) point.lon = (180.0 + point.lon / 17.0);
        
        /*if(point2.lat > 90.0) point2.lat = (-90.0 + point2.lat / 11.0);
        if(point2.lon > 180.0) point2.lon = (-180.0 + point2.lon / 17.0);
        if(point2.lat < -90.0) point2.lat = (90.0 - point2.lat / 11.0);
        if(point2.lon < -180.0) point2.lon = (180.0 - point2.lon / 17.0);*/
        
        ilum = {id++, Tipo::Iluminacao, point, 1};
        agua = {id++, Tipo::Agua, point, 1};
	    bur = {id++, Tipo::Buraco, point, 1};
	       
	    estrutura->registrarChamado(ilum, 30.0);
        estrutura->registrarChamado(agua, 30.0);
        estrutura->registrarChamado(bur, 30.0);
        
        /*ilum2 = {0, Tipo::Iluminacao, point2};
        agua2 = {0, Tipo::Agua, point2};
	    bur2 = {0, Tipo::Buraco, point2};
        
        raiz->registrarChamado(ilum2, 30.0);
        raiz->registrarChamado(agua2, 30.0);
        raiz->registrarChamado(bur2, 30.0);*/
        
        /*Hash = GeoHash::encode(point, 12, buffer);
        Hash2 = GeoHash::encode(point2, 12, buffer);
        
        raiz->adicionarReport(Hash, ilum);
        raiz->adicionarReport(Hash, agua);
        raiz->adicionarReport(Hash, bur);
        raiz->adicionarReport(Hash2, ilum);
        raiz->adicionarReport(Hash2, agua);
        raiz->adicionarReport(Hash2, bur);*/
    }
    
    auto fim = chrono::steady_clock::now();
    
    auto total = fim - inicio;
    
    cout << "Completo. Tempo: " << chrono::duration_cast<chrono::milliseconds>(total).count() << "ms.\n";
}

template <typename Estrutura>
void macroBuscar(Estrutura *estrutura, GeoHash::Point point, int tests){
    cout << "Entrou (busca).\n";
    size_t totalAchados = 0;
    //string Hash, Hash2;
    //GeoHash::buffer_t buffer;
    
    auto inicio = chrono::steady_clock::now();
	
	//Hash = GeoHash::encode(point, 8, buffer);
	//Hash2 = GeoHash::encode(point, 8, buffer);
        
    //raiz->consultarRaio(point, 500.0);
    totalAchados += estrutura->consultarRaio(point, 500.0).size();
	
	//GeoHash::Point point2 = point;
    
    for(int i = 0; i < tests; i++){
        point.lat += 0.0019;
        point.lon += 0.0023;
        //point2.lat -= 0.11;
        //point2.lon -= 0.11;
        
        if(point.lat > -5.70) point.lat -= 0.20;
        if(point.lon > -35.15) point.lon -= 0.14;
        //if(point.lat < -5.90) point.lat += 0.00037;
        //if(point.lon < -35.29) point.lon += 0.00053;
        
        /*if(point2.lat > 90.0) point2.lat = (-90.0 + point2.lat / 11.0);
        if(point2.lon > 180.0) point2.lon = (-180.0 + point2.lon / 17.0);
        if(point2.lat < -90.0) point2.lat = (90.0 - point2.lat / 11.0);
        if(point2.lon < -180.0) point2.lon = (180.0 - point2.lon / 17.0);*/
        
        //Hash = GeoHash::encode(point, 8, buffer);
        //Hash2 = GeoHash::encode(point2, 8, buffer);
        
        totalAchados += estrutura->consultarRaio(point, 500.0).size();

        /*if(raiz->procurarSilencioso(Hash) == false){
            cout << "Algo deu errado.\n";
            break;
        }*/
        /*if(raiz->procurarSilencioso(Hash2) == false){
            cout << "Algo deu errado2.\n";
            break;
        }*/
        
        /*if(i == 100000){
            raiz->procurarReport(Hash);
            raiz->procurarReport(Hash2);
        }
        if(i == 250000){
            raiz->procurarReport(Hash);
            raiz->procurarReport(Hash2);
        }
        if(i == 325000 - 1){
            raiz->procurarReport(Hash);
            raiz->procurarReport(Hash2);
        }*/
    }
    auto fim = chrono::steady_clock::now();
    
    auto total = fim - inicio;
    
    cout << "Completo. Tempo: " << chrono::duration_cast<chrono::milliseconds>(total).count() << "ms. | Total achados: " << totalAchados << "\n";
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
	QuadTree qt({{-6.0, -35.4}, {-5.7, -35.1}});
    Baseline bl;

	//As linhas até o último "registrarChamado" podem ser comentadas
    /*
	cout << "Resultados Trie" << endl;

	Chamado c4 = {1, Tipo::Buraco, {-5.833597, -35.203036}};
	Chamado c5 = {2, Tipo::Buraco, {-5.833795, -35.203038}}; // Esperado que sofra deduplicação por estar próximo do c4 e ser do mesmo tipo
	Chamado c6 = {3, Tipo::Buraco, {-5.834980, -35.203048}};

	cout << trie.registrarChamado(c4, 30.0) << endl;
	cout << trie.registrarChamado(c5, 30.0) << endl;
	cout << trie.registrarChamado(c6, 30.0) << endl;

	cout << "Resultados QuadTree" << endl;

	QuadTree qt({{-6.0, -35.4}, {-5.7, -35.1}});
	
	cout << qt.registrarChamado(c4, 30.0) << endl;
	cout << qt.registrarChamado(c5, 30.0) << endl;
	cout << qt.registrarChamado(c6, 30.0) << endl;
    */
	
	GeoHash::buffer_t testbuffer;
	GeoHash::Point testPoint = { -5.833597, -35.203036 };
	string_view testHash = GeoHash::encode(testPoint, 8, testbuffer);
	
	macroAdicionar(&trie, testPoint, 5000);
    macroBuscar(&trie, testPoint, 5000);

    //trie.procurarReport(testHash);
    
	macroAdicionar(&qt, testPoint, 5000);
    macroBuscar(&qt, testPoint, 5000);

    macroAdicionar(&bl, testPoint, 5000);
    macroBuscar(&bl, testPoint, 5000);
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
