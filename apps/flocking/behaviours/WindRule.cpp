#include "WindRule.h"
#include "imgui.h"
#include <cmath>

glm::vec2 WindRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  // std::cos and std::sin return the cosine and sine of an angle in radians, respectively.
  // windAngle is the angle of the wind direction in degrees, so we need to convert it to radians by multiplying it by (pi / 180).
  glm::vec2 windForce(0.f);
  // begin solution


  // for some reason the using the glm radians function doesn't work as well as just using the angle does.
  glm::vec2 dir(sin(windAngle), cos(windAngle));

  // the force is the angle times the weight.
  windForce = dir * this->weight;
  
  // end solution
  return windForce;
}

bool WindRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;
  if (ImGui::SliderAngle("Wind Direction", &windAngle, 0)) {
    valueHasChanged = true;
  }
  return valueHasChanged;
}
