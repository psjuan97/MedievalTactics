#include "IsoTileMap.hpp"
#include "EntityManager.hpp"
#include "ID_CONSTANTS.hpp"
#include "Utils.hpp"
#include "fwd.hpp"

#include <queue>
using namespace std;

vector<glm::vec2> calculateBFS(glm::vec2 ori, glm::vec2 dest,
                               EntityEnum mat[8][8]);

EntityEnum entities[8][8] = {
    {EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
     EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None},
    {EntityEnum::DarkTree, EntityEnum::DarkTree, EntityEnum::None,
     EntityEnum::None, EntityEnum::DarkTree, EntityEnum::None, EntityEnum::Orc,
     EntityEnum::None},
    {EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
     EntityEnum::None, EntityEnum::None, EntityEnum::LightTree,
     EntityEnum::None},
    {EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
     EntityEnum::Orc, EntityEnum::None, EntityEnum::None, EntityEnum::None},
    {EntityEnum::None, EntityEnum::None, EntityEnum::Orc, EntityEnum::None,
     EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None},
    {EntityEnum::None, EntityEnum::LightTree, EntityEnum::None,
     EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
     EntityEnum::None},
    {EntityEnum::None, EntityEnum::None, EntityEnum::None, EntityEnum::None,
     EntityEnum::None, EntityEnum::Soldier, EntityEnum::None, EntityEnum::None},
    {EntityEnum::House, EntityEnum::None, EntityEnum::None,
     EntityEnum::DarkTree, EntityEnum::LightTree, EntityEnum::None,
     EntityEnum::House, EntityEnum::None},
};

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

#if PSP
        auto tInfo = Rendering::TextureManager::get().get_texture(texture);

        float wRatio = 1.0f;
        float hRatio = 1.0f;

        if (tInfo != nullptr) {
            wRatio = (float)tInfo->width / (float)tInfo->pW;
            hRatio = (float)tInfo->height / (float)tInfo->pH;
        }

        for (int i = 0; i < 4; i++) {
            uvs[i * 2 + 0] *= wRatio;
            uvs[i * 2 + 1] *= hRatio;
        }
#endif

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

std::vector<glm::vec2> ISOTileMap::getPath(glm::vec2 from, glm::vec2 to) {

  EntityEnum actual_entities[MAP_SIZE][MAP_SIZE];
  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++) {
      auto tile = getTile(glm::vec2{i, j});
      if (tile->entity != nullptr) {
        actual_entities[i][j] = tile->entity->props.enumType;
      } else {
        actual_entities[i][j] = EntityEnum::None;
      }
    }
  }
  auto q = calculateBFS(from, to, actual_entities);

  /* for(auto c: q ){
     this->getTile(c)->color = {125, 125, 125, 255};
     SC_APP_INFO("coord x={} y={}", c.x, c.y);

   }
   */
  SC_APP_INFO("----");

  return q;
}

// https://www.tutorialspoint.com/breadth-first-search-on-matrix-in-cplusplus
vector<glm::vec2> calculateBFS(glm::vec2 ori, glm::vec2 dest,
                               EntityEnum mat[8][8]) {
  int source_i, source_j, destination_i, destination_j;

  source_i = ori.x;
  source_j = ori.y;

  destination_i = dest.x;
  destination_j = dest.y;

  int dist[MAP_SIZE][MAP_SIZE];
  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++)
      dist[i][j] = INT_MAX;
  }

  glm::vec2 parents[MAP_SIZE][MAP_SIZE];
  for (int i = 0; i < MAP_SIZE; i++) {
    for (int j = 0; j < MAP_SIZE; j++)
      parents[i][j] = glm::vec2{-1, -1};
  }

  queue<glm::vec2> q;

  q.push(glm::vec2(source_i, source_j));
  dist[source_i][source_j] = 0;

  // modified BFS by add constraint checks
  while (!q.empty()) {
    // storing the x co-ordinate or row information of cell
    int x = q.front().x;
    // storing the y co-ordinate or column information of cell
    int y = q.front().y;

    // Remove the cell from queue
    q.pop();
    // If move towards left is allowed or it is the destnation cell
    if (y - 1 >= 0 && (mat[x][y - 1] == EntityEnum::None ||
                       mat[x][y - 1] == EntityEnum::Soldier)) {
      // if distance to reach the cell to the left is less than the computed
      // previous path distance, update it
      if (dist[x][y] + 1 < dist[x][y - 1]) {
        dist[x][y - 1] = dist[x][y] + 1;
        q.push(glm::vec2(x, y - 1));

        parents[x][y - 1] = glm::vec2{x, y};
      }
    }
    // If move towards right is allowed or it is the destination cell
    if (y + 1 < MAP_SIZE && (mat[x][y + 1] == EntityEnum::None ||
                             mat[x][y + 1] == EntityEnum::Soldier)) {
      // if distance to reach the cell to the right is less than the computed
      // previous path distance, update it
      if (dist[x][y] + 1 < dist[x][y + 1]) {
        dist[x][y + 1] = dist[x][y] + 1;
        q.push(glm::vec2(x, y + 1));
        parents[x][y + 1] = glm::vec2{x, y};
      }
    }
    // If upward direction is allowed
    if (x - 1 >= 0 && (mat[x - 1][y] == EntityEnum::None ||
                       mat[x - 1][y] == EntityEnum::Soldier)) {
      if (dist[x][y] + 1 < dist[x - 1][y]) {
        dist[x - 1][y] = dist[x][y] + 1;
        q.push(glm::vec2(x - 1, y));
        parents[x - 1][y] = glm::vec2{x, y};
      }
    }

    // If downward direction allowed
    if (x + 1 < MAP_SIZE && (mat[x + 1][y] == EntityEnum::None ||
                             mat[x + 1][y] == EntityEnum::Soldier)) {
      // if distance to reach the cell to the down is less than the computed
      // previous path distance, update it
      if (dist[x][y] + 1 < dist[x + 1][y]) {
        dist[x + 1][y] = dist[x][y] + 1;
        q.push(glm::vec2(x + 1, y));
        parents[x + 1][y] = glm::vec2{x, y};
      }
    }
  }
  vector<glm::vec2> lista = {};

  int p_x = dest.x;
  int p_y = dest.y;
  do {
    auto vec = parents[p_x][p_y];
    p_x = vec.x;
    p_y = vec.y;
    lista.insert(lista.begin(), vec);

  } while (parents[p_x][p_y] !=
           glm::vec2{-1, -1}); // float == ? meaby not a good idea

  // lista.pop();
  return lista;
}
