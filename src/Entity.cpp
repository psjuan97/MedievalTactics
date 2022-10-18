#pragma once
#include "World.hpp"
#include <Entity.hpp>
#include <cstddef>
#include <unordered_map>

std::unordered_map<int, u32> Entity::sprite_map = {};

Entity::Entity(u32 idSprite, glm::vec2 coords, bool isAnimated) {

  auto pos = calculate_pixels(coords.x, coords.y);

  auto bounds =
      Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

  if (isAnimated) {
    sprite = create_scopeptr<AnimatedSprite>(idSprite, bounds, glm::vec2{2, 1});
  } else {
    sprite = create_scopeptr<Graphics::G2D::Sprite>(idSprite, bounds);
  }

  sprite->set_layer(1);
  this->idSprite = idSprite;
  this->coords = coords;
};

void Entity::update(double dt) {
  if (sprite)
    sprite->update(dt);
  else
    SC_APP_INFO("INVISIBLE ENTITY");
}

void Entity::draw() {
  if (sprite)
    sprite->draw();
  else
    SC_APP_INFO("INVISIBLE ENTITY");
};

glm::vec2 Entity::getCoords() { return coords; }
#include <memory>

// POR CADA SER COORDS HAY QUE MODIDICA EL TILEMAP
void Entity::setCoords(glm::vec2 c) {
  this->coords = c;

  if (World::instance().getTile(coords)->entity != nullptr) {
    SC_APP_INFO("ENTIDAD VA A DESAPARECER!!");
    SC_APP_INFO("vec2 {} {}",coords.x, coords.y );


  }
  World::instance().getTile(coords)->entity = shared_from_this();
  auto pos = calculate_pixels(coords.x, coords.y);

  auto bounds =
      Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

  sprite->set_rect(bounds);
}

RefPtr<Graphics::G2D::Sprite> Entity::getArrow() { return arrow; }

void Entity::setArrow(u32 idSprite) {
  auto pos = calculate_pixels(coords.x, coords.y);
  auto bounds =
      Rendering::Rectangle{pos, glm::vec2(TILE_SIZE_W - 5, TILE_SIZE_H - 5)};

  arrow = create_scopeptr<Sprite>(idSprite, bounds);
  arrow->set_layer(1);
}
