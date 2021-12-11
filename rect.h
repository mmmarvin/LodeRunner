#ifndef RECT_H
#define RECT_H

#include "vec.h"

template<class T>
struct Rectangle
{
  Rectangle() : left(0), top(0), width(0), height(0) {}
  Rectangle(T left_, T top_, T width_, T height_) : left(left_), top(top_), width(width_), height(height_) {}

  T left;
  T top;
  T width;
  T height;
};

template<class T> bool contains(const Rectangle<T>& r, const Vector2<T>& v);
template<class T> bool intersects(const Rectangle<T>& r1, const Rectangle<T>& r2);

#include "rect.inl"

#endif // RECT_H
