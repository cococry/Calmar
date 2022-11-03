#include "pch.h"
#include <calmar/calmar.hpp>

#include "editor.hpp"

int main(void) {
    calmar::application application = calmar::application(calmar::windowProperties(
        1280, 720, "Calmar Editor", calmar::windowingBackend::GLFW, calmar::renderingBackend::OPENGL));

    application.addAttachment(new calmarEd::editorAttachment());

    application.run();

    return 0;
}