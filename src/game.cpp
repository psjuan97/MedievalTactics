#include "Cursor.hpp"
#include "Graphics/2D/Tilemap.hpp"
#include "Utilities/Controllers/KeyboardController.hpp"
#include "Utilities/Controllers/MouseController.hpp"
#include <Graphics/2D/Sprite.hpp>
#include <Stardust-Celeste.hpp>
#include <Utilities/Controllers/VitaController.hpp>
#include <Utilities/Input.hpp>

#include "EntityManager.hpp"
#include <World.hpp>

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Utilities::Input;

class GameState : public Core::ApplicationState {

public:
  GameState() : vitaCTRL(nullptr) {}

  void on_update(Core::Application *app, double dt) {
    appref = app;
    Utilities::Input::update();

    World::instance().update(dt);
  }
  void on_draw(Core::Application *app, double dt) { World::instance().draw(); }

  static void quit(std::any a) {
    SC_APP_INFO("Pressed Cross!");
    auto gs = std::any_cast<GameState *>(a);
    SC_APP_INFO("SECRET: {}", gs->secret_value);
    gs->appref->exit();
  }

  static void move(std::any d) {
    SC_APP_INFO("LETS move the cursor!");
    World::instance().getCursor()->move(std::any_cast<Direction>(d));
  }

  static void moveEntity(std::any d) {
    SC_APP_INFO("LETS move the entity!");
    World::instance().getCursor()->moveEntity();
  }

  static void attackEntity(std::any d) {
    SC_APP_INFO("LETS atacks!");
    World::instance().getCursor()->attackEntity();
  }

  static void select(std::any d) {
    SC_APP_INFO("LETS select the current entity!");
    World::instance().getCursor()->selectEntity();
  }



  static void enemyTurn(std::any t) {
    SC_APP_INFO("ENEMY TURN");
    World::instance().enemyTurn();
  }




  void on_start() {
    SC_APP_INFO("Hello World!");
    // auto vitaCTRL = new Utilities::Input::VitaController();
    auto vitaCTRL = new Utilities::Input::KeyboardController();

    vitaCTRL->add_command({static_cast<int>(Utilities::Input::Keys::W),
                           Utilities::KeyFlag::Press},
                          {GameState::move, Direction::up});

    vitaCTRL->add_command({static_cast<int>(Utilities::Input::Keys::S),
                           Utilities::KeyFlag::Press},
                          {GameState::move, Direction::down});

    vitaCTRL->add_command({static_cast<int>(Utilities::Input::Keys::A),
                           Utilities::KeyFlag::Press},
                          {GameState::move, Direction::left});

    vitaCTRL->add_command({static_cast<int>(Utilities::Input::Keys::D),
                           Utilities::KeyFlag::Press},
                          {GameState::move, Direction::right});

    vitaCTRL->add_command({static_cast<int>(Utilities::Input::Keys::Space),
                           Utilities::KeyFlag::Press},
                          {GameState::select, Direction::right});

    vitaCTRL->add_command({static_cast<int>(Utilities::Input::Keys::M),
                           Utilities::KeyFlag::Press},
                          {GameState::moveEntity, Direction::right});


    vitaCTRL->add_command({static_cast<int>(Utilities::Input::Keys::T),
                           Utilities::KeyFlag::Press},
                          {GameState::enemyTurn, Direction::right});


    Utilities::Input::add_controller(vitaCTRL);

    Rendering::RenderContext::get().matrix_ortho(0, 480, 272, 0, -1, 1);

    Rendering::RenderContext::get().set_mode_2D();

    EntityManager::instance();
    World::instance().generateMap();
  }

  void on_cleanup() {}

private:
  bool added;
  const int secret_value = 12;
  VitaController *vitaCTRL;
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