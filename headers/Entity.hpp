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

enum EntityEnum { None, LightTree, DarkTree, Soldier, Orc };

enum RoleEntity { Hero, Enemy, Decor };

struct EntityProperties {
  int ID;
  RoleEntity role;
  bool isAnimated;
  int life;
  int attack;
  int moveDistance;
  u32 textureId;
};

class Entity {

public:
  static std::unordered_map<int, u32> sprite_map;

public:
  Entity(u32 idSprite, glm::vec2 coords, bool isAnimated = false) {

    auto pos = calculate_pixels(coords.x, coords.y);

    auto bounds =
        Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

    if (isAnimated) {
      sprite =
          create_scopeptr<AnimatedSprite>(idSprite, bounds, glm::vec2{2, 1});
    } else {
      sprite = create_scopeptr<Graphics::G2D::Sprite>(idSprite, bounds);
    }

    sprite->set_layer(1);
    this->idSprite = idSprite;
    this->coords = coords;
  };

  void select() { sprite->set_color(Rendering::Color{255, 0, 255, 255}); }

  void unSelect() { sprite->set_color(Rendering::Color{255, 255, 255, 255}); }

  void update(double dt) {
    if (sprite)
      sprite->update(dt);
    else
      SC_APP_INFO("INVISIBLE ENTITY");
  }

  void draw() {
    if (sprite)
      sprite->draw();
    else
      SC_APP_INFO("INVISIBLE ENTITY");
  };
  EntityProperties props;

  glm::vec2 getCoords() { return coords; }

  void setCoords(glm::vec2 c) {
    coords = c;
    auto pos = calculate_pixels(coords.x, coords.y);

    auto bounds =
        Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

    sprite->set_rect(bounds);
  }

protected:
  glm::vec2 coords;
  u32 idSprite;
  RefPtr<Graphics::G2D::Sprite> sprite = nullptr;
};