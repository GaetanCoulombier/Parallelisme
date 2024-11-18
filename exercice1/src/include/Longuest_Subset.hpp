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
    // Longueur de la sous-liste consécutive de `val` à partir du premier élément
    static constexpr int currentCount = Count<T, val, rest...>::value + 1;

    // Calculer la longueur maximale pour l'élément courant et les éléments suivants
    static constexpr int maxCount = Longuest_Subset<T, rest...>::value;

public:
    // La longueur de la plus longue sous-liste consécutive
    enum : int { value = (currentCount > maxCount) ? currentCount : maxCount};
};

// Cas particulier : liste vide
template <typename T>
class Longuest_Subset<T> {
public:
    // Si la liste est vide, la longueur maximale est 0
    enum : int { value = 0 };
};

#endif // LONGUEST_SUBSET_HPP
