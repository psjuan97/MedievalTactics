#include "Cursor.hpp"
#include "Graphics/2D/FontRenderer.hpp"
#include "Graphics/2D/Tilemap.hpp"

#include "Rendering/RenderContext.hpp"
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
    EntityManager::instance();
    TurnSystem::instance().font_renderer =
        create_scopeptr<Graphics::G2D::FontRenderer>(
            EntityManager::instance().fontID, glm::vec2(16, 16));

    TurnSystem::instance().font_renderer->scale_factor = 2.f;
    TurnSystem::instance().nextTurn(); // we start in the enemy turn.
  }

  void on_update(Core::Application *app, double dt) {
    int turn = TurnSystem::instance().getTurn();

    // font_renderer->clear();

    appref = app;
    Utilities::Input::update();

    // check if round is end
    if (turn < 0) {
      TurnSystem::instance().setTurn(10);
      World::instance().newLevel();
      World::instance().generateMap();
      TurnSystem::instance().PauseMenu("VICTORY. NEXT ROUND");
    }

    if (World::instance().getHouses().size() == 0) {
      TurnSystem::instance().setTurn(10);

      World::instance().newLevel();
      World::instance().generateMap();
      TurnSystem::instance().nextTurn();
      TurnSystem::instance().PauseMenu("You Loose, X Restart Game");
    }

    TurnSystem::instance().executeActualTurn();
    World::instance().update(dt);
  }
  void on_draw(Core::Application *app, double dt) {
    TurnSystem::instance().font_renderer->draw();
    World::instance().draw();
  }

  void on_start() {
    SC_APP_INFO("Medieval Tactics!");

    Rendering::RenderContext::get().matrix_ortho(0, 480, 272, 0, -1, 1);
    Rendering::RenderContext::get().set_color({0, 0, 0, 255});
    Rendering::RenderContext::get().set_mode_2D();

    World::instance().generateMap();
    //World::instance().playSound();

    /*str = "You Loose. START Again";
    size = font_renderer->calculate_size(str);
    font_renderer->add_text(str,
                            glm::vec2(W_WINDOW / 2 - size / 2, H_WINDOW / 2),
                            Rendering::Color{255, 255, 255, 255}, 1);*/
  }

  void on_cleanup() {}

private:
  bool added;
  const int secret_value = 12;
  Core::Application *appref;
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