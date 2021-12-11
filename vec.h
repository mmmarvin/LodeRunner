#ifndef VEC_H
#define VEC_H

template<class T>
struct Vector2
{
  Vector2() : x(0), y(0) {}
  Vector2(T x_, T y_) : x(x_), y(y_) {}

  T x;
  T y;
};

template<class T> Vector2<T>& operator+=(Vector2<T>& lhs, const Vector2<T>& rhs);
template<class T> Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs);
template<class T> Vector2<T>& operator-=(Vector2<T>& lhs, const Vector2<T>& rhs);
template<class T> Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs);

using Vector2i = Vector2<int>;
using Vector2u = Vector2<unsigned int>;
using Vector2f = Vector2<float>;

#include "vec.inl"

#endif // VEC_H
