#include "AlignmentRule.h"
#include <glm/glm.hpp>

glm::vec2 AlignmentRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 averageVelocity(0.f);
  // glm::vec2 can be divided by a float, which will divide each component of the vector by that float.

  // begin solution

  // if there are no neighbors dont add any force
  if (neighborhood.size() == 0) return averageVelocity;

  // for each neighbor add the velocity of the neighbors to the average velocity
  for (auto n : neighborhood)
  {
	  averageVelocity += n.velocity;
  }

  // devide the average velocity by the neighborhood size
  averageVelocity /= neighborhood.size();
  
  // end solution

  return averageVelocity;
}
