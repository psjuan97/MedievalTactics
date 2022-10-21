#pragma once

#include "Graphics/2D/FontRenderer.hpp"
#include "Utilities/Controller.hpp"
#include "Utilities/Controllers/KeyboardController.hpp"

#include <Stardust-Celeste.hpp>
#include <Utilities/Input.hpp>
#include <string>

#include "Utilities/Controllers/PSPController.hpp"
#include "World.hpp"

using namespace Stardust_Celeste;
using namespace Stardust_Celeste::Utilities;
using namespace Stardust_Celeste::Utilities::Input;
class GameState;

static void move(std::any d);
static void moveEntity(std::any d);
static void select(std::any d);
static void enemyTurn(std::any t);
static void heroAttack(std::any t);
static void exitPause(std::any t);

class TurnSystem : public Singleton {

public:
  static TurnSystem &instance() {
    static TurnSystem INSTANCE;
    return INSTANCE;
  }

  void PreEnemyTurn() {
    keyCtr->clear_command();
    pspCtr->clear_command();
    Utilities::Input::clear_controller();

    auto heros = World::instance().getHeroEnt();
    for (auto e : heros) {
      e->isMoved = false;
    }
  }

  void PrePayerTurn() {
    font_renderer->clear();
    nTurn--;

    auto str = "Round " + std::to_string(nTurn);
    auto size = TurnSystem::instance().font_renderer->calculate_size(str);
    TurnSystem::instance().font_renderer->add_text(
        str, glm::vec2(20, 20), Rendering::Color{255, 255, 255, 255}, 1);

    TurnSystem::instance().font_renderer->rebuild();

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Up),
                         Utilities::KeyFlag::Press},
                        {move, Direction::up});

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Down),
                         Utilities::KeyFlag::Press},
                        {move, Direction::down});

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Left),
                         Utilities::KeyFlag::Press},
                        {move, Direction::left});

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Right),
                         Utilities::KeyFlag::Press},
                        {move, Direction::right});

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Cross),
                         Utilities::KeyFlag::Press},
                        {select, Direction::right});

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Square),
                         Utilities::KeyFlag::Press},
                        {moveEntity, Direction::right});

    pspCtr->add_command(
        {static_cast<int>(Utilities::Input::PSPButtons::Triangle),
         Utilities::KeyFlag::Press},
        {enemyTurn, Direction::right});

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Circle),
                         Utilities::KeyFlag::Press},
                        {heroAttack, Direction::right});

    ////////////////
    // KEYBOARD
    ///////////////

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::W),
                         Utilities::KeyFlag::Press},
                        {move, Direction::up});

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::S),
                         Utilities::KeyFlag::Press},
                        {move, Direction::down});

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::A),
                         Utilities::KeyFlag::Press},
                        {move, Direction::left});

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::D),
                         Utilities::KeyFlag::Press},
                        {move, Direction::right});

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::Space),
                         Utilities::KeyFlag::Press},
                        {select, Direction::right});

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::M),
                         Utilities::KeyFlag::Press},
                        {moveEntity, Direction::right});

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::T),
                         Utilities::KeyFlag::Press},
                        {enemyTurn, Direction::right});

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::K),
                         Utilities::KeyFlag::Press},
                        {heroAttack, Direction::right});

    Utilities::Input::add_controller(keyCtr);
    Utilities::Input::add_controller(pspCtr);
  }

  void PlayerTurn() {
    // check if end turn if press and change turn
  }

  void EnemyTurn() {

    auto enemies = World::instance().getEnemies();
    // get obbejtvies


    for (auto e : enemies) {

      if (e->pedingAction == 2) {
        return;
      }

      if (e->pedingAction == 1) {
        e->attack(e->getObjetive());

        return;
      }
    }

    World::instance().enemyTurn();

    TurnSystem::instance().nextTurn();

    // Do previus actions
    // execute actions one by one
    // search one enemy, apply actions (animation + damage ) (we have two now
    // when an action is finished or not)
    // search other enemy, apply actions
    // Spawn new enemies and move to new locations
    // the same at last actions

    // Tell the player the actions
    // one by one tell the actions

    // End enemy turn
  }

  void nextTurn() {
    SC_APP_INFO("---------");
    SC_APP_INFO("- RONDA {}", nTurn);
    SC_APP_INFO("---------");
    if (turn == 0) {
      turn = 1;
      PreEnemyTurn();
    } else {
      turn = 0;
      PrePayerTurn();
    }
  }

  void executeActualTurn() {
    if (turn == 0) {
      PlayerTurn();
    } else {
      EnemyTurn();
    }
  }

  Controller *getKeyboardController() { return keyCtr; }
  Controller *getPspController() { return pspCtr; }

  int getTurn() { return nTurn; };
  void setTurn(int n) { nTurn = n; };

  void PauseMenu(std::string text) {
    PreEnemyTurn();

    font_renderer->clear();

    auto str = text;
    auto size = font_renderer->calculate_size(str);
    font_renderer->add_text(str,
                            glm::vec2(W_WINDOW / 2 - size / 2, H_WINDOW / 2),
                            Rendering::Color{255, 255, 255, 255}, 1);

    font_renderer->rebuild();

    keyCtr->add_command({static_cast<int>(Utilities::Input::Keys::X),
                         Utilities::KeyFlag::Press},
                        {exitPause, Direction::right});

    pspCtr->add_command({static_cast<int>(Utilities::Input::PSPButtons::Cross),
                         Utilities::KeyFlag::Press},
                        {exitPause, Direction::right});

    Utilities::Input::add_controller(keyCtr);
    Utilities::Input::add_controller(pspCtr);
  }

  RefPtr<FontRenderer> font_renderer = nullptr;

private:
  TurnSystem() {
    keyCtr = new Utilities::Input::KeyboardController();
    pspCtr = new Utilities::Input::PSPController();
  }

  int nTurn = 6;

  bool isEnemyActionRunning = false;
  Controller *keyCtr = nullptr;
  Controller *pspCtr = nullptr;
  int turn = 1;
};

static void move(std::any d) {
  SC_APP_INFO("LETS move the cursor!");
  World::instance().getCursor()->move(std::any_cast<Direction>(d));
}

static void moveEntity(std::any d) {
  SC_APP_INFO("LETS move the entity!");
  World::instance().getCursor()->moveEntity();
}

static void select(std::any d) {
  SC_APP_INFO("LETS select the current entity!");
  World::instance().getCursor()->selectEntity();
}

static void enemyTurn(std::any t) {
  SC_APP_INFO("ENEMY TURN");

  TurnSystem::instance().nextTurn();
}

static void heroAttack(std::any t) {
  SC_APP_INFO("HERO IS GOING TO ATTACK");

  World::instance().getCursor()->attackEntity();
}

static void exitPause(std::any t) {
  SC_APP_INFO("We want to exist pause");
  TurnSystem::instance().font_renderer->clear();
  TurnSystem::instance().font_renderer->rebuild();

  // this value maybe is better to be dynamic
  TurnSystem::instance().setTurn(14);

  TurnSystem::instance().PrePayerTurn();
}
