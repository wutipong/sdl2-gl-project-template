#include "scene.hpp"

#include <imgui.h>

void Scene::Init() {

}

void Scene::CleanUp() {

}

void Scene::DoFrame(SDL_Event& event) {

}

void Scene::DoUI() {
    ImGui::Begin("Hello");
    ImGui::LabelText("Hello", "Hello World");
    ImGui::End();
}