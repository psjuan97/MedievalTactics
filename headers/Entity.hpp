#pragma once

#include <Graphics/2D/AnimatedSprite.hpp>
#include <Graphics/2D/Sprite.hpp>

#include <Stardust-Celeste.hpp>
#include <cstddef>
#include <unordered_map>
#include <vector>

#include "ID_CONSTANTS.hpp"
#include "Utils.hpp"
#include "fwd.hpp"

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Utilities::Input;
using namespace Stardust_Celeste::Graphics::G2D;

enum EntityEnum { None, LightTree, DarkTree, Soldier, Orc, House };

enum RoleEntity { Hero, Enemy, Decor, Building };

struct EntityProperties {
  EntityEnum enumType;
  RoleEntity role;
  bool isAnimated;
  int life;
  int attack;
  int moveDistance;
  u32 textureId;
  u32 texactionId;
};

class Entity : public std::enable_shared_from_this<Entity> {

public:
  static std::unordered_map<int, u32> sprite_map;

public:
  Entity(u32 idSprite, glm::vec2 coords, bool isAnimated = false);

  void attack();

  void select() { sprite->set_color(Rendering::Color{255, 0, 255, 255}); }

  void unSelect() { sprite->set_color(Rendering::Color{255, 255, 255, 255}); }

  void setArrow(u32 idSprite);

  RefPtr<Graphics::G2D::Sprite> getArrow();

  void update(double dt);

  void draw();

  glm::vec2 getCoords();

  // POR CADA SER COORDS HAY QUE MODIDICA EL TILEMAP
  void setCoords(glm::vec2 c);

  EntityProperties props;
  bool isMoved = false;
  int pedingAction = 0; // 0 no peding, 1 pending, 2 in progress

  void refreshAttackSprite() {
    auto pos = calculate_pixels(coords.x, coords.y);

    actionSprite->set_position(pos);
  }

  void setActionSprite(u32 id) {
    auto pos = calculate_pixels(coords.x, coords.y);

    auto bounds =
        Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

    actionSprite = create_scopeptr<AnimatedSprite>(id, bounds, glm::vec2{7, 1});
    actionSprite->set_layer(1);
    actionSprite->ticksPerSec = 12;
  }

  void removeArrow() { arrow = nullptr; }

protected:
  glm::vec2 coords;
  u32 idSprite;
  u32 idAttackSprite;
  RefPtr<Graphics::G2D::Sprite> sprite = nullptr;
  RefPtr<Graphics::G2D::Sprite> arrow = nullptr;
  RefPtr<Graphics::G2D::AnimatedSprite> actionSprite = nullptr;
};