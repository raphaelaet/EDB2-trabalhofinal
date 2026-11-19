#include "geohash.h"
#include "trie.h"
#include "quadtree.h"
#include "baseline.h"

#include <iostream>
#include <random>
#include <chrono>

GeoHash::Point deslocar(GeoHash::Point p, double norteM, double lesteM) {
    return {p.lat + norteM / 111320.0,
            p.lon + lesteM / (111320.0 * cos(p.lat * 3.14159265358979 / 180.0))};
}

template <typename Estrutura>
void macroAdicionar(Estrutura *estrutura, GeoHash::Point point, int tests){
    cout << "Entrou.\n";
    int id = 1;
    Chamado ilum = {id++, Tipo::Iluminacao, point, 1};
	Chamado agua = {id++, Tipo::Agua, point, 1};
	Chamado bur = {id++, Tipo::Buraco, point, 1};

    const int relatosPorDefeito = 3; // Ou seja, 3 pessoas diferentes fizeram uma reclamação do mesmo problema. Pode ser alterado.
    const double ruidoGPS = 8.0; // Possíveis erros de GPS, duas pessoas fazem a reclamação do mesmo problema, mas ficaram fora do mesmo raio. Pode ser alterado
    mt19937 rng(42);
    normal_distribution<double> ruido(0, ruidoGPS);
    vector<Chamado> abertos; // Quantos chamados realmente foram criados, ou seja, não sofreram deduplicação
    vector<int> defeitoDoRelato;
    int nDefeitos = 0;

    auto registrar = [&](Chamado defeito) {
        for (int i = 0; i < relatosPorDefeito; i++) {
            Chamado r = defeito;
            r.id = defeitoDoRelato.size();
            r.local = deslocar(defeito.local, ruido(rng), ruido(rng));
            defeitoDoRelato.push_back(nDefeitos);
            if (!estrutura->registrarChamado(r, 30.0)) abertos.push_back(r);
        }
        nDefeitos++;
    };
    
    auto inicio = chrono::steady_clock::now();

    registrar(ilum);
    registrar(agua);
    registrar(bur);

    for(int i = 0; i < tests; i++){
        point.lat += 0.00037;
        point.lon += 0.00053;
        
        if(point.lat > -5.70) point.lat -= 0.20;
        if(point.lon > -35.15) point.lon -= 0.14;
        
        ilum = {id++, Tipo::Iluminacao, point, 1};
        agua = {id++, Tipo::Agua, point, 1};
	    bur = {id++, Tipo::Buraco, point, 1};
        
        registrar(ilum);
        registrar(agua);
        registrar(bur);
    }
    
    auto fim = chrono::steady_clock::now();
    
    auto total = fim - inicio;
    
    cout << "Completo. Tempo: " << chrono::duration_cast<chrono::milliseconds>(total).count() << "ms.\n";

    vector<int> chamadosPorDefeito(nDefeitos, 0);
    for (const Chamado &c : abertos) chamadosPorDefeito[defeitoDoRelato[c.id]]++;
    int semChamado = 0, excedentes = 0;
    for (int n : chamadosPorDefeito) {
        if (n == 0) semChamado++;
        else excedentes += n - 1;
    }

    vector<bool> atendido(defeitoDoRelato.size(), false);
    int visitas = 0;
    for (const Chamado &c : abertos) {
        if (atendido[c.id]) continue;
        visitas++;
        for (Chamado *v : estrutura->consultarRaio(c.local, 500.0)) atendido[v->id] = true;
    }

    cout << "Relatos: " << defeitoDoRelato.size() << " | Chamados: " << abertos.size()
         << " | Defeitos reais: " << nDefeitos << " | Defeitos sem chamado: " << semChamado
         << " | Chamados a mais: " << excedentes << " | Visitas (500m): " << visitas << "\n";
}

template <typename Estrutura>
void macroBuscar(Estrutura *estrutura, GeoHash::Point point, int tests){
    cout << "Entrou (busca).\n";
    size_t totalAchados = 0;
    
    auto inicio = chrono::steady_clock::now();
	
    totalAchados += estrutura->consultarRaio(point, 500.0).size();
    
    for(int i = 0; i < tests; i++){
        point.lat += 0.0019;
        point.lon += 0.0023;
        
        if(point.lat > -5.70) point.lat -= 0.20;
        if(point.lon > -35.15) point.lon -= 0.14;
        
        totalAchados += estrutura->consultarRaio(point, 500.0).size();
    }
    auto fim = chrono::steady_clock::now();
    
    auto total = fim - inicio;
    
    cout << "Completo. Tempo: " << chrono::duration_cast<chrono::milliseconds>(total).count() << "ms.\n";
}


int main(){

	Trie trie(8);
	QuadTree qt({{-6.0, -35.4}, {-5.6, -35.0}});
    Baseline bl;
	
	GeoHash::Point testPoint = { -5.833597, -35.203036 };
	
    cout << "Trie" << endl;
	macroAdicionar(&trie, testPoint, 5000);
    macroBuscar(&trie, testPoint, 5000);
    
    cout << "QuadTree" << endl;
	macroAdicionar(&qt, testPoint, 5000);
    macroBuscar(&qt, testPoint, 5000);

    cout << "Baseline" << endl;
    macroAdicionar(&bl, testPoint, 5000);
    macroBuscar(&bl, testPoint, 5000);
}
