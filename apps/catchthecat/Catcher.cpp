#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(CatWorld* world) {
	Point2D next = {};
	std::vector<Point2D> path = Agent::generateFilledPath(world);
	int size = world->getWorldSideSize() / 2;

	if (size > 5) // remove corners first if the grid is large enough
	{
        if (!world->getContent({size, size}))
			return {size, size};
        else if (!world->getContent({size, -size}))
			return {size, -size};
        else if (!world->getContent({-size + 1, -size}))
			return {-size + 1, -size};
        else if (!world->getContent({-size + 1, size}))
			return {-size + 1, size};
	}

	if (!path.empty())
	{
		int index = 0;
		if (path.size() > 3)
		{
			index = path.size() - size / 10;
		}
		next = path[index];
	}
	
	return next;
}
