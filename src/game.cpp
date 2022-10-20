#include "Cursor.hpp"
#include "Graphics/2D/Tilemap.hpp"
#include "Utilities/Controllers/KeyboardController.hpp"
#include <Graphics/2D/Sprite.hpp>
#include <Stardust-Celeste.hpp>
#include <Utilities/Controllers/VitaController.hpp>
#include <Utilities/Input.hpp>

#include "EntityManager.hpp"
#include "TurnSystem.hpp"
#include <World.hpp>

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Utilities::Input;

class GameState : public Core::ApplicationState {

public:
  GameState() {

    TurnSystem::instance().nextTurn(); // we start in the enemy turn.
  }

  void on_update(Core::Application *app, double dt) {

    appref = app;
    Utilities::Input::update();


    TurnSystem::instance().executeActualTurn();

    World::instance().update(dt);
  }
  void on_draw(Core::Application *app, double dt) { World::instance().draw(); }

  void on_start() {
    SC_APP_INFO("Hello World!");

    Rendering::RenderContext::get().matrix_ortho(0, 480, 272, 0, -1, 1);

    Rendering::RenderContext::get().set_mode_2D();

    EntityManager::instance();
    World::instance().generateMap();
  }

  void on_cleanup() {}

private:
  bool added;
  const int secret_value = 12;
  Core::Application *appref;

  u32 tex_id;
};

class GameApplication : public Core::Application {
public:
  void on_start() override {
    state = create_refptr<GameState>();
    Application::get().push_state(state);
  }

private:
  RefPtr<GameState> state;
};

Core::Application *CreateNewSCApp() {
  Core::AppConfig config;
  config.headless = false;

  Core::PlatformLayer::get().initialize(config);

  return new GameApplication();
}