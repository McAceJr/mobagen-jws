#include "SeparationRule.h"
#include "imgui.h"
#include <glm/glm.hpp>

glm::vec2 SeparationRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 separatingForce(0.f);

  // the header have the desiredMinimalDistance member variable, which is the distance that the boids should try to maintain from each other.
  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.
  // multiply by (desiredMinimalDistance / distance) is the proportionality factor that makes the force stronger when the boids are closer together, and weaker when they are farther apart.

  // begin solution

  // create direction and distance variables
  glm::vec2 dir;
  float dist;

  // for each neighbor get the direction from the boid away from the neighbor and get the length of the vector
  for (auto n : neighborhood)
  {
    dir = boid.position - n.position;
    dist = glm::length(dir);

    // if the length is greater than roughly 0 then normalize the vector and add to the seperating force the direction vector * the minimal distance / the distance.
    if (dist > 0.0001f)
    {
      dir = glm::normalize(dir);
      separatingForce += dir * (desiredMinimalDistance / dist);
    }
  }

  // end solution

  return separatingForce;
}

bool SeparationRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;
  if (ImGui::DragFloat("Desired Separation", &desiredMinimalDistance, 0.05f)) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}
