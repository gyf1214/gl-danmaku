#include "ext.hpp"
#include "object_box.hpp"

int main() {
    Scene *scene1 = ObjectBox::scene(Script::scene1());
    Application::setScene(scene1);
    Application::run();
}
