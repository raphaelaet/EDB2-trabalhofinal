#include "geohash.h"
#include "trie.h"
#include "quadtree.h"

#include <iostream>
#include <utility>

struct Place{
	GeoHash::Point		point;
	std::string_view	name;
	double			supposed_distance;

	GeoHash::buffer_t	buffer;

	std::string_view	hash = GeoHash::encode(point, 12, buffer);
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

int main(){
	// 80 m - 3x3
	// 60 m - 2x3
	// 50 m - 2x2
	// 12 m - 3x2
	auto const radius = 80;
	
	Chamado c1 = {0, Tipo::Iluminacao, {42.69210669738513, 23.32489318092748}};
	Chamado c2 = {0, Tipo::Agua, {42.69210669738513, 23.32489318092748}};
	Chamado c3 = {0, Tipo::Buraco, {42.69210669738513, 23.32489318092748}};
	
	GeoHash::Point testPoint = { 42.69210669738513, 23.32489318092748 };
	GeoHash::buffer_t testbuffer;
	Trie raiz;
	
	//Podes tanto criar uma string_view quanto transformar em string (segunda linha).
	//A primeira linha é a criação de um GeoHash sem ser baseado no struct "place" (local).
	string_view testHash = GeoHash::encode(testPoint, 12, testbuffer);
	string tHashString = string(testHash);
	
	raiz.adicionarReport(testHash, c1);
	raiz.adicionarReport(testHash, c2);
	raiz.adicionarReport(testHash, c3);
	raiz.procurarReport(testHash);
	//Para poder usar escrita direta assim tive que diferenciar a busca entre string_view e string colocando "V" no final do nome do método para string_view
    raiz.procurarPrefixo("sx8");
	
	//cout << testHash << endl;

	//mySearchNearby(me, radius, UNITS);
}

