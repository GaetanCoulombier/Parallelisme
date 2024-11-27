#ifndef ParallelRecursiveMerge_hpp
#define ParallelRecursiveMerge_hpp

#include <functional>
#include <algorithm>
#include <tbb/tbb.h>
#include <iterator>

namespace merging
{

    class ParallelRecursiveMerge
    {
    public:
        template <typename InputRandomAccessIterator1,
                  typename InputRandomAccessIterator2,
                  typename OutputRandomAccessIterator,
                  typename Compare>
        static OutputRandomAccessIterator
        apply(const InputRandomAccessIterator1 &first1,
              const InputRandomAccessIterator1 &last1,
              const InputRandomAccessIterator2 &first2,
              const InputRandomAccessIterator2 &last2,
              const OutputRandomAccessIterator &result,
              const Compare &comp,
              const size_t &cutoff)
        {

            // Utilisation de la stratégie parallèle via TBB
            strategyB(first1, last1, first2, last2, result, comp, cutoff);

            // Retourner la fin du conteneur résultant
            return result + (last1 - first1) + (last2 - first2);
        }

        template <typename InputRandomAccessIterator1,
                  typename InputRandomAccessIterator2,
                  typename OutputRandomAccessIterator>
        static OutputRandomAccessIterator
        apply(const InputRandomAccessIterator1 &first1,
              const InputRandomAccessIterator1 &last1,
              const InputRandomAccessIterator2 &first2,
              const InputRandomAccessIterator2 &last2,
              const OutputRandomAccessIterator &result,
              const size_t &cutoff)
        {

            typedef std::iterator_traits<InputRandomAccessIterator1> Traits;
            typedef typename Traits::value_type value_type;

            return apply(first1, last1, first2, last2, result, std::less<const value_type &>(), cutoff);
        }

    protected:
        template <typename InputRandomAccessIterator1,
                  typename InputRandomAccessIterator2,
                  typename OutputRandomAccessIterator,
                  typename Compare>
        static void strategyB(const InputRandomAccessIterator1 &first1,
                              const InputRandomAccessIterator1 &last1,
                              const InputRandomAccessIterator2 &first2,
                              const InputRandomAccessIterator2 &last2,
                              const OutputRandomAccessIterator &result,
                              const Compare &comp,
                              const size_t &cutoff)
        {

            tbb::task_group task_group_instance;
            strategyBTasking(first1, last1, first2, last2, result, comp, cutoff, task_group_instance);
        }

        template <typename InputRandomAccessIterator1,
                  typename InputRandomAccessIterator2,
                  typename OutputRandomAccessIterator,
                  typename Compare>
        static void strategyBTasking(const InputRandomAccessIterator1 &first1,
                                     const InputRandomAccessIterator1 &last1,
                                     const InputRandomAccessIterator2 &first2,
                                     const InputRandomAccessIterator2 &last2,
                                     const OutputRandomAccessIterator &result,
                                     const Compare &comp,
                                     const size_t &cutoff,
                                     tbb::task_group &task_group_instance)
        {
            // Taille des deux sous-conteneurs.
            const auto size1 = last1 - first1;
            const auto size2 = last2 - first2;

            // Nous sommes passés sous la tolérance : la récursion s'arrête.
            if (static_cast<size_t>(size1 + size2) < cutoff)
            {
                std::merge(first1, last1, first2, last2, result, comp);
                return;
            }

            // Le sous-conteneur correspondant à l'opérande gauche est par convention
            // plus long que celui correspondant à l'opérande droit. Si ce n'est pas
            // le cas, nous remettons les choses en ordre.
            if (size1 < size2)
            {
                strategyBTasking(first2,
                                 last2,
                                 first1,
                                 last1,
                                 result,
                                 comp,
                                 cutoff,
                                 task_group_instance);
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

            // Créer deux tâches pour chaque moitié
            task_group_instance.run([&]
                                    { strategyBTasking(first1,
                                                       middle1,
                                                       first2,
                                                       middle2,
                                                       result,
                                                       comp,
                                                       cutoff,
                                                       task_group_instance); });

            task_group_instance.run([&]
                                    { strategyBTasking(middle1 + 1,
                                                       last1,
                                                       middle2,
                                                       last2,
                                                       middle3 + 1,
                                                       comp,
                                                       cutoff,
                                                       task_group_instance); });

            task_group_instance.wait();
        }
    };

} // namespace merging

#endif
