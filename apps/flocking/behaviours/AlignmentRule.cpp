#include "AlignmentRule.h"
#include "imgui.h"
#include <glm/glm.hpp>

glm::vec2 AlignmentRule::computeForce(const std::vector<BoidView>& boids, int selfIndex) {
  glm::vec2 averageVelocity(0.f);
  // glm::vec2 can be divided by a float, which will divide each component of the vector by that float.
  // boids contains every boid, including this one (boids[selfIndex]).
  // Bonus: use spatial hashing to avoid O(n^2) complexity. Implement that on World.

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

bool AlignmentRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;
  if (ImGui::DragFloat("Detection Radius", &radius, 1.f, 0.f, 500.f)) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}

void AlignmentRule::drawRadius(const BoidView& boid, ImDrawList* dl) const {
  dl->AddCircle({boid.position.x, boid.position.y}, radius,
                IM_COL32(static_cast<int>(debugColor.r * 255), static_cast<int>(debugColor.g * 255), static_cast<int>(debugColor.b * 255), 64), 32);
}
