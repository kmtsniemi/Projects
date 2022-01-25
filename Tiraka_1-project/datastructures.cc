// Datastructures.cc
//
// Student name: Krista Mätäsniemi
// Student email: krista.matasniemi@tuni.fi

#include "datastructures.hh"

#include <random>

#include <cmath>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}

// Modify the code below to implement the functionality of the class.
// Also remove comments from the parameter names when you implement
// an operation (Commenting out parameter name prevents compiler from
// warning about unused parameters on operations you haven't yet implemented.)

Datastructures::Datastructures()
{
    // Write any initialization you need here
}

Datastructures::~Datastructures()
{
    // Write any cleanup you need here
}

unsigned int Datastructures::town_count()
{
    return Towns.size(); // O(1)
}

void Datastructures::clear_all()
{
    Towns.clear(); // O(n), n = kaikki olemassa olevat kaupungit
    AllTowns.clear(); // O(n), n = kaikki olemassa olevat kaupungit
    is_sorted_alphabetically = false;
    is_sorted_distance_increasing = false;

    // poistetaan myös tiet
    AllRoads.clear(); // O(k), k = teiden lkm
}

bool Datastructures::add_town(TownID newId, const Name &newName, Coord newCoord, int newTax)
{
    if ( Towns.find(newId) == Towns.end()) { // Tarkastellaan että uutta kaupunkia ei ole jo olemassa
        // find on O(log(n)), kuitenkin keskimäärin O(1), koska kyseessä unordered_map
        // end on O(1)
        Town newTown = {newId, newName, newTax, newCoord};
        AllTowns.push_back(newId); // O(1)
        Towns.insert({newId, newTown}); // O(n), n = kaikki olemassa olevat kaupungit
        // insert on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
        is_sorted_alphabetically = false;
        is_sorted_distance_increasing = false;
        return true;
    }
    return false;
}

Name Datastructures::get_town_name(TownID id)
{
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ) { // Tarkastellaan onko kaupunki olemassa
        // end on O(1)
        return town_it->second.name;
    }
    return NO_NAME;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ) { // Tarkastellaan onko kaupunki olemassa
        // end on O(1)
        return town_it->second.coordinates;
    }
    return NO_COORD;
}

int Datastructures::get_town_tax(TownID id)
{
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ) { // Tarkastellaan onko kaupunki olemssa
        // end on O(1)
        return town_it->second.tax;
    }
    return NO_VALUE;
}

std::vector<TownID> Datastructures::all_towns()
{
    return AllTowns;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> findedTowns;

    // varataan muistista tilaa valmiiksi tehokkuuden lisäämiseksi
    findedTowns.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit

    for ( auto &item : Towns ){ // O(n), n = kaikki olemassa olevat kaupungit
        if ( item.second.name == name ){
            findedTowns.push_back(item.first); // O(1)
        }
    }
    return findedTowns;
}

bool Datastructures::change_town_name(TownID id, const Name &newName)
{
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ) { // Tarkastellaan onko kaupunki olemssa
        // end on O(1)
        town_it->second.name = newName;
        is_sorted_alphabetically = false;
        return true;
    }
    return false;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    if ( is_sorted_alphabetically ){ // jos järjestys on ajantasainen
        return sorted_alphabetically;
    }

    std::vector<std::pair<TownID, Name>> names;
    sorted_alphabetically.clear(); // O(n)

    // varataan muistista tilaa valmiiksi tehokkuuden lisäämiseksi
    names.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit
    sorted_alphabetically.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit

    // tallenenetaan kaikkien olemassa olevien kaupunkien nimet ja id:t arvopareina names-vektoriin
    for ( auto &item : Towns ){ // O(n), n = kaikki olemassa olevat kaupungit
        names.push_back({item.second.id, item.second.name}); // O(1)
    }

    // järejestetään names-vektori kaupungin nimen mukaan aakkosjärjestykseen
    std::sort(names.begin(), names.end(),
              [](const std::pair<TownID, Name> &x, const std::pair<TownID, Name> &y){
        return (x.second < y.second);
    }); // O(n*log(n)), n = kaikki olemassa olevat kaupungit

    // muodostetaan palautettava vektori järjestetyn vektorin mukaan
    for ( auto &item : names ){ // O(n), n = kaikki olemassa olevat kaupungit
        sorted_alphabetically.push_back(item.first); // O(1)
    }
    is_sorted_alphabetically = true;
    return sorted_alphabetically;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    if( is_sorted_distance_increasing ){ // jos järjestys on ajantasainen
        return sorted_distance_increasing;
    }

    std::vector<std::pair<TownID, int>> distances;
    sorted_distance_increasing.clear(); // O(n)

    // varataan muistista tilaa valmiiksi tehokkuuden lisäämiseksi
    distances.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit
    sorted_distance_increasing.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit

    // tallenenetaan kaikkien olemassa olevien kaupunkien etäisyydet origosta ja id:t arvopareina distances-vektoriin
    for ( auto &item : Towns ){ // O(n), n = kaikki olemassa olevat kaupungit
        int D = distance(item.second.coordinates);
        distances.push_back({item.second.id, D}); // O(1)
    }

    // järejestetään distances-vektori kaupungin etäidyyden mukaan
    std::sort(distances.begin(), distances.end(),
              [](const std::pair<TownID, int> &x, const std::pair<TownID, int> &y){
        return (x.second < y.second);
    }); // O(n*log(n)), n = kaikki olemassa olevat kaupungit

    // muodostetaan palautettava vektori järjestetyn vektorin mukaan
    for ( auto &item : distances ){ // O(n), n = kaikki olemassa olevat kaupungit
        sorted_distance_increasing.push_back(item.first); // O(1)
    }
    is_sorted_distance_increasing = true;
    return sorted_distance_increasing;
}

TownID Datastructures::min_distance()
{
    std::vector<TownID> townsSortedByDistance = towns_distance_increasing();

    if ( townsSortedByDistance.size() != 0 ){
        return townsSortedByDistance.front(); // O(1)
    }
    return NO_TOWNID;
}

TownID Datastructures::max_distance()
{
    std::vector<TownID> townsSortedByDistance = towns_distance_increasing();

    if ( townsSortedByDistance.size() != 0 ){
        return townsSortedByDistance.back(); // O(1)
    }
    return NO_TOWNID;
}

bool Datastructures::add_vassalship(TownID vassalId, TownID masterId)
{
    auto master_it = Towns.find(masterId); // O(log(n)), n = kaikki olemassa olevat kaupungit
    auto vassal_it = Towns.find(vassalId);// O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( master_it->second.master == vassalId ){ // tarkastellaan onko vassalisuhde jo olemassa toiseen suuntaaan
        // ei haluta silmukoita
        return false;
    }

    if ( master_it != Towns.end() && vassal_it != Towns.end() ){ // Tarkastellaan että molemmat kaupungit ovat olemassa
        if ( vassal_it->second.master == NO_TOWNID ){
            master_it->second.vassals.push_back(vassalId); // O(n), n = vassalien määrä
            // push_back on O(1), jos peräkkäiset muistipaikat riittävät
            // Tässä tapauksessa tarkastellaan tehokkuutta olemassa olevien kaupunkien suhteen
            vassal_it->second.master = masterId;
            return true;
        }
    }
    return false;
}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    std::vector<TownID> no_vas = {NO_TOWNID};
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ){ // Tarkastellaan onko kaupunki olemassa
        return town_it->second.vassals;
    }
    return no_vas;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    std::vector<TownID> taxers;

    // varataan muistista tarvittava tila tehokkuuden lisäämiseksi
    taxers.reserve(town_count()); // O(n)

    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ){ // Tarkastellaan onko kaupunki olemassa
        taxers.push_back(id); // O(1)

        while( !(town_it->second.master == NO_TOWNID) ){ // O(n), n = kaikki olemassa olevat kaupungit
            taxers.push_back(town_it->second.master); // O(1)
            town_it= Towns.find(town_it->second.master); // O(log(n)), n = kaikki olemassa olevat kaupungit
            // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
        }
    }
    else {
        taxers.push_back(NO_TOWNID); // O(1)
    }
    return taxers;
}

bool Datastructures::remove_town(TownID id)
{
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ){ // Tarkastetaan onko kaupunki olemassa
        Town town = town_it->second;
        if ( !(town_it->second.master == NO_TOWNID) ){ // tarkastetaan, että kaupungilla on master
            // lisätään uudet vassali uudet suhteet
            for ( auto &item : town.vassals ){ // O(k), k = vasallien määrä
                // Tarkastellaan tehokkuutta vain olemassa olevien kaupunkien suhteen
                Towns.find(item)->second.master = NO_TOWNID; // O(log(n)), n = kaikki olemassa olevat kaupungit
                // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
                add_vassalship(item, town.master);
            }
            auto master = Towns.find(town.master); // O(log(n)), n = kaikki olemassa olevat kaupungit
            // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
            auto it = std::find(master->second.vassals.begin(), master->second.vassals.end(), id);
            // find on O(k), k = vassalien määrä
            // poistetaan kaupunki masterin vassaleista
            master->second.vassals.erase(it); // O(k), k = vassalien määrä
            // Tarkastellaan tehokkuutta vain olemassa olevien kaupunkien suhteen
        }
        else {
            for ( auto &item : town.vassals ){ // O(k), k = vasallien määrä
                // Tarkastellaan tehokkuutta vain olemassa olevien kaupunkien suhteen
                Towns.find(item)->second.master = NO_TOWNID; // O(log(n)), n = kaikki olemassa olevat kaupungit
                // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
            }
        }
        // poistetaan tiet
        for ( auto &to_id : town_it->second.roads_to ){ // O(t), t = teiden lkm
            remove_road(id, to_id); // O(t + log n)
        }

        // poistetaan kaupunki
        auto it = std::find(AllTowns.begin(), AllTowns.end(), id); // O(n), n = kaikki olemassa olevat kaupungit
        AllTowns.erase(it); // O(n), n = kaikki olemassa olevat kaupungit

        auto i = Towns.find(id); // O(n), n = kaikki olemassa olevat kaupungit
        Towns.erase(i); // O(n), n = kaikki olemassa olevat kaupungit

        is_sorted_alphabetically = false;
        is_sorted_distance_increasing = false;

        return true;
    }
    return false;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    // luodaan apuvektorit
    std::vector<std::pair<TownID, int>> distances;
    std::vector<TownID> sorted;

    // varataan muistipaikat vektoreille tehokkuuden lisäämiseksi
    distances.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit
    sorted.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit

    // lisätään distances-vektoriin arvoparit jokaisesta kaupungista ja lasketusta etäisyydestä
    for ( auto &item : Towns ){ // O(n), n = kaikki olemassa olevat kaupungit
        int D = distance(item.second.coordinates, coord);
        distances.push_back({item.second.id, D}); // O(1)
    }

    // järjestetään arvoparit etäisyyden mukaan
    std::sort(distances.begin(), distances.end(),
              [](const std::pair<TownID, int> &x, const std::pair<TownID, int> &y){
        return (x.second < y.second);
    }); // O(n*log(n)), n = kaikki olemassa olevat kaupungit

    // muodostetaan palautettava vektori järjestetyn vektorin mukaan
    for ( auto &item : distances ){ // O(n), n = kaikki olemassa olevat kaupungit
        sorted.push_back(item.first); // O(1)
    }
    return sorted;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    std::vector<TownID> longest_path;
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ){ // Tarkastellaan onko kaupunki olemassa
        // varataan tila muistista tehokkuuden lisäämiseksi
        longest.reserve(town_count()); // O(n), n = kaikki olemassa olevat kaupungit
        longest_path = vassal_path(id);
        longest.clear(); // O(n), n = kaikki olemassa olevat kaupungit
    }
    else {
        longest_path.push_back(NO_TOWNID); // O(1)
    }
    return longest_path;

}

int Datastructures::total_net_tax(TownID id)
{
    auto town_it = Towns.find(id); // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ){ // tarkastellaan onko kaupunki olemassa
        Town town = town_it->second;
        if ( !(town_it->second.master == NO_TOWNID) ){ // tarkastetaan, että kaupungilla on master
            if ( town_it->second.vassals.size() != 0 ){
                int t_tax = total_tax(id);
                return t_tax - floor(t_tax * 0.1);
            } else {
                return town.tax - floor(town.tax * 0.1); // O(1), koska kaupunkien määrä ei vaikuta
            }
        } else {
            if ( town_it->second.vassals.size() != 0 ){
                return total_tax(id);
            } else {
                return town.tax;
            }
        }
    }
    return NO_VALUE;
}

//
// Phase 2 operations
//


void Datastructures::clear_roads()
{
    for ( auto &town : Towns ){ // O(n), n = kaupunkien lkm
        town.second.roads_to.clear(); // O(n)
    }

    AllRoads.clear(); // O(k), k = teiden lkm
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    return AllRoads;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    auto town1_it = Towns.find(town1); // O(log(n)), n = kaupunkien lkm
    auto town2_it = Towns.find(town2); // O(log(n)), n = kaupunkien lkm
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town1_it != Towns.end() && town2_it != Towns.end() ){ // tarkastellaan, että kaupungit ovat olemassa
        std::pair<TownID, TownID> road;
        std::vector<TownID> roads_to_1 = town1_it->second.roads_to;

        // tarkastellaan onko tie jo olemassa
        if ( std::find(roads_to_1.begin(), roads_to_1.end(), town2) != roads_to_1.end() ){ // O(k), k = teiden lkm
            return false;
        }

        // muodostetaan tiepari
        if ( town1 < town2 ){
            road.first = town1;
            road.second = town2;
        } else {
            road.first = town2;
            road.second = town1;
        }

        // lisätään tie
        AllRoads.push_back(road); // O(1)

        town1_it->second.roads_to.push_back(town2); // O(1)
        town2_it->second.roads_to.push_back(town1); // O(1)

        return true;
    }

    return false;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    auto town_it = Towns.find(id); // O(log(n)), n = kaupunkien lkm
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town_it != Towns.end() ){ // tarkastellaan, että kaupunki on olemassa
        return town_it->second.roads_to;
    }

    return {NO_TOWNID};
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    return least_towns_route(fromid, toid);
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    auto town1_it = Towns.find(town1); // O(log(n)), n = kaupunkien lkm
    auto town2_it = Towns.find(town2); // O(log(n)), n = kaupunkien lkm
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( town1_it != Towns.end() && town2_it != Towns.end() ){ // tarkastellaan, että kaupungit ovat olemassa
        std::pair<TownID, TownID> road;

        // muodostetaan tiepari
        if ( town1 < town2 ){
            road.first = town1;
            road.second = town2;
        }
        else {
            road.first = town2;
            road.second = town1;
        }

        // tarkastellaan onko tie olemassa
        auto it = std::find(AllRoads.begin(), AllRoads.end(), road); // O(k), k = teiden lkm
        if ( it == AllRoads.end() ){
            return false;
        }
        else {
            auto it_1 = std::find(town1_it->second.roads_to.begin(), town1_it->second.roads_to.end(), town2); // O(k), k = teiden lkm
            auto it_2 = std::find(town2_it->second.roads_to.begin(), town2_it->second.roads_to.end(), town1); // O(k), k = teiden lkm
            town2_it->second.roads_to.erase(it_2); // O(k), k = teiden lkm
            town1_it->second.roads_to.erase(it_1); // O(k), k = teiden lkm
            AllRoads.erase(it); // O(k), k = teiden lkm
        }

        return true;
    }

    return false;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    auto from_it = Towns.find(fromid); // O(log(n)), n = kaupunkien lkm
    auto to_it = Towns.find(toid); // O(log(n)), n = kaupunkien lkm
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( from_it != Towns.end() && to_it != Towns.end() ){ // tarkastellaan onko kaupungit olemassa
        std::vector<TownID> route;
        std::queue<TownID> q;
        bool is_found = false;

        route.reserve(town_count()); // O(n)

        // alustetaan kaupunkien tiedot
        for ( auto &town : Towns ){
            town.second.colour = WHITE;
            town.second.d = INFINITY;
            town.second.pi = NO_TOWNID;
        }

        // alustetaan lähtö
        from_it->second.colour = GRAY;
        from_it->second.d = 0;

        q.push(fromid); // O(1)

        while ( !q.empty() ){ // O(n)
            TownID id = q.front(); // O(1)
            q.pop(); // O(1)
            auto it = Towns.find(id); // O(log(n)), n = kaupunkien lkm
            // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

            if ( id == toid ){
                is_found = true;
                break;
            }

            for ( auto &to_id : it->second.roads_to ){ // O(k)
                auto to = Towns.find(to_id); // O(log(n)), n = kaupunkien lkm
                // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

                if ( to->second.colour == WHITE ){
                    to->second.colour = GRAY;
                    to->second.d = it->second.d + 1;
                    to->second.pi = id;
                    q.push(to_id); // O(1)
                }
            }
            it->second.colour = BLACK;
        }
        if ( is_found == true ){
            TownID id = toid;
            while ( id != NO_TOWNID ) {
                auto it = Towns.find(id); // O(log(n)), n = kaupunkien lkm
                // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
                route.push_back(id); // O(1)
                id = it->second.pi;
            }
            std::reverse(route.begin(), route.end()); // O(n)
            return route;
        } else {
            return {};
        }
    }
    return {NO_TOWNID};
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    auto start_it = Towns.find(startid); // O(log(n)), n = kaupunkien lkm
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( start_it != Towns.end() ){ // tarkastellaan onko kaupunki olemassa
        std::vector<TownID> route;
        std::stack<TownID> s;

        route.reserve(town_count()); // O(n)

        // alustetaan kaupunkien tiedot
        for ( auto &town : Towns ){
            town.second.colour = WHITE;
        }

        s.push(startid); // O(1)

        while ( !s.empty() ){ // O(n)
            TownID id = s.top(); // O(1)
            s.pop(); // O(1)
            auto it = Towns.find(id);// O(log(n)), n = kaupunkien lkm
            // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

            if ( it->second.colour == WHITE ){
                it->second.colour = GRAY;
                s.push(id); // O(1)

                for ( auto &to_id : it->second.roads_to ){ // O(k)
                    auto to = Towns.find(to_id);// O(log(n)), n = kaupunkien lkm
                    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

                    if ( to->second.colour == WHITE ){
                        s.push(to_id); // O(1)
                    }
                    else if ( to->second.colour == GRAY && to_id != route.back() ){
                        // käseittele silmukka
                        route.push_back(id); // O(1)
                        route.push_back(to_id); // O(1)
                        return route;

                    }
                }
                route.push_back(id); // O(1)
            }
            else {
                it->second.colour = BLACK;
                route.pop_back(); // O(1)
            }
        }
        return {};
    }
    return {NO_TOWNID};
}

std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    auto from_it = Towns.find(fromid); // O(log(n)), n = kaupunkien lkm
    auto to_it = Towns.find(toid); // O(log(n)), n = kaupunkien lkm
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

    if ( from_it != Towns.end() && to_it != Towns.end() ){ // tarkastellaan, että kaupungit ovat olemassa
        std::priority_queue<std::pair<int,TownID>> pq;
        std::vector<TownID> route;
        bool is_found = false;

        route.reserve(town_count()); // O(n)

        // alustetaan kaupunkien tiedot
        for ( auto &town : Towns ){
            town.second.colour = WHITE;
            town.second.d = std::numeric_limits<int>::min(); // ääretön
            town.second.de = std::numeric_limits<int>::min(); // ääretön
            town.second.pi = NO_TOWNID;
        }

        // alustetaan lähtö
        from_it->second.colour = GRAY;
        from_it->second.d = 0;

        pq.push({0,fromid}); // O(log(n+k))

        while ( !pq.empty() ){ // O(n)
            TownID sortest = pq.top().second; // O(log(n+k))
            pq.pop(); // O(log(n+k))

            auto it = Towns.find(sortest);// O(log(n)), n = kaupunkien lkm
            // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map

            if ( it->second.colour != BLACK ){

                if ( sortest == toid ){
                    is_found = true;
                    break;
                    //return route;
                }

                for ( auto &id : it->second.roads_to ){ // O(k)
                    auto id_it = Towns.find(id);// O(log(n)), n = kaupunkien lkm
                    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
                    bool updated = false;

                    // Relax A*
                    int d = distance(it->second.coordinates, id_it->second.coordinates);
                    if ( id_it->second.d < (it->second.d - d) ){
                        id_it->second.d = it->second.d - d;
                        id_it->second.de = id_it->second.d - distance(id_it->second.coordinates, to_it->second.coordinates); // O(1)
                        id_it->second.pi = sortest;
                        updated = true;
                    }

                    if ( id_it->second.colour == WHITE ){
                        id_it->second.colour = GRAY;
                        pq.push({(id_it->second.de), id}); // O(log(n))
                    }
                    else if ( updated ){
                        pq.push({(id_it->second.de), id}); // O(log(n+k)) // lisätään uutena eikä päivitetä vanhaa
                    }
                }
                it->second.colour = BLACK;
            }
        }
        // muodostetaan palautettava vektori
        if ( is_found == true ){
            TownID id = toid;
            while ( id != NO_TOWNID ){
                route.push_back(id);
                auto id_it = Towns.find(id);// O(log(n)), n = kaupunkien lkm
                // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
                id = id_it->second.pi;
            }
            std::reverse(route.begin(), route.end());
            return route;
        } else {
            return {};
        }
    }

    return {NO_TOWNID};
}

Distance Datastructures::trim_road_network()
{
    std::unordered_map<TownID, TownID> disjoint_set;
    std::priority_queue<std::pair<int,std::pair<TownID, TownID>>> pq;
    Distance road_network_d = 0;

    for ( auto &id : AllTowns ){ // O(n)
        disjoint_set.insert({id, NO_TOWNID});
    }

    for ( auto &pair : AllRoads ){ // O(k)
        Coord c1 = Towns.find(pair.first)->second.coordinates;
        Coord c2 = Towns.find(pair.second)->second.coordinates;
        int d = distance(c1, c2);
        pq.push({-d, pair}); // O(log(n+k))
    }

    // käydään läpi kaikki tiet lyhimmästä pisimpään
    while ( !pq.empty() ){ // O(k)
        std::pair<TownID, TownID> road = pq.top().second; // O(log(n+k))
        Distance d = pq.top().first; // O(log(n+k))
        pq.pop(); // O(log(n+k))

        TownID id1 = disjoint_set.at(road.first); // O(n)
        TownID id2 = disjoint_set.at(road.second); // O(n)
        // keskimäärin kuitenkin vakio O(1)

        TownID to_id1 = road.first;
        TownID to_id2 = road.second;

        while( id1 != NO_TOWNID && id1 != to_id1 ){ // O(n)
            to_id1 = id1;
            id1 = disjoint_set.at(id1); // O(n) mutta keskimäärin vakio
        }
        while ( id2 != NO_TOWNID && id2 != to_id2 ){ // O(n)
            to_id2 = id2;
            id2 = disjoint_set.at(id2); // O(n) mutta keskimäärin vakio
        }

        if ( id1 == NO_TOWNID && id2 == NO_TOWNID ){
            disjoint_set.at(road.first) = road.second;
            disjoint_set.at(road.second) = road.second;
        }
        else if ( id1 == to_id1 && id2 == NO_TOWNID ){
            disjoint_set.at(road.second) = id1;
        }
        else if ( id2 == to_id2 && id1 == NO_TOWNID ){
            disjoint_set.at(road.first) = id2;
        }
        else if ( id1 != id2 ){
            disjoint_set.at(id1) = id2;
        }
        else {
            // poistetaan tie
            remove_road(road.first, road.second); // O(k+log n)

            d = 0;
        }
        road_network_d -= d;
    }
    return road_network_d; // palautetaan tieverkon yhteispituus
}


// Private

int Datastructures::distance(Coord townCoord, Coord compCoord)
{
    float xCoordDif = compCoord.x - townCoord.x;
    float yCoordDif = compCoord.y - townCoord.y;
    int distance = floor(sqrt(pow(xCoordDif,2) + pow(yCoordDif,2))); // O(1), koska kaupunkien määrä ei vaikuta
    return distance;
}

int Datastructures::total_tax(TownID id)
{
    Town town = Towns.find(id)->second; // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
    int tax = 0;
    for ( auto &item : town.vassals ){
        // tehokkuus ei sidoksissa kaupunkien määrään, joten tässä tapauksessa...
        tax += floor((total_tax(item)) * 0.1); // ...floor on O(1)
    }
    return town.tax + tax;
}

std::vector<TownID> Datastructures::vassal_path(TownID id)
{
    Town town = Towns.find(id)->second; // O(log(n)), n = kaikki olemassa olevat kaupungit
    // find on kuitenkin keskimäärin O(1), koska kyseessä unordered_map
    current.push_back(id); // O(1) jos muistissa tilaa
    for ( auto &item : town.vassals ){
        vassal_path(item);
    }
    if (current.size() > longest.size()){ // O(1)
        longest = current; // O(1)
    }
    current.pop_back(); // O(1)
    return longest;

}

