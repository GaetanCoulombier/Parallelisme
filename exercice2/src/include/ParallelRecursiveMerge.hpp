#ifndef ParallelRecursiveMerge_hpp
#define ParallelRecursiveMerge_hpp

#include <functional>
#include <algorithm>
#include <omp.h>

namespace merging {

  /**
   * @class ParallelRecursiveMerge ParallelRecursiveMerge.hpp
   *
   * Version OpenMP de l'algorithme merge de la biblothèque standard. 
   * 
   * @note L'implémentation proposée est celle du recursive merging décrite dans
   *   Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest and Clifford 
   *   Stein, "Introduction to Algorithms", 3rd ed., 2009, pp 798-802. La 
   *   récursion est interrompue lorsque la somme des tailles des deux 
   *   sous-conteneurs à fusionner passe sous une certaine tolérance. La fusion 
   *   est alors effectuée via l'algorithme merge de la bibliothèque standard.
   */
  class ParallelRecursiveMerge {
  public:

    /**
     * Forme générale de l'algorithme.
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     * @return un itérateur repérant la fin de la zone de fusion dans le
     *   conteneur cible.
     */
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator,
	      typename Compare >
    static OutputRandomAccessIterator 
    apply(const InputRandomAccessIterator1& first1,
	  const InputRandomAccessIterator1& last1,
	  const InputRandomAccessIterator2& first2,
	  const InputRandomAccessIterator2& last2,
	  const OutputRandomAccessIterator& result,
	  const Compare& comp,
	  const size_t& cutoff) {

      // Invocation de la stratégie adéquate.
      strategyB(first1, 
		last1, 
		first2, 
		last2, 
		result, 
		comp, 
		cutoff);
      
      // Respect de la sémantique de l'algorithme merge.
      return result + (last1 - first1) + (last2 - first2);

    } // apply

    /**
     * Forme spécifique de l'algorithme pour la relation d'ordre total 
     * strictement inférieur à.
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     * @return un itérateur repérant la fin de la zone de fusion dans le
     *   conteneur cible.
     */
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator >
    static OutputRandomAccessIterator
    apply(const InputRandomAccessIterator1& first1,
	  const InputRandomAccessIterator1& last1,
	  const InputRandomAccessIterator2& first2,
	  const InputRandomAccessIterator2& last2,
	  const OutputRandomAccessIterator& result,
	  const size_t& cutoff) {

      // Type synonyme pour le type des éléments du premier conteneur. Le type 
      // des éléments du second devra pouvoir se convertir implicitement en le 
      // type des éléments du premier.
      typedef std::iterator_traits< InputRandomAccessIterator1 > Traits;
      typedef typename Traits::value_type value_type;

      // Fabriquer le comparateur less puis invoquer la méthode définie 
      // ci-dessus.
      return apply(first1, 
		   last1,
		   first2,
		   last2,
		   result,
		   std::less< const value_type& >(),
		   cutoff);
      
    } // apply

  protected:

    /**
     * Implémentation MIMD de cet algorithme basée sur l'utilisation de
     * sections parallèles.
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     * @return un itérateur repérant la fin de la zone de fusion dans le
     *   conteneur cible.
     */
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator,
	      typename Compare >
    static void strategyA(const InputRandomAccessIterator1& first1,
			  const InputRandomAccessIterator1& last1,
			  const InputRandomAccessIterator2& first2,
			  const InputRandomAccessIterator2& last2,
			  const OutputRandomAccessIterator& result,
			  const Compare& comp,
			  const size_t& cutoff) {
 
      // Taille des deux sous-conteneurs.
      const auto size1 = last1 - first1;
      const auto size2 = last2 - first2;

      // Nous sommes passés sous la tolérance : la récursion s'arrête.
      if (static_cast< size_t >(size1 + size2) < cutoff) {
        std::merge(first1, last1, first2, last2, result, comp);
	return;
      }

      // Le sous-conteneur correspondant à l'opérande gauche est par convention 
      // plus long que celui correspondant à l'opérande droit. Si ce n'est pas
      // le cas, nous remettons les choses en ordre.
      if (size1 < size2) {
        strategyA(first2, 
		  last2, 
		  first1, 
		  last1, 
		  result,	  
		  comp,
		  cutoff);
	return;
      }

      // Itérateur repérant l'élément médian dans le sous-conteneur gauche.
      const InputRandomAccessIterator1 middle1 = 
	first1 + size1 / 2 + size1 % 2;

      // Itérateur repérant l'élément pivot dans le sous-conteneur droit.
      const InputRandomAccessIterator2 middle2 = 
	std::lower_bound(first2, last2, *middle1, comp);

      // Itérateur répérant la position du sous-conteneur résultat à laquelle 
      // sera implanté l'élément médian du sous-conteneur gauche.
      const OutputRandomAccessIterator middle3 = 
	result + (middle1 - first1) + (middle2 - first2);

      // Recopie de l'élément médian du sous-conteneur gauche dans le 
      // sous-conteneur résultat.
      *middle3 = *middle1;

      # pragma omp parallel sections
      {
            # pragma omp section
            {
                  // Fusion de la partie située à gauche de l'élément médian dans le 
                  // sous-conteneur gauche avec la partie située à gauche de l'élément 
                  // pivot dans le sous-conteneur droit.
                  strategyA(first1, 
                        middle1,
                        first2, 
                        middle2,
                        result,
                        comp,
                        cutoff);
            }
            # pragma omp section
            {
                  // Fusion de la partie située à droite de l'élément médian dans le 
                  // sous-conteneur gauche avec la partie située à droite (élément pivot 
                  // inclu) de l'élément pivot dans le sous-conteneur droit.
                  strategyA(middle1 + 1, 
                        last1,
                        middle2, 
                        last2,
                        middle3 + 1,
                        comp,
                        cutoff);
            }
      }

    } // strategyA

    /**
     * Implémentation MIMD de cet algorithme basée sur l'utilisation de tâches.
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     */
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator,
	      typename Compare >
    static void strategyB(const InputRandomAccessIterator1& first1,
			  const InputRandomAccessIterator1& last1,
			  const InputRandomAccessIterator2& first2,
			  const InputRandomAccessIterator2& last2,
			  const OutputRandomAccessIterator& result,
			  const Compare& comp,
			  const size_t& cutoff) {

      #pragma omp parallel
      {
            #pragma omp single
            {
                  strategyBTasking(
                  first1,
                  last1,
                  first2,
                  last2,
                  result,
                  comp,
                  cutoff);
            }
      }

    } // strategyB

    /**
     * Partie récursive de la stratégie de parallélisation @c strategyB.
     *
     * @param[in] first1 - un itérateur repérant le premier élément du premier
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last1 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du premier sous-conteneur concerné par la fusion ;
     * @param[in] first2 - un itérateur repérant le premier élément du second
     *   sous-conteneur concerné par la fusion ;
     * @param[in] last2 - un itérateur repérant l'élément situé juste derrière 
     *   le dernier élément du second sous-conteneur concerné par la fusion ;
     * @param[in] result - un itérateur repérant la position ou récopier le 
     *   premier élément résultant de la fusion ;
     * @param[in] comp - un comparateur binaire représentant la relation d'ordre
     *   total régissant les sous-conteneurs ;
     * @param[in] cutoff - la somme des tailles des deux sous-conteneurs au 
     *   dessous de laquelle la fusion est effectuée via l'algorithme merge de 
     *   la bibliothèque standard.
     */    
    template< typename InputRandomAccessIterator1,
	      typename InputRandomAccessIterator2,
	      typename OutputRandomAccessIterator,
	      typename Compare >
    static void strategyBTasking(const InputRandomAccessIterator1& first1,
				 const InputRandomAccessIterator1& last1,
				 const InputRandomAccessIterator2& first2,
				 const InputRandomAccessIterator2& last2,
				 const OutputRandomAccessIterator& result,
				 const Compare& comp,
				 const size_t& cutoff) {

      // Taille des deux sous-conteneurs.
      const auto size1 = last1 - first1;
      const auto size2 = last2 - first2;

      // Nous sommes passés sous la tolérance : la récursion s'arrête.
      if (static_cast< size_t >(size1 + size2) < cutoff) {
	std::merge(first1, last1, first2, last2, result, comp);
	return;
      }

      // Le sous-conteneur correspondant à l'opérande gauche est par convention 
      // plus long que celui correspondant à l'opérande droit. Si ce n'est pas
      // le cas, nous remettons les choses en ordre.
      if (size1 < size2) {
	strategyBTasking(first2, 
			 last2, 
			 first1, 
			 last1, 
			 result,	  
			 comp,
			 cutoff);
	return;
      }

      // Itérateur repérant l'élément médian dans le sous-conteneur gauche.
      const InputRandomAccessIterator1 middle1 = 
	first1 + size1 / 2 + size1 % 2;

      // Itérateur repérant l'élément pivot dans le sous-conteneur droit.
      const InputRandomAccessIterator2 middle2 = 
	std::lower_bound(first2, last2, *middle1, comp);

      // Itérateur répérant la position du sous-conteneur résultat à laquelle 
      // sera implanté l'élément médian du sous-conteneur gauche.
      const OutputRandomAccessIterator middle3 = 
	result + (middle1 - first1) + (middle2 - first2);

      // Recopie de l'élément médian du sous-conteneur gauche dans le 
      // sous-conteneur résultat.
      *middle3 = *middle1;

      #pragma omp task
      {
            // Fusion de la partie située à gauche de l'élément médian dans le 
            // sous-conteneur gauche avec la partie située à gauche de l'élément 
            // pivot dans le sous-conteneur droit.
            strategyBTasking(first1, 
                        middle1,
                        first2, 
                        middle2,
                        result,
                        comp,
                        cutoff);
      }
      #pragma omp task
      {
            // Fusion de la partie située à droite de l'élément médian dans le 
            // sous-conteneur gauche avec la partie située à droite (élément pivot 
            // inclu) de l'élément pivot dans le sous-conteneur droit.
            strategyBTasking(middle1 + 1, 
                        last1,
                        middle2, 
                        last2,
                        middle3 + 1,
                        comp,
                        cutoff);

      } // strategyBTasking

      #pragma omp taskwait
    }

  }; // ParallelRecursiveMerge

} // merging

#endif
