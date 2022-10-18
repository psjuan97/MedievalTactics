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

  u32 cursorId;
  u32 cursorOffsetId;
private:
  EntityManager();

  std::unordered_map<EntityEnum, EntityProperties> entityBlueprints;
};