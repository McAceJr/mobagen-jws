#include "Cat.h"
#include "World.h"
#include <stdexcept>

Point2D Cat::Move(CatWorld* world) {
	Point2D next = {};
	std::vector<Point2D> path = Agent::generatePath(world);
	if (!path.empty()) next = path[path.size() - 1];



	return next;
}
