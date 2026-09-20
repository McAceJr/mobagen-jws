#include "../World.h"
#include "../SeededRandom.h"
#include "RecursiveBacktrackerExample.h"
#include <climits>

// Recursive backtracker, in FORMAL units: (0, 0) is the top-left cell, x grows
// right, y grows down. The caller seeds SeededRandom before the first Step;
// every decision consumes the seed in order, so the maze is deterministic.
//
// Procedure per Step, on the cell at the top of the path stack:
//   1. mark it visited;
//   2. list its visitable (unvisited) neighbors in clockwise order starting
//      from the top: UP, RIGHT, DOWN, LEFT (getVisitables does this);
//   3. none        -> dead end: pop the stack (backtrack). Empty stack = done;
//   4. exactly one -> move to it, do not consume a random number;
//   5. two or more -> consume SeededRandom::next() and pick
//      next() % visitableCount;
//   6. moving opens the wall between the two cells
//      (World::SetNorth/SetEast/SetSouth/SetWest with false).

void RecursiveBacktrackerExample::Clear(World* w) {
  // todo: reset the walk
  // hint:
  //   clear visited and the path stack, then start the walk at the
  //   top-left cell in formal units: stack.push_back({0, 0})
  // begin solution

	stack.clear();
	
	for (auto vis : visited)
	{
          vis.second.clear();
	}

	visited.clear();

	Point2D size = w->ToFormalCoords({w->GetWidth() - 1, w->GetHeight() - 1});

	for (int x = -size.x; x < size.x; x++)
	{
		std::map<int, bool> vis;
		for (int y = -size.y; y < size.y; y++)
		{
			vis.insert({y, false});
		}
		visited.insert({x, vis});
	}

	stack.push_back({0, 0});

  // end solution
}

bool RecursiveBacktrackerExample::Step(World* w) {
  // todo: implement one iteration of the recursive backtracker
  // hint:
  //   empty stack  -> the maze is done, return false
  //   otherwise, on the cell at the top of the stack (formal units):
  //   1. mark it visited;
  //   2. list its visitable neighbors with getVisitables
  //      (already in clockwise order: UP, RIGHT, DOWN, LEFT);
  //   3. none        -> dead end: pop the stack (backtrack);
  //   4. exactly one -> move to it, do not consume a random number;
  //   5. two or more -> consume SeededRandom::next() and pick
  //      next() % visitables.size();
  //   moving = opening the wall between the two cells, through the
  //   World coordinate translation:
  //     Point2D worldCurrent = w->ToWorldCoords(current);
  //     UP    -> w->SetNorth(worldCurrent, false)
  //     RIGHT -> w->SetEast(worldCurrent, false)
  //     DOWN  -> w->SetSouth(worldCurrent, false)
  //     LEFT  -> w->SetWest(worldCurrent, false)
  //   return true while there is still work (stack not empty after the move)
  // begin solution

	if (stack.empty()) return false;

	Point2D p = stack.back();

	visited[p.x][p.y] = true;

	std::vector<Point2D> neighbors = getVisitables(w, p);

	Color32 nodeCol;

	bool deadEnd = false;

	if (neighbors.empty())
	{
          stack.pop_back();
          nodeCol = Color32(0.5f, 0.f, 0.0f, 1.f);
          deadEnd = true;
	}
	else if (neighbors.size() == 1)
	{
          stack.push_back(neighbors.back());
          nodeCol = Color32(0.f, 0.5f, 0.f, 1.f);
	}
	else
	{
          stack.push_back(neighbors[SeededRandom::next() % (uint8_t)neighbors.size()]);
          nodeCol = Color32(0.f, 0.5f, 0.f, 1.f);
	}

	if (stack.empty()) return false;

	Point2D wp = w->ToWorldCoords(p);

	w->SetNodeColor(wp, nodeCol);
	if (!deadEnd)
	{
        if ((p.y < stack.back().y))
			w->SetNorth(wp, false);
        else if ((p.x < stack.back().x))
			w->SetEast(wp, false);
        else if ((p.y > stack.back().y))
			w->SetSouth(wp, false);
        else if ((p.x > stack.back().x))
			w->SetWest(wp, false);
	}

  // end solution
  return true;
}

std::vector<Point2D> RecursiveBacktrackerExample::getVisitables(World* w, const Point2D& formalPoint) {
  // todo: list the unvisited neighbors of formalPoint, in clockwise order
  // hint:
  //   candidates in order: UP {x, y-1}, RIGHT {x+1, y}, DOWN {x, y+1}, LEFT {x-1, y}
  //   keep a candidate only if it is inside the grid
  //   (0 <= x < w->GetWidth(), 0 <= y < w->GetHeight()) and not visited
  // begin solution

	Point2D worldPoint = w->ToWorldCoords(formalPoint);
	std::vector<Point2D> dir;
	std::vector<Point2D> neighbors = {};

	if (0 <= formalPoint.x < w->GetWidth() && 0 <= formalPoint.y < w->GetHeight())
	{
          int x = formalPoint.x;
          int y = formalPoint.y;

		  if (0 <= y - 1) dir.push_back({x, y - 1});
		  if (x + 1 < w->GetWidth()) dir.push_back({x + 1, y});
		  if (y + 1 < w->GetHeight()) dir.push_back({x, y + 1});
		  if (0 <= x - 1) dir.push_back({x - 1, y});

		  if (dir.empty()) return {};

		  for (auto d : dir)
		  {
			  if (!visited[d.x][d.y])
			  {
				  neighbors.push_back(d);
			  }
		  }

	}

  // end solution
  return neighbors;
}
