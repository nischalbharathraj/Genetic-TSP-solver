#include <bits/stdc++.h>
using namespace std;

double calculateDistance(const vector<int>& route, const vector<pair<int, int>>& cities) {
    double totalDistance = 0.0;
    int numCities = cities.size();
    for (int i = 0; i < numCities; ++i) {
        int x1 = cities[route[i]].first, y1 = cities[route[i]].second;
        int x2 = cities[route[(i + 1) % numCities]].first, y2 = cities[route[(i + 1) % numCities]].second;
        totalDistance += sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }
    return totalDistance;
}

vector<vector<int>> createInitialPopulation(int populationSize, int numCities) {
    vector<vector<int>> population(populationSize, vector<int>(numCities));
    iota(population[0].begin(), population[0].end(), 0);
    for (int i = 0; i < populationSize; ++i) {
        random_shuffle(population[i].begin(), population[i].end());
    }
    return population;
}

vector<int> selectParent(const vector<vector<int>>& population, const vector<pair<int, int>>& cities) {
    return calculateDistance(population[0], cities) < calculateDistance(population[1], cities) ? population[0] : population[1];
}

vector<int> crossover(const vector<int>& parent1, const vector<int>& parent2) {
    int size = parent1.size();
    vector<int> child(size, -1);
    int start = rand() % size;
    int end = start + size / 2;
    copy(parent1.begin() + start, parent1.begin() + end, child.begin() + start);
    int childIndex = end % size;
    for (int i = 0; i < size; ++i) {
        int city = parent2[i];
        if (find(child.begin(), child.end(), city) == child.end()) {
            child[childIndex] = city;
            childIndex = (childIndex + 1) % size;
        }
    }
    return child;
}

void mutate(vector<int>& route) {
    int idx1 = rand() % route.size();
    int idx2 = rand() % route.size();
    swap(route[idx1], route[idx2]);
}

vector<int> geneticAlgorithm(const vector<pair<int, int>>& cities, int populationSize, int generations) {
    int numCities = cities.size();
    vector<vector<int>> population = createInitialPopulation(populationSize, numCities);
    for (int gen = 0; gen < generations; ++gen) {
        vector<vector<int>> newPopulation(populationSize);
        for (int i = 0; i < populationSize; ++i) {
            vector<int> parent1 = selectParent(population, cities);
            vector<int> parent2 = selectParent(population, cities);
            vector<int> child = crossover(parent1, parent2);
            mutate(child);
            newPopulation[i] = child;
        }
        population = newPopulation;
    }
    vector<int> bestRoute = population[0];
    double bestDistance = calculateDistance(bestRoute, cities);
    for (const auto& route : population) {
        double distance = calculateDistance(route, cities);
        if (distance < bestDistance) {
            bestRoute = route;
            bestDistance = distance;
        }
    }
    return bestRoute;
}

int main() {
    srand(time(0));
    vector<pair<int, int>> cities = {{0, 0}, {1, 5}, {5, 1}, {10, 10}, {10, 0}};
    int populationSize = 10;
    int generations = 100;
    vector<int> bestRoute = geneticAlgorithm(cities, populationSize, generations);
    double bestDistance = calculateDistance(bestRoute, cities);
    cout << "Best route: ";
    for (int city : bestRoute) {
        cout << city << " ";
    }
    cout << endl;
    cout << "Total distance: " << bestDistance << endl;
    return 0;
}
