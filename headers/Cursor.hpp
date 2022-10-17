#pragma once

#include "Entity.hpp"
#include "Utilities/Types.hpp"
#include <cstddef>

class World;

enum Direction { up, down, left, right };

class Cursor : public Entity {
public:
  Cursor(u32 idSprite, glm::vec2 coords);


  void selectEntity();

  void move(Direction dir);

  void applyChange();

private:
  glm::vec2 coord;
  RefPtr<Entity> selectedEntity = nullptr; 
};