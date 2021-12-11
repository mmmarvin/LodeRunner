#include "rect.h"

template<class T>
bool contains(const Rectangle<T>& r, const Vector2<T>& v)
{
  auto x1 = r.left;
  auto x2 = x1 + r.width;
  auto y1 = r.top;
  auto y2 = y1 + r.height;

  return v.x >= x1 && v.x <= x2 &&
         v.y >= y1 && v.y <= y2;
}

template<class T>
bool intersects(const Rectangle<T>& r1, const Rectangle<T>& r2)
{
  auto p1 = Vector2<T>(r2.left, r2.top);
  auto p2 = p1 + Vector2<T>(r2.width, 0);
  auto p3 = p2 + Vector2<T>(0, r2.height);
  auto p4 = p1 + Vector2<T>(0, r2.height);

  return contains(r1, p1) || contains(r1, p2) ||
         contains(r1, p3) || contains(r1, p4);
}
