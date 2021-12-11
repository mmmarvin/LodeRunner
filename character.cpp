#include "character.h"

Character::Character(float x_, float y_, ERole role_) :
  Object(x_, y_),
  goldTaken(nullptr),
  facingDirection(EFD_RIGHT),
  climbingDirection(ECD_UP),
  role(role_)
{
}
