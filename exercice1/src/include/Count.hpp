#ifndef COUNT_HPP
#define COUNT_HPP

template <typename T, T val, T... list>
class Count;

// Cas général : liste non vide
template <typename T, T val, T first, T... rest>
class Count<T, val, first, rest...> {
public:
    // Si le premier élément est égal à `val`, continue la récursion
    enum : int { value = (first == val) ? 1 + Count<T, val, rest...>::value : 0};
};

// Cas particulier : liste vide
template <typename T, T val>
class Count<T, val> {
public:
    // Liste vide, retourne 0
    enum : int { value = 0 };
};

#endif