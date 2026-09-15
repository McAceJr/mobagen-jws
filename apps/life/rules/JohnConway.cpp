#include "JohnConway.h"
#include "../fsm/Action.h"
#include "../fsm/AgentContext.h"
#include "../fsm/Condition.h"

#include <SDL3/SDL_log.h>

#include <stdexcept>
#include <iostream>

// The four Conway rules as machine parts:
//   underpopulation / overpopulation -> conditions that leave Alive (cell dies)
//   reproduction                     -> condition that leaves Dead (cell revives)
//   survival is implicit: no transition firing means the stay actions run.
//
// Where the data lives (read this before touching anything):
//   - the persistent state of a cell is one bit in the world grid;
//   - the Alive/Dead State objects below are shared behavior nodes, not storage:
//     every cell runs the same two nodes, they hold nothing per-cell;
//   - per-update info (position, isAlive, aliveNeighbors) travels in the AgentContext.

// begin solution
namespace conway {
class Underpopulation : public Condition {
public:
  bool Test(const AgentContext& context) override {
    // todo: implement the underpopulation condition
    // check if cell is alive
    if (context.isAlive)
    { // if neighbors is less than 2 die
      if (context.aliveNeighbors < 2)
      {
        return true;
      }
    }
    return false;
  }
};

class Overpopulation : public Condition {
public:
  bool Test(const AgentContext& context) override {
    // todo: implement the overpopulation condition
    // check if cell is alive
    if (context.isAlive)
    { // if neighbors is more than 3 die
      if (context.aliveNeighbors > 3)
      {
        return true;
      }
    }
    return false;
  }
};

class Reproduction : public Condition {
public:
  bool Test(const AgentContext& context) override {
    // todo: implement the reproduction condition
    // check if cell is dead
    if (!context.isAlive)
    { // if there are 3 alive nighbors then revive
      if (context.aliveNeighbors == 3)
      {
        return true;
      }
    }
    return false;
  }
};

class DieAction : public Action {
public:
  void Execute(const AgentContext& context) override {
    // todo: implement the die action,
    // hint:
    //   use the context.world.SetNext() to set the next state of the cell to dead
    //   use the context.position to get the current cell's position
    // at the position invert its alive state
    context.world.SetNext(context.position, !context.isAlive);
  }
};

class BornAction : public Action {
public:
  void Execute(const AgentContext& context) override {
    // see hints in DieAction
    // same as die action
    context.world.SetNext(context.position, !context.isAlive);
  }
};

class StayAliveAction : public Action {
public:
  void Execute(const AgentContext& context) override {
    // see hints in DieAction
    // at the position keep its alive state
    context.world.SetNext(context.position, context.isAlive);
  }
};

class StayDeadAction : public Action {
public:
  void Execute(const AgentContext& context) override {
    // see hints in DieAction
    // same as stayalive action
    context.world.SetNext(context.position, context.isAlive);
  }
};
}  // namespace conway

// end solution

JohnConway::JohnConway() {
  using namespace conway;

  alive = std::make_shared<State>("Alive");
  dead = std::make_shared<State>("Dead");

  const auto die = std::make_shared<DieAction>();
  const auto born = std::make_shared<BornAction>();

  // todo: add transitions and actions for alive, dead. example:
  //   alive->AddTransition(std::make_shared<Underpopulation>(), dead, {die});
  //   dead->AddAction(std::make_shared<StayDeadAction>());

  alive->AddTransition(std::make_shared<Underpopulation>(), dead, {die});
  alive->AddTransition(std::make_shared<Overpopulation>(), dead, {die});
  dead->AddTransition(std::make_shared<Reproduction>(), alive, {born});
  dead->AddAction(std::make_shared<StayDeadAction>());
  alive->AddAction(std::make_shared<StayAliveAction>());

  // begin solution
  // note: log instead of throw - the constructor runs at app startup and at
  // every fixture load; throwing here would kill the process before it runs.

  // end solution
}

// Reference: https://playgameoflife.com/info
void JohnConway::Step(World& world) {
  // relevant functions:
  //   world.Height() and world.Width() to get the world dimensions,
  //   world.Get() reads the CURRENT generation, world.SetNext() writes the NEXT one
  // Build one context per cell and let the machine decide: conditions read the
  // current generation through the context, actions write the next one.
  //
  // note: the double buffering does NOT happen here. Your actions only write
  // the next buffer via SetNext; whoever drives the simulation (the demo app's
  // Manager::step or the life-tests runner) calls world.SwapBuffers() right
  // AFTER this function returns. Never call SwapBuffers from inside a rule.
  // begin solution
  for (int y = 0; y < world.Height(); ++y) {
    for (int x = 0; x < world.Width(); ++x) {
      AgentContext context{world, {x, y}, world.Get({x, y}), CountNeighbors(world, {x, y})};
      machine.SetCurrent(context.isAlive ? alive : dead);
      machine.Update(context);
    }
  }
  // end solution
}

int JohnConway::CountNeighbors(World& world, Point2D point) {
  // todo: count the ALIVE neighbors of the cell at point, on the square grid
  // hint:
  //   a square cell has 8 neighbors, one per dx/dy in {-1, 0, 1}, excluding itself
  //   world.Get({point.x + dx, point.y + dy}) wraps around the borders (toroidal)
  // begin solution

    int neighbors = 0;
    // go through the 3 positions that x could be
    for (int dx = -1; dx < 2; dx++)
    {
        // go through the 3 positions that y could be
        for (int dy = -1; dy < 2; dy++)
        {
            // make sure its not the center cell
            if (dx != 0 || dy != 0)
            {
                // get if the neighbor is alive
                if (world.Get({point.x + dx, point.y + dy}))
                {
                    // increase neighbors if alive
                    neighbors++;
                }
            }
        }
    }
    
    return neighbors;

  // end solution
}
