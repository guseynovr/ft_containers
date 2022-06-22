#ifndef ALGORITHM_H
#define ALGORITHM_H

namespace ft {

template <class T>
void swap (T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

}; // namescape ft

#endif // ALGORITM_H
