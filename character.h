#ifndef CHARACTER_H
#define CHARACTER_H

#include <tuple>
#include "object.h"

struct Character : public Object
{
  enum EFacingDirection
  {
    EFD_LEFT,
    EFD_RIGHT
  };

  enum EClimbingDirection
  {
    ECD_UP,
    ECD_DOWN
  };

  enum ERole
  {
    ER_PLAYER,
    ER_AI
  };

  Character(float x_, float y_, ERole role_);

  std::pair<Object, bool>*  goldTaken;
  EFacingDirection          facingDirection;
  EClimbingDirection        climbingDirection;
  ERole                     role;
};

#endif // CHARACTER_H
