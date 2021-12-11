#ifndef OBJECT_H
#define OBJECT_H

struct Object
{
  enum EState
  {
    ES_IDLE,
    ES_FALLING,
    ES_FALLING_POLE,
    ES_MOVING,
    ES_CLIMBING
  };

  Object(float x_, float y_);

  float   x;
  float   y;
  EState  state;
};

#endif // OBJECT_H
