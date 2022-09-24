
#include <calmar/calmar.hpp>

int main(void) {
    calmar::application application = calmar::application(calmar::windowProperties(
        1280, 720, "Calmar Editor", calmar::windowingBackend::GLFW, calmar::renderingBackend::OPENGL));

    application.run();

    return 0;
}