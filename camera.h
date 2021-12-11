#ifndef CAMERA_H
#define CAMERA_H

#include <vec.h>

class Camera
{
public:
  Camera();
  Camera(float centerX, float centerY, float width, float height);
  Camera(const Vector2f& center, const Vector2f& size);

  void setCenter(float centerX, float centerY);
  void setCenter(const Vector2f& center);
  const Vector2f& getCenter() const;

  void setSize(float width, float height);
  void setSize(const Vector2f& size);
  const Vector2f& getSize() const;

private:
  Vector2f m_center;
  Vector2f m_size;
};

#endif // CAMERA_H
