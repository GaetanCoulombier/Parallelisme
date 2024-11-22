#include <iostream>
#include <vector>
#include <chrono>
#include "ParallelRecursiveMerge.hpp"

int main() {
    using namespace std;

    // Déclaration des vecteurs
    vector<int> vec1 = {1, 3, 5, 7, 9};
    vector<int> vec2 = {2, 4, 6, 8, 10};
    vector<int> result(vec1.size() + vec2.size());
    size_t cutoff = 10;

    cout << "== [ Parallel Recursive Merge TBB tasking test = begin ] ==" << endl;

    // Démarrage du chronomètre
    auto start = chrono::high_resolution_clock::now();

    // Appel de la fusion parallèle
    merging::ParallelRecursiveMerge::apply(vec1.begin(), vec1.end(), vec2.begin(), vec2.end(), result.begin(), cutoff);

    // Fin du chronomètre
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end - start;

    // Affichage du temps d'exécution
    cout << '\t' << "Temps d'exécution : " << duration.count() << " ms" << endl;

    // Affichage du résultat
    cout << '\t' << "Résultat de la fusion : ";
    for (const auto& val : result) {
        cout << val << " ";
    }
    cout << endl;

    cout << "== [ Parallel Recursive Merge TBB tasking test = end ] ==" << endl;

    return EXIT_SUCCESS;
}
