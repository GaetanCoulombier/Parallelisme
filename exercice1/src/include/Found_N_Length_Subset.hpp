#ifndef FOUND_N_LENGTH_SUBSET_HPP
#define FOUND_N_LENGTH_SUBSET_HPP

#include <cstddef>

template <typename T, size_t n, T... list>
class Found_N_Length_Subset;

// Cas général : liste non vide
template <typename T, size_t n, T val, T next, T... rest>
class Found_N_Length_Subset<T, n, val, next, rest...> {
public:
    enum : bool {
        Yes = (n == 0) 
            ? true // Si on cherche une sous-liste de taille 0, elle existe toujours
            : ((val == next) && Found_N_Length_Subset<T, n - 1, next, rest...>::Yes) 
              || Found_N_Length_Subset<T, n, next, rest...>::Yes
    };
};

// Cas particulier : un seul élément
template <typename T, size_t n, T val>
class Found_N_Length_Subset<T, n, val> {
public:
    enum : bool { Yes = (n == 0) ? true : (n == 1) };
};

// Cas particulier : liste vide
template <typename T, size_t n>
class Found_N_Length_Subset<T, n> {
public:
    enum : bool { Yes = (n == 0) };
};

#endif // FOUND_N_LENGTH_SUBSET_HPP
