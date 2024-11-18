#ifndef FOUND_N_LENGTH_SUBSET_HPP
#define FOUND_N_LENGTH_SUBSET_HPP

#include <cstddef>
#include "Count.hpp"

template <typename T, size_t n, T... list>
class Found_N_Length_Subset;

// Cas général : liste non vide
template <typename T, size_t n, T val, T... rest>
class Found_N_Length_Subset<T, n, val, rest...> {
public:
    // On regarde si la sous-liste commence par `val` et contient au moins `n` éléments
    enum : bool {
        Yes = (Count<T, val, rest...>::value + 1 >= n) ? 1 : Found_N_Length_Subset<T, n, rest...>::Yes
    };
};

// Cas particulier : liste vide
template <typename T, size_t n>
class Found_N_Length_Subset<T, n> {
public:
    // Liste vide, il est impossible de trouver une sous-liste d'au moins `n` éléments
    enum : bool { Yes = (n == 0) ? 1 : 0 };
};

#endif // FOUND_N_LENGTH_SUBSET_HPP
