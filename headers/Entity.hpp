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
};

class Entity : public std::enable_shared_from_this<Entity> {

public:
  static std::unordered_map<int, u32> sprite_map;

public:
  Entity(u32 idSprite, glm::vec2 coords, bool isAnimated = false);

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
protected:
  glm::vec2 coords;
  u32 idSprite;
  RefPtr<Graphics::G2D::Sprite> sprite = nullptr;
  RefPtr<Graphics::G2D::Sprite> arrow = nullptr;
};