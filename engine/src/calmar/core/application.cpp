#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

#include "calmar/event_system/window_events.hpp"

#include "calmar/input/input.hpp"

#include "calmar/renderer/render_command.hpp"

calmar::application* calmar::application::mInstance = nullptr;

namespace calmar {
    application::application(const windowProperties& windowProps) {
        logging::init();

        /* Singelton setup */
        CALMAR_ASSERT_MSG(mInstance == nullptr, "Tried to instantiate application more than once.");

        mInstance = this;
        mRunning = true;

        renderBackend = windowProps.renderBackend;

        // Creating the windowing context
        mWindow = window::createScoped(windowProps);

        /* Listening to events to be handled in the "handleEvents()" method. */
        evDispatcher.listen(windowCloseEvent::evType, EVENT_CALLBACK(application::handleEvents));

        evDispatcher.listen(windowResizeEvent::evType, EVENT_CALLBACK(application::handleEvents));

        /* Initializing backend specifc subsystems */
        input::init(windowProps.backened);

        appRenderer.initSubsystems(windowProps.renderBackend);

        /* TODO: Temporary */

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f};

        u32 indices[] = {0, 1, 2, 2, 3, 0};

        mVertexArray = vertexArray::createRef(3);

        std::shared_ptr<vertexBuffer> vb = vertexBuffer::createRef(vertices, sizeof(vertices));

        mVertexArray->addVertexBuffer(vb);

        std::shared_ptr<indexBuffer> ib = indexBuffer::createRef(indices, sizeof(indices) / sizeof(u32));

        mVertexArray->setIndexBuffer(ib);

        mVertexArray->setVertexLayoutAttribute(3);

        mShader = shader::createRef("../../../../editor/assets/shaders/default_vertex.glsl", "../../../../editor/assets/shaders/default_fragment.glsl");
    }

    application::~application() {
        // Breaking out of the application loop by calling "close()"
        close();
    }

    void application::run() {
        /* Main application loop */
        while (mRunning) {
            // Breaking out of the loop if the window is closed
            if (mWindow->closeRequested()) {
                close();
            }

            /* Temporary testing code */
            renderCommand::clearBuffers(clearBuffers::colorBuffer);
            renderCommand::clearColor({0.2f, 0.3f, 0.8f, 1.0f});

            mShader->bind();
            renderCommand::drawIndexed(mVertexArray);
            mShader->unbind();

            /* Updating subsystems */
            mWindow->update();

            input::update();
        }
    }

    void application::handleEvents(const event& ev) {
        // Checking the type of the event and handling it
        if (COMPARE_EVENTS(ev, windowCloseEvent)) {
            // Closing the window if a close event was recived
            mRunning = false;
        }
        if (COMPARE_EVENTS(ev, windowResizeEvent)) {
            // Setting the rendering APIs viewport to the new size of the window
            // if a resize event was recived
            const windowResizeEvent& resizeEvent = static_cast<const windowResizeEvent&>(ev);
            renderCommand::setViewport(resizeEvent.getWidth(), resizeEvent.getHeight());
        }
    }

}  // namespace calmar