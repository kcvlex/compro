#pragma once
#include "base.hpp"

namespace math {

template <typename T>
struct convolution {
    using value_type = T::value_type;

    template <typename InputIterator1, typename InputIterator2, typename OutputIterator>
    void multiply(InputIterator1 begin1, InputIterator1 end1,
                  InputIterator2 begin2, InputIterator2 end2,
                  OutputIterator out)
    {
        static_cast<T*>(this)->multiply(begin1, end1, begin2, end2, out);
    }
};

}
