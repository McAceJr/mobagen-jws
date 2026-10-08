#include "Agent.h"
#include <climits>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "World.h"

using namespace std;

std::vector<Point2D> Agent::generatePath(CatWorld* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  priority_queue<
  pair<float, Point2D>,
  vector<std::pair<float, Point2D>>,
  greater<std::pair<float, Point2D>>> frontier;                   // to store next ones to visit
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results
  unordered_map<Point2D, int> exitPoints;

  // bootstrap state
  auto catPos = w->getCat();
  frontier.push(MakeHeuristic(w, catPos));
  frontierSet.insert(catPos);
  Point2D borderExit = {INT32_MAX, INT32_MAX};  // sentinel: no border found yet
  int exitDist = 0;

  while (!frontier.empty()) {
    // get the current from frontier
    // remove the current from frontierset
    // mark current as visited
    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    // iterate over the neighs:
    // for every neighbor set the cameFrom
    // enqueue the neighbors to frontier and frontierset
    // do this up to find a visitable border and break the loop

    pair<int, Point2D> cur = frontier.top();
    frontier.pop();

    auto it = frontierSet.find(cur.second);
    if (it != frontierSet.end())
    {
        frontierSet.erase(it);
    }
    visited[cur.second] = true;

    auto neighbors = getVisitableNeighbors(w, cur.second, frontierSet, visited);
    for (auto v : neighbors)
    {
        cameFrom[v] = cur.second;
        frontier.push(MakeHeuristic(w, v));
        frontierSet.emplace(v);
    }

    if (w->catWinsOnSpace(cur.second))
    {
        int dist = 0;
        Point2D cursor = cur.second;
        while (cursor != catPos)
        {
            cursor = cameFrom[cursor];
            dist++;
        }
        exitPoints.insert({cur.second, dist});
        if (getNumVisNeighbors(w, cur.second) > 2)
        {
            exitDist = dist;
            borderExit = cur.second;
            break;
        }
    }

  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move,
  // and the last element is the cat move

  vector<Point2D> path;
  Point2D altExit = {};
  Point2D cur = {};
  int dist = w->getWorldSideSize() * 2;

  if (exitPoints.empty()) return path;

  for (auto p : exitPoints)
  {
      if (p.second < dist)
      {
          dist = p.second;
          altExit = p.first;
      }
  }

  if (w->isValidPosition(borderExit))
      cur = borderExit;
  else
      cur = altExit;

  if (dist < 3)
      cur = altExit;

  while (cur != catPos)
  {
      path.push_back(cur);
      cur = cameFrom[cur];
  }
  
  return path;
}

std::vector<Point2D> Agent::generateFilledPath(CatWorld* w) {
  unordered_map<Point2D, Point2D> cameFrom;  // to build the flowfield and build the path
  queue<Point2D> frontier;                   // to store next ones to visit
  unordered_set<Point2D> frontierSet;        // OPTIMIZATION to check faster if a point is in the queue
  unordered_map<Point2D, bool> visited;      // use .at() to get data, if the element dont exist [] will give you wrong results

  // bootstrap state
  auto catPos = w->getCat();
  frontier.push(catPos);
  frontierSet.insert(catPos);
  Point2D borderExit = {INT32_MAX, INT32_MAX};  // sentinel: no border found yet

  while (!frontier.empty()) {
    // get the current from frontier
    // remove the current from frontierset
    // mark current as visited
    // getVisitableNeightbors(world, current) returns a vector of neighbors that are not visited, not cat, not block, not in the queue
    // iterate over the neighs:
    // for every neighbor set the cameFrom
    // enqueue the neighbors to frontier and frontierset
    // do this up to find a visitable border and break the loop

    Point2D cur = frontier.front();
    frontier.pop();

    auto it = frontierSet.find(cur);
    if (it != frontierSet.end()) {
      frontierSet.erase(it);
    }
    visited[cur] = true;

    auto neighbors = getVisitableNeighbors(w, cur, frontierSet, visited);
    for (auto v : neighbors) {
      cameFrom[v] = cur;
      frontier.emplace(v);
      frontierSet.emplace(v);
    }

    if (w->catWinsOnSpace(cur)) {
      borderExit = cur;
      break;
    }
  }

  // if the border is not infinity, build the path from border to the cat using the camefrom map
  // if there isnt a reachable border, just return empty vector
  // if your vector is filled from the border to the cat, the first element is the catcher move,
  // and the last element is the cat move

  vector<Point2D> path;

  if (!w->isValidPosition(borderExit)) return path;
  
  Point2D cur = borderExit;

  while (cur != catPos) {
    path.push_back(cur);
    cur = cameFrom[cur];
  }

  return path;
}

std::vector<Point2D> Agent::getVisitableNeighbors(CatWorld* w, Point2D cur, std::unordered_set<Point2D> fSet, std::unordered_map<Point2D, bool> visited)
{ 
    vector<Point2D> directions;
    vector<Point2D> neighbors;

    directions.push_back(w->SE(cur));
    directions.push_back(w->SW(cur));
    directions.push_back(w->W(cur));
    directions.push_back(w->NW(cur));
    directions.push_back(w->NE(cur));
    directions.push_back(w->E(cur));

    for (auto v : directions)
    {
        if ( w->isValidPosition(v) && 
            !w->getContent(v)      && 
            !visited[v]            && 
            !fSet.contains(v)      && 
            w->getCat() != v         )
        {
            neighbors.push_back(v);
        }
    }

    return neighbors;
}

int Agent::getNumVisNeighbors(CatWorld* w, Point2D cur) {
  vector<Point2D> directions;
  int neighbors = 0;

  directions.push_back(w->SE(cur));
  directions.push_back(w->SW(cur));
  directions.push_back(w->W(cur));
  directions.push_back(w->NW(cur));
  directions.push_back(w->NE(cur));
  directions.push_back(w->E(cur));

  for (auto v : directions)
  {
      if (w->isValidPosition(v) && !w->getContent(v))
      {
          neighbors++;
      }
  }

  return neighbors;
}

std::pair<int, Point2D> Agent::MakeHeuristic(CatWorld *w, Point2D p)
{
    float size = w->getWorldSideSize() / 2;
    int dist = min((int)(size - abs(p.x)), (int)(size - abs(p.y)));

    return {dist, p};

}