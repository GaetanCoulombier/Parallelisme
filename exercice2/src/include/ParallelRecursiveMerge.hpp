#ifndef ParallelRecursiveMerge_hpp
#define ParallelRecursiveMerge_hpp

#include <functional>
#include <algorithm>
#include <iterator>
#include <tbb/parallel_invoke.h>
#include <chrono>

namespace merging {

  class ParallelRecursiveMerge {
  public:

    template<typename InputRandomAccessIterator1,
             typename InputRandomAccessIterator2,
             typename OutputRandomAccessIterator,
             typename Compare>
    static OutputRandomAccessIterator apply(const InputRandomAccessIterator1& first1,
                                            const InputRandomAccessIterator1& last1,
                                            const InputRandomAccessIterator2& first2,
                                            const InputRandomAccessIterator2& last2,
                                            const OutputRandomAccessIterator& result,
                                            const Compare& comp,
                                            const size_t& cutoff) {

      strategyA(first1, last1, first2, last2, result, comp, cutoff);

      // Respect de la sémantique de l'algorithme merge.
      return result + (last1 - first1) + (last2 - first2);
    }

    template<typename InputRandomAccessIterator1,
             typename InputRandomAccessIterator2,
             typename OutputRandomAccessIterator>
    static OutputRandomAccessIterator apply(const InputRandomAccessIterator1& first1,
                                            const InputRandomAccessIterator1& last1,
                                            const InputRandomAccessIterator2& first2,
                                            const InputRandomAccessIterator2& last2,
                                            const OutputRandomAccessIterator& result,
                                            const size_t& cutoff) {
      typedef std::iterator_traits<InputRandomAccessIterator1> Traits;
      typedef typename Traits::value_type value_type;

      return apply(first1, last1, first2, last2, result, std::less<value_type>(), cutoff);
    }

  protected:

    template<typename InputRandomAccessIterator1,
             typename InputRandomAccessIterator2,
             typename OutputRandomAccessIterator,
             typename Compare>
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

      if (static_cast<size_t>(size1 + size2) < cutoff) {
        std::merge(first1, last1, first2, last2, result, comp);
        return;
      }

      if (size1 < size2) {
        strategyA(first2, last2, first1, last1, result, comp, cutoff);
        return;
      }

      const InputRandomAccessIterator1 middle1 = first1 + size1 / 2 + size1 % 2;
      const InputRandomAccessIterator2 middle2 = std::lower_bound(first2, last2, *middle1, comp);
      const OutputRandomAccessIterator middle3 = result + (middle1 - first1) + (middle2 - first2);

      *middle3 = *middle1;

      tbb::parallel_invoke(
        [&]() {
          strategyA(first1, middle1, first2, middle2, result, comp, cutoff);
        },
        [&]() {
          strategyA(middle1 + 1, last1, middle2, last2, middle3 + 1, comp, cutoff);
        }
      );
    }

  }; // ParallelRecursiveMerge

} // namespace merging

#endif
