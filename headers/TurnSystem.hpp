#pragma once

#include "Utilities/Controller.hpp"
#include "Utilities/Controllers/KeyboardController.hpp"

#include <Stardust-Celeste.hpp>
#include <Utilities/Input.hpp>

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

class TurnSystem : public Singleton {

public:
  static TurnSystem &instance() {
    static TurnSystem INSTANCE;
    return INSTANCE;
  }

  void PreEnemyTurn() {
    keyCtr->clear_command();
    Utilities::Input::clear_controller();
  }

  void PrePayerTurn() {
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
  }

  void PlayerTurn() {
    // check if end turn if press and change turn
  }

  void EnemyTurn() {

    auto enemies = World::instance().getEnemies();
    for (auto e : enemies) {

      if (e->pedingAction == 2) {
        return;
      }

      if (e->pedingAction == 1) {
        e->attack();
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

private:
  TurnSystem() { keyCtr = new Utilities::Input::KeyboardController(); }
  bool isEnemyActionRunning = false;
  Controller *keyCtr = nullptr;
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
  // World::instance().enemyTurn();
}


static void heroAttack(std::any t) {
  SC_APP_INFO("HERO IS GOING TO ATTACK");

  World::instance().getCursor()->attackEntity();



  // World::instance().enemyTurn();
}



