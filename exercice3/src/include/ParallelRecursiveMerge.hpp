#ifndef ParallelRecursiveMerge_hpp
#define ParallelRecursiveMerge_hpp

#include <functional>
#include <algorithm>
#include <tbb/task_group.h>
#include <iterator>

namespace merging {

class ParallelRecursiveMerge {
public:
    template<typename InputRandomAccessIterator1,
             typename InputRandomAccessIterator2,
             typename OutputRandomAccessIterator,
             typename Compare>
    static OutputRandomAccessIterator
    apply(const InputRandomAccessIterator1& first1,
          const InputRandomAccessIterator1& last1,
          const InputRandomAccessIterator2& first2,
          const InputRandomAccessIterator2& last2,
          const OutputRandomAccessIterator& result,
          const Compare& comp,
          const size_t& cutoff) {

        // Utilisation de la stratégie parallèle via TBB
        strategyB(first1, last1, first2, last2, result, comp, cutoff);

        // Retourner la fin du conteneur résultant
        return result + (last1 - first1) + (last2 - first2);
    }

    template<typename InputRandomAccessIterator1,
             typename InputRandomAccessIterator2,
             typename OutputRandomAccessIterator>
    static OutputRandomAccessIterator
    apply(const InputRandomAccessIterator1& first1,
          const InputRandomAccessIterator1& last1,
          const InputRandomAccessIterator2& first2,
          const InputRandomAccessIterator2& last2,
          const OutputRandomAccessIterator& result,
          const size_t& cutoff) {

        using value_type = typename std::iterator_traits<InputRandomAccessIterator1>::value_type;

        // Utiliser un comparateur par défaut (less)
        return apply(first1, last1, first2, last2, result, std::less<const value_type&>(), cutoff);
    }

protected:
    template<typename InputRandomAccessIterator1,
             typename InputRandomAccessIterator2,
             typename OutputRandomAccessIterator,
             typename Compare>
    static void strategyB(const InputRandomAccessIterator1& first1,
                          const InputRandomAccessIterator1& last1,
                          const InputRandomAccessIterator2& first2,
                          const InputRandomAccessIterator2& last2,
                          const OutputRandomAccessIterator& result,
                          const Compare& comp,
                          const size_t& cutoff) {

        tbb::task_group tg;
        strategyBTasking(first1, last1, first2, last2, result, comp, cutoff, tg);
        tg.wait();
    }

    template<typename InputRandomAccessIterator1,
             typename InputRandomAccessIterator2,
             typename OutputRandomAccessIterator,
             typename Compare>
    static void strategyBTasking(const InputRandomAccessIterator1& first1,
                                  const InputRandomAccessIterator1& last1,
                                  const InputRandomAccessIterator2& first2,
                                  const InputRandomAccessIterator2& last2,
                                  const OutputRandomAccessIterator& result,
                                  const Compare& comp,
                                  const size_t& cutoff,
                                  tbb::task_group& tg) {

        const auto size1 = last1 - first1;
        const auto size2 = last2 - first2;

        // Seuil pour la terminaison de la récursion
        if (static_cast<size_t>(size1 + size2) < cutoff) {
            std::merge(first1, last1, first2, last2, result, comp);
            return;
        }

        // S'assurer que le premier conteneur est plus grand
        if (size1 < size2) {
            strategyBTasking(first2, last2, first1, last1, result, comp, cutoff, tg);
            return;
        }

        const auto middle1 = first1 + size1 / 2 + size1 % 2;
        const auto middle2 = std::lower_bound(first2, last2, *middle1, comp);
        const auto middle3 = result + (middle1 - first1) + (middle2 - first2);

        *middle3 = *middle1;

        // Créer deux tâches pour chaque moitié
        tg.run([&] {
            strategyBTasking(first1, middle1, first2, middle2, result, comp, cutoff, tg);
        });

        tg.run([&] {
            strategyBTasking(middle1 + 1, last1, middle2, last2, middle3 + 1, comp, cutoff, tg);
        });
    }
};

} // namespace merging

#endif
