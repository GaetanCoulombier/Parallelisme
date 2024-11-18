#include "include/Count.hpp"
#include "include/Found_N_Length_Subset.hpp"
#include "include/Longuest_Subset.hpp"
#include <iostream>
#include <cstdlib>
#include <cstddef>

int main() { 
    using namespace std;

    // Question 1
    cout << "== [ Count test = begin ] ==" << endl;
    cout << '\t' << "Count<int, 5 >::value = " << Count<int,5>::value << endl;
    cout << '\t' << "Count<char, 'a', 'a'>::value = "<< Count<char,'a','a'>::value << endl;
    cout << '\t' <<"Count<int, 5, 5, 5, 6>:: value = "<< Count<int,5,5,5,6>::value << endl;
    cout << "== [ Count test = end ] ==" << endl;

    // Question 2
    cout << "== [ Found N Length Subset test = begin ] ==" << endl;
    cout << '\t' << "Found_N_Length_Subset<int, 1, 3>::Yes = " << Found_N_Length_Subset<int, 1, 3>::Yes << endl;
    cout << '\t' << "Found_N_Length_Subset<int, 0>::Yes = " << Found_N_Length_Subset<int, 0>::Yes << endl;
    cout << '\t' << "Found_N_Length_Subset<char, 2, 'a', 'b', 'a'>::Yes = " << Found_N_Length_Subset<char, 2, 'a', 'b', 'a'>::Yes << endl;
    cout << '\t' << "Found_N_Length_Subset<char, 2, 'a', 'b', 'a', 'a'>::Yes = " << Found_N_Length_Subset<char, 2, 'a', 'b', 'a', 'a'>::Yes << endl;
    cout << "== [ Found N Length Subset test = end ] ==" << endl;

    // Question 3
    cout << "== [ Longuest_Subset test = begin ] ==" << endl;
    cout << '\t' << "Longuest_Subset<int>::value = " << Longuest_Subset<int>::value << endl;
    cout << '\t' << "Longuest_Subset<char, 'a'>::value = " << Longuest_Subset<char, 'a'>::value << endl;
    cout << '\t' << "Longuest_Subset<int, 0, 1, 0, 0, 1, 1, 1, 0>::value = " << Longuest_Subset<int, 0, 1, 0, 0, 1, 1, 1, 0>::value << endl;
    cout << "== [ Longuest_Subset test = end ] ==" << endl;
    return EXIT_SUCCESS;
}