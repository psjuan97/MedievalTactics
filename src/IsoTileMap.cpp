#include "IsoTileMap.hpp"
#include "EntityManager.hpp"
#include "ID_CONSTANTS.hpp"
#include "Utils.hpp"

void ISOTileMap::add_tile(ISOTile tile) {}

auto ISOTileMap::generate_map() -> void {
  mesh->delete_data();

  vert_data.clear();
  vert_data.shrink_to_fit();
  idx_data.clear();
  idx_data.shrink_to_fit();

  auto idxc = 0;

  for (auto &t : tileMap) {
    auto x = t->bounds.position.x;
    auto y = t->bounds.position.y;
    auto w = t->bounds.extent.x;
    auto h = t->bounds.extent.y;

    auto uvs = Rendering::Texture::get_tile_uvs(atlasDimensions, t->index);

    vert_data.push_back(
        Rendering::Vertex{uvs[0], uvs[1], t->color, x, y + h, t->layer});
    vert_data.push_back(
        Rendering::Vertex{uvs[2], uvs[3], t->color, x + w, y + h, t->layer});
    vert_data.push_back(
        Rendering::Vertex{uvs[4], uvs[5], t->color, x + w, y, t->layer});
    vert_data.push_back(
        Rendering::Vertex{uvs[6], uvs[7], t->color, x, y, t->layer});

    idx_data.insert(idx_data.end(),
                    {(u16)(idxc + 0), (u16)(idxc + 1), (u16)(idxc + 2),
                     (u16)(idxc + 2), (u16)(idxc + 3), (u16)(idxc + 0)});
    idxc += 4;
  }

  mesh->add_data(vert_data.data(), vert_data.size(), idx_data.data(),
                 idx_data.size());
}

void ISOTileMap::generate_tiles() {

  u16 map[8][8] = {{0, 0, 0, 2, 2, 2}, {1, 2, 2, 1, 1, 1}, {2, 2, 0, 1, 1, 1},
                   {2, 1, 1, 2, 1, 1}, {2, 1, 1, 1, 2, 1}, {2, 1, 1, 1, 1, 2},
                   {2, 1, 1, 1, 1, 2}, {2, 1, 1, 1, 1, 2}};

  EntityEnum entities[8][8] = {
      {EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::Orc,
       EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None},
      {EntityEnum::DarkTree, EntityEnum::DarkTree, EntityEnum::None,
       EntityEnum::None, EntityEnum::DarkTree, EntityEnum::None,
       EntityEnum::None, EntityEnum::None},
      {EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
       EntityEnum::None, EntityEnum::None, EntityEnum::LightTree,
       EntityEnum::None},
      {EntityEnum::Orc, EntityEnum::None, EntityEnum::None, EntityEnum::None,
       EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None},
      {EntityEnum::None, EntityEnum::None, EntityEnum::Orc, EntityEnum::None,
       EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None},
      {EntityEnum::None, EntityEnum::LightTree, EntityEnum::None,
       EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
       EntityEnum::None},
      {EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
       EntityEnum::None, EntityEnum::Soldier, EntityEnum::None,
       EntityEnum::None},
      {EntityEnum::None, EntityEnum::None, EntityEnum::None,
       EntityEnum::DarkTree, EntityEnum::LightTree, EntityEnum::None,
       EntityEnum::None, EntityEnum::None},
  };
  int dim_size = 8;
  // cuanta tiles tiene cada fila?
  //  row % dim_size

  for (int i = 0; i < dim_size; i++) {
    int n_tile = 0;

    for (int j = 0; j < dim_size; j++) {

      // cell i,j go to row i +j

      SC_APP_INFO("i j : {} {}", i, j);

      int screen_x = (i - j) * TILE_SIZE_W / 2 + OFFSET_X;
      int screen_y = (i + j) * TILE_SIZE_H / 2 + OFFSET_Y;
      RefPtr<Entity> entity = EntityManager::instance().createEntity(
          entities[i][j], glm::vec2{i, j});

      tileMap.push_back(

          create_refptr<ISOTile>(
              ISOTile{Rendering::Rectangle{{screen_x, screen_y},
                                           {TILE_SIZE_W, TILE_SIZE_H}},
                      Rendering::Color{255, 255, 255, 255}, 0 /*map[i][j]*/,
                      0.0f, entity})

      );

      // auto tile1 = tileMap.back(); //estoy iniciliando una copia
      // tile1.entity = create_refptr<Entity>(ENTITY, glm::vec2(1, 1));
    }
    SC_APP_INFO("\n");
  }
}