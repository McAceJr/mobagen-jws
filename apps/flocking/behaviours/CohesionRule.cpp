#include "CohesionRule.h"
#include <glm/glm.hpp>

glm::vec2 CohesionRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 cohesionForce(0.f);

  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.

  // begin solution

  
  // if there are no neighbors return no force.
  if (neighborhood.size() == 0) return cohesionForce;

  // create an average position
  glm::vec2 avg(0.f);

  // add the position of all neighbors to the average position
  for (auto n : neighborhood)
  {
    avg += n.position;
  }
  
  // devide the average position by the amount of neighbors
  avg /= neighborhood.size();

  // if the length of the vector from the boids position to the average position is closeto 0 then return an empty force
  if (glm::length(avg - boid.position) <= 0.0001f) return cohesionForce;

  // set the force to the normalized vector from the boids position to the average position.
  cohesionForce = glm::normalize(avg - boid.position);

  // end solution

  return cohesionForce;
}
