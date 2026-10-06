#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(CatWorld* world) {
	Point2D next = {};
	std::vector<Point2D> path = Agent::generatePath(world);
	if (!path.empty()) next = path[0];
	
	
	
	return next;
}
