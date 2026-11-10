#include <iostream>
#include <string>
#include <vector>
using namespace std;

class StarSystem;

class TravelRoute {
public:
    float travel_cost;
    StarSystem *destination;

    TravelRoute(float cost, StarSystem *dest) : travel_cost(cost), destination(dest) {}
};

class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    float x, y, z;
    bool is_habitable;
    vector<TravelRoute> routes;

    StarSystem(int id, const string &n, int planets, float px, float py, float pz, bool habitable)
        : system_id(id), name(n), num_planets(planets), x(px), y(py), z(pz), is_habitable(habitable) {}
};

class Galaxy {
private:
    int M;
    StarSystem *root;

    int subtree_size(StarSystem *x, bool &balanced) const;
    void destroy(StarSystem *x);

public:
    Galaxy(int m) : M(m), root(NULL) {}
    Galaxy(int m, StarSystem *r) : M(m), root(r) {}
    ~Galaxy() { destroy(root); }

    bool add_route(StarSystem *from, StarSystem *to, float cost);
    bool is_balanced(StarSystem *x) const;
    int count_leaves(StarSystem *x) const;
};

bool Galaxy::add_route(StarSystem *from, StarSystem *to, float cost) {
    if (from == NULL || to == NULL) return false;
    if ((int)from->routes.size() >= M) {
        return false;
    }
    from->routes.push_back(TravelRoute(cost, to));
    return true;
}

int Galaxy::subtree_size(StarSystem *x, bool &balanced) const {
    if (x == NULL) return 0;

    int total = 1;
    int smallest = 0, largest = 0;
    for (size_t i = 0; i < x->routes.size(); i++) {
        int s = subtree_size(x->routes[i].destination, balanced);
        total += s;
        if (i == 0 || s < smallest) smallest = s;
        if (i == 0 || s > largest) largest = s;
    }
    if (largest - smallest > 1) balanced = false;
    return total;
}

bool Galaxy::is_balanced(StarSystem *x) const {
    bool balanced = true;
    subtree_size(x, balanced);
    return balanced;
}

int Galaxy::count_leaves(StarSystem *x) const {
    if (x == NULL) return 0;
    if (x->routes.empty()) return 1;
    int leaves = 0;
    for (size_t i = 0; i < x->routes.size(); i++)
        leaves += count_leaves(x->routes[i].destination);
    return leaves;
}

void Galaxy::destroy(StarSystem *x) {
    if (x == NULL) return;
    for (size_t i = 0; i < x->routes.size(); i++)
        destroy(x->routes[i].destination);
    delete x;
}

int main() {
    // a) Galaxy with M = 3, created with the constructor that takes a root
    StarSystem *sol = new StarSystem(1, "Sol", 8, 0, 0, 0, true);
    Galaxy galaxy(3, sol);

    StarSystem *alpha   = new StarSystem(2, "Alpha Centauri", 3, 4.3f, 0.5f, -1.2f, true);
    StarSystem *sirius  = new StarSystem(3, "Sirius", 2, -8.6f, 1.1f, 0.4f, false);
    StarSystem *vega    = new StarSystem(4, "Vega", 5, 25.0f, -3.2f, 7.8f, false);
    StarSystem *proxima = new StarSystem(5, "Proxima", 1, 4.2f, 0.4f, -1.1f, true);
    StarSystem *barnard = new StarSystem(6, "Barnard", 0, 6.0f, 2.1f, 0.9f, false);
    StarSystem *procyon = new StarSystem(7, "Procyon", 1, -11.4f, 2.0f, 1.5f, false);
    StarSystem *altair  = new StarSystem(8, "Altair", 2, 16.7f, -4.0f, 6.1f, true);

    galaxy.add_route(sol, alpha, 4.5f);
    galaxy.add_route(sol, sirius, 8.7f);
    galaxy.add_route(sol, vega, 25.3f);
    galaxy.add_route(alpha, proxima, 0.2f);
    galaxy.add_route(alpha, barnard, 2.1f);
    galaxy.add_route(sirius, procyon, 5.2f);
    galaxy.add_route(vega, altair, 9.4f);

    // b) Balance check and c) leaf count
    cout << "is_balanced(Sol): " << (galaxy.is_balanced(sol) ? "true" : "false") << endl;
    cout << "count_leaves(Sol): " << galaxy.count_leaves(sol) << endl;
    cout << "count_leaves(Alpha Centauri): " << galaxy.count_leaves(alpha) << endl;

    StarSystem *deneb = new StarSystem(9, "Deneb", 4, -30.0f, 9.0f, 12.0f, false);
    galaxy.add_route(proxima, deneb, 14.8f);
    cout << "\nAfter adding the route Proxima -> Deneb:" << endl;
    cout << "is_balanced(Sol): " << (galaxy.is_balanced(sol) ? "true" : "false") << endl;
    cout << "is_balanced(Alpha Centauri): " << (galaxy.is_balanced(alpha) ? "true" : "false") << endl;
    cout << "count_leaves(Sol): " << galaxy.count_leaves(sol) << endl;

    // a) The overloaded constructor with only M (no root)
    Galaxy emptyGalaxy(4);
    cout << "\nGalaxy with M = 4 and no root: is_balanced = "
         << (emptyGalaxy.is_balanced(NULL) ? "true" : "false")
         << ", count_leaves = " << emptyGalaxy.count_leaves(NULL) << endl;

    return 0;
}