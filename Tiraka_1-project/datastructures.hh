// Datastructures.hh
//
// Student name: Krista Mätäsniemi
// Student email: krista.matasniemi@tuni.fi

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <unordered_map>
#include <queue>
#include <stack>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

enum Colour { WHITE, GRAY, BLACK };

struct Town
{
    TownID id = NO_TOWNID;
    Name name = NO_NAME;
    int tax = NO_VALUE;
    Coord coordinates = {};
    std::vector<TownID> vassals = {};
    TownID master = NO_TOWNID;

    std::vector<TownID> roads_to = {};
    Colour colour = WHITE;
    float d = NO_VALUE;
    float de = NO_VALUE;
    TownID pi = NO_TOWNID;
};

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: O(1)
    // Short rationale for estimate: size-funktion ilmoitettu tehokkuus
    unsigned int town_count();

    // Estimate of performance: O(n+k), n = kaupunkien lkm ja k = teiden lkm
    // Short rationale for estimate: clear-funktion ilmoitettu tehokkuus
    void clear_all();

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find ilmoitettu tehokkuus pahimmassa tapauksessa
    // keskimäärin kuitenkin O(1)
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find ilmoitettu tehokkuus
    // keskimäärin kuitenkin O(1)
    Name get_town_name(TownID id);

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find ilmoitettu tehokkuus
    // keskimäärin kuitenkin O(1)
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find ilmoitettu tehokkuus
    // keskimäärin kuitenkin O(1)
    int get_town_tax(TownID id);

    // Estimate of performance: O(1)
    // Short rationale for estimate: palautetaan vain valmis vektori
    std::vector<TownID> all_towns();

    // Estimate of performance: O(n)
    // Short rationale for estimate: for-silmukan tehokkuus ja reserve
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find ilmoitettu tehokkuus
    // keskimäärin kuitenkin O(1)
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: sort-funktion tehokkuus huomioidaan vain koska for-silmukat ovat n
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: sort-funktion tehokkuus huomioidaan vain koska for-silmukat ovat n
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: tehokkuus tulee towns_distance_increasing-funktiosta
    TownID min_distance();

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: tehokkuus tulee towns_distance_increasing-funktiosta
    TownID max_distance();

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find ilmoitettu tehokkuus
    // keskimäärin kuitenkin O(1)
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: O(log n)
    // Short rationale for estimate: find ilmoitettu tehokkuus
    // keskimäärin kuitenkin O(1)
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: while-silmukan tehokkuus n ja sen sisällä find
    // keskimäärin kuitenkin O(n)
    std::vector<TownID> taxer_path(TownID id);

    // Non-compulsory phase 1 operations

    // Estimate of performance: O(k log n + (t + log n)), k = vassalien lkm ja n = kaupunkien lkm ja t = teiden lkm
    // Short rationale for estimate: for-silmukan tehokkuus k ja sen sisällä find-funktio log n
    bool remove_town(TownID id);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: sort-funktion tehokkuus
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: kutsuu rekusrsivista funktiota, jonka sisällä find
    // keskimäärin kuitenkin O(n)
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: kutsuu rekusrsivista funktiota, jonka sisällä find
    // keskimäärin kuitenkin O(n)
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Estimate of performance: O(n²+k), n = kaupunkien lkm ja k = teiden lkm
    // Short rationale for estimate: tyhdenetään tiet ja taupunkien tiedoista tie tiedot
    void clear_roads();

    // Estimate of performance: O(1)
    // Short rationale for estimate: palautetaan vain vektori
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: O(k + log n), k = teiden lkm ja n = kaupunkien lkm
    // Short rationale for estimate: tarkastelussa onko tie jo olemassa ja kaupunki iteraattorien etsintä
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: O(log n), n = kaupunkien lkm (O(1))
    // Short rationale for estimate: find-tehokkuus keskimäärin kuitenkin vakio
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: O((n+k)log n), n = kaupunkien lkm ja k = teiden lkm
    // Short rationale for estimate: least_towns_route tehokkuus
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: O(k+log(n)), n = kaupunkien lkm ja k = teiden lkm
    // Short rationale for estimate: tarkastelusta onko tie olemassa
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: O((n+k)log n), n = kaupunkien lkm ja k = teiden lkm
    // Short rationale for estimate: while-silmukka n kertaa ja for-silmukka k kertaa
    // kaaret käydään useammin kuin kerran läpi mutta vakiokerrointa ei huomioida
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance: O((n+k)log n), n = kaupunkien lkm ja k = teiden lkm
    // Short rationale for estimate: while-silmukka n kertaa ja for-silmukka k kertaa
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: O((n+k)log(n+k)), n = kaupunkien lkm ja k = teiden lkm
    // Short rationale for estimate: while- ja for-silmukat ja niissä prioriteettijonosta poistaminen ja lisääminen
    // prioriteettijonoon lisätään tupla-alkioita siksi log(n+k)
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance: O(n + k * n²)
    // Short rationale for estimate: while-silmukassa lineaarisia operaatioita
    // kaupunkeja aina enemmän kun teitä
    Distance trim_road_network();

private:
    // Add stuff needed for your class implementation here

    std::unordered_map<TownID,Town> Towns;
    std::vector<TownID> AllTowns;
    std::vector<TownID> sorted_distance_increasing;
    bool is_sorted_distance_increasing;
    std::vector<TownID> sorted_alphabetically;
    bool is_sorted_alphabetically;
    std::vector<TownID> current;
    std::vector<TownID> longest;

    std::vector<std::pair<TownID, TownID>> AllRoads;

    // Estimate of performance: O(1)
    // Short rationale for estimate: kaupunkien määrä ei vaikuta
    int distance(Coord townCoord, Coord compCoord = {0,0});

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: rekursiivinen funktio jonka sisällä find
    // keskimäärin kuitenkin O(n)
    int total_tax(TownID id);

    // Estimate of performance: O(n log n)
    // Short rationale for estimate: rekursiivinen funktio jonka sisällä find
    // keskimäärin kuitenkin O(n)
    std::vector<TownID> vassal_path(TownID id);

};

#endif // DATASTRUCTURES_HH
