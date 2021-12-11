#include "vec.h"

template<class T>
Vector2<T>& operator+=(Vector2<T>& lhs, const Vector2<T>& rhs)
{
  lhs.x += rhs.x;
  lhs.y += rhs.y;

  return lhs;
}

template<class T>
Vector2<T> operator+(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
  auto temp = lhs;
  temp += rhs;

  return temp;
}

template<class T>
Vector2<T>& operator-=(Vector2<T>& lhs, const Vector2<T>& rhs)
{
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;

  return lhs;
}

template<class T>
Vector2<T> operator-(const Vector2<T>& lhs, const Vector2<T>& rhs)
{
  auto temp = lhs;
  temp -= rhs;

  return temp;
}
