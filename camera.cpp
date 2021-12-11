#include "camera.h"

Camera::Camera() :
  m_center(0, 0),
  m_size(0, 0)
{
}

Camera::Camera(float centerX, float centerY, float width, float height) :
  m_center(centerX, centerY),
  m_size(width, height)
{
}

Camera::Camera(const Vector2f& center, const Vector2f& size) :
  m_center(center),
  m_size(size)
{
}

void Camera::setCenter(float centerX, float centerY)
{
  m_center.x = centerX;
  m_center.y = centerY;
}

void Camera::setCenter(const Vector2f& center)
{
  setCenter(center.x, center.y);
}

const Vector2f& Camera::getCenter() const
{
  return m_center;
}

void Camera::setSize(float width, float height)
{
  m_size.x = width;
  m_size.y = height;
}

void Camera::setSize(const Vector2f& size)
{
  setSize(size.x, size.y);
}

const Vector2f& Camera::getSize() const
{
  return m_size;
}
