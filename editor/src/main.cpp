
#include <calmar/calmar.hpp>

int main(void) {
    calmar::application application = calmar::application(calmar::windowProperties(
        1280, 720, "Calmar Editor", calmar::windowingBackend::WINDOWS, calmar::renderingBackend::OPENGL));

    application.run();

    return 0;
}