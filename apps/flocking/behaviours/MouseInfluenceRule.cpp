#include "MouseInfluenceRule.h"
#include "imgui.h"

glm::vec2 MouseInfluenceRule::computeForce(const std::vector<BoidView>& neighborhood, const BoidView& boid) {
  glm::vec2 force(0.f);

  // ImGui::IsMouseDown(ImGuiMouseButton_Left) returns true if the left mouse button is currently pressed.
  // ImGui::GetIO().MousePos returns the current mouse position as an ImVec2.
  // glm::length(vec) returns the length of a vector

  // begin solution

  if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
  {
    // mosue position vector
    glm::vec2 mouse(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
    // direction from boid to mouse
    glm::vec2 dir = boid.position - mouse;

    // get the length and normalized direction vector
    float dist = glm::length(dir);
    dir = glm::normalize(dir);
    
    // if length is close to 0 return no force
    if (dist <= 0.0001f) return force;

    // force is the direction and the magnitude becomes larger the closer the object is to the mouse.
    force = (dir * (1 / dist)) * this->weight;

    // scale with neighborhood size (self inclusive)
    force *= neighborhood.size() + 1;

    // invert the force is it is not repulsive.
    if (!isRepulsive) force = -force;
  }

  // end solution

  return force;
}

bool MouseInfluenceRule::drawImguiRuleExtra() {
  bool valueHasChanged = false;

  if (ImGui::RadioButton("Attractive", !isRepulsive)) {
    isRepulsive = false;
    valueHasChanged = true;
  }
  ImGui::SameLine();
  if (ImGui::RadioButton("Repulsive", isRepulsive)) {
    isRepulsive = true;
    valueHasChanged = true;
  }

  return valueHasChanged;
}
