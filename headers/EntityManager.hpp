#pragma once
// Know all the info to create the entitys, an orc is enemy? can be a tree
// atacked? what is the sprite for
#include "Entity.hpp"
#include "Utilities/Singleton.hpp"
#include <Stardust-Celeste.hpp>
#include <unordered_map>




class EntityManager : public Singleton {

public:
  static EntityManager &instance() {
    static EntityManager INSTANCE;
    return INSTANCE;
  }
  RefPtr<Entity> createEntity(EntityEnum type, glm::vec2 vec);

  u32 arrow_right;
  u32 arrow_left;

  u32 arrow_up;
  u32 arrow_down;

  u32 cursorId;
  u32 cursorOffsetId;
  u32 fontID;

  u32 hud_cursor;
  u32 hud_cross;
  u32 hud_square;
  u32 hud_triangle;
  u32 hud_circle;
private:
  EntityManager();
  
  std::unordered_map<EntityEnum, EntityProperties> entityBlueprints;
};