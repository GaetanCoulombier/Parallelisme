#ifndef LONGUEST_SUBSET_HPP
#define LONGUEST_SUBSET_HPP

#include <cstddef>
#include "Count.hpp"

template <typename T, T... list>
class Longuest_Subset;

// Cas général : liste non vide
template <typename T, T val, T... rest>
class Longuest_Subset<T, val, rest...> {
private:
    // Calcule la longueur de la sous-liste courante
    static const int currentCount = Count<T, val, rest...>::value + 1;

    // Calcule la longueur de la sous-liste la plus longue
    static const int maxCount = Longuest_Subset<T, rest...>::value;

public:
    // La longueur de la sous-liste la plus longue est le maximum entre la longueur de la sous-liste courante et la longueur de la sous-liste la plus longue
    static const int value = (currentCount > maxCount) ? currentCount : maxCount;
};

// Cas particulier : liste vide
template <typename T>
class Longuest_Subset<T> {
public:
    static const int value = 0;
};

#endif
