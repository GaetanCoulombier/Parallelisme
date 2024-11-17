#include "Count.hpp"
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
    cout << "== [ Count test = begin ] ==" << endl;
    cout << '\t' << "Count<int, 5 >::value = " << Count<int,5>::value << endl;
    cout << '\t' << "Count<char, 'a', 'a'>::value = "<< Count<char,'a','a'>::value << endl;
    cout << '\t' <<"Count<int, 5, 5, 5, 6>:: value = "<< Count<int,5,5,5,6>::value << endl;
    cout << "== [ Count test = end ] ==" << endl;

    return EXIT_SUCCESS;
}