#include "include/Count.hpp"
#include "include/Found_N_Length_Subset.hpp"
#include <iostream>
#include <cstdlib>
#include <cstddef>

/**
 * Programme principal.
 *
 * @return @c EXIT_FAILURE car exécution toujours réussie.
 */
int
main() { 
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

    return EXIT_SUCCESS;
}