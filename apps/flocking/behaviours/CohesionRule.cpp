#include "CohesionRule.h"
#include "imgui.h"
#include <glm/glm.hpp>

glm::vec2 CohesionRule::computeForce(const std::vector<BoidView>& boids, int selfIndex) {
  glm::vec2 cohesionForce(0.f);

  // glm::length(vec) returns the length of a vector,
  // glm::normalize(vec) returns the normalized vector (length 1) in the same direction as vec.
  // boids contains every boid, including this one (boids[selfIndex]);
  // use radius to scale the force so it is stronger when the boid is far from the center of mass
  // the force magnitude should be between 0 and 1. 1 is when the CM is at the edge of the radius, and 0 is when it is at the center of mass. 
  // Bonus: use spatial hashing to avoid O(n^2) complexity. Implement that on World.

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

bool CohesionRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;
  if (ImGui::DragFloat("Detection Radius", &radius, 1.f, 0.f, 500.f)) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}

void CohesionRule::drawRadius(const BoidView& boid, ImDrawList* dl) const {
  dl->AddCircle({boid.position.x, boid.position.y}, radius,
                IM_COL32(static_cast<int>(debugColor.r * 255), static_cast<int>(debugColor.g * 255), static_cast<int>(debugColor.b * 255), 64), 32);
}
