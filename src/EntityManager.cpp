#include "EntityManager.hpp"
using namespace Stardust_Celeste;

EntityManager::EntityManager() {

  entityBlueprints = {};
  // Generar todas las entidades

  u32 tex_floor_id = Rendering::TextureManager::get().load_texture(
      "./assets/suelo.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true,
      false, true);

  u32 text_tree_id = Rendering::TextureManager::get().load_texture(
      "./assets/arbol1.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true,
      false, true);

  u32 text_tree2_id = Rendering::TextureManager::get().load_texture(
      "./assets/arbol2.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST, true,
      false, true);

  u32 text_cursor_id = Rendering::TextureManager::get().load_texture(
      "./assets/cursor.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  u32 text_sword1_id = Rendering::TextureManager::get().load_texture(
      "./assets/swordidledown.png", SC_TEX_FILTER_NEAREST,
      SC_TEX_FILTER_NEAREST, true);

  u32 text_sword2_id = Rendering::TextureManager::get().load_texture(
      "./assets/swordidleup.png", SC_TEX_FILTER_NEAREST, SC_TEX_FILTER_NEAREST,
      true);

  u32 text_orc_id = Rendering::TextureManager::get().load_texture(
      "./assets/orc_iddle_down.png", SC_TEX_FILTER_NEAREST,
      SC_TEX_FILTER_NEAREST, true);

  entityBlueprints[EntityEnum::LightTree] =
      EntityProperties{0, RoleEntity::Decor, false, 1, 0, text_tree_id};
  entityBlueprints[EntityEnum::DarkTree] =
      EntityProperties{0, RoleEntity::Decor, false, 1, 0, text_tree2_id};
  entityBlueprints[EntityEnum::Orc] =
      EntityProperties{0, RoleEntity::Enemy, true, 3, 1, text_orc_id};
  entityBlueprints[EntityEnum::Soldier] =
      EntityProperties{0, RoleEntity::Hero, true, 5, 1, text_sword1_id};
}

// i dont know if is necessary that a entity have the coords
RefPtr<Entity> EntityManager::createEntity(EntityEnum type, glm::vec2 vec) {
  if (type == EntityEnum::None)
    return nullptr;
  EntityProperties props = entityBlueprints[type];
  auto entity = create_refptr<Entity>(props.textureId, vec, props.isAnimated);
  return entity;
}