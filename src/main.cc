#include "ext.hpp"
#include "scene_box.hpp"

int main() {
    Scene *scene1 = SceneBox::scene1();
    Application::setScene(scene1);
    Application::run();
}
