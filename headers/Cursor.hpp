#pragma once

#include "Entity.hpp"
#include "Utilities/Types.hpp"
#include <cstddef>

class World;


class Cursor : public Entity {
public:
  Cursor(u32 idSprite, glm::vec2 coords);

  auto getNearTiles(RefPtr<Entity> ent);
  void selectEntity();
  void moveEntity();
  void attackEntity();


  void move(Direction dir);

  void applyChange();

private:
  RefPtr<Entity> selectedEntity = nullptr;
};