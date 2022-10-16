#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

#include "calmar/event_system/window_events.hpp"

#include "calmar/input/input.hpp"
#include "calmar/input/key_codes.hpp"

#include "calmar/renderer/render_command.hpp"

#include "calmar/ecs/ecs.hpp"
#include "calmar/ecs/components.hpp"
#include "calmar/ecs/scene.hpp"

#include "calmar/platform/opengl/gl_rendering.hpp"


#include <sstream>

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
        display = window::createScoped(windowProps);
        display->initRenderBackend();
        std::string windowTitleStr = display->getProperties().title;
        std::string windowTitle = windowTitleStr + " x64";
        switch (display->getProperties().backened) {
            case windowingBackend::GLFW:
                windowTitle += " GLFW Windowing,";
                break;
            default:
                break;
        };
        if (display->getProperties().renderBackend == renderingBackend::NONE) {
            windowTitle += " No Rendering Backend, ";
        } else if (display->getProperties().renderBackend == renderingBackend::OPENGL) {
            const char* glVers = reinterpret_cast<const char*>(glRendering::getVersion());
            std::string glVersStr = glVers;
            windowTitle += " OpenGL " + glVersStr + " Rendering Backend, ";
        } else if (display->getProperties().renderBackend == renderingBackend::VULKAN) {
            windowTitle += " Vulkan Rendering Backend, ";
        } else if (display->getProperties().renderBackend == renderingBackend::DIRECT3D) {
            windowTitle += " Direct3D Rendering Backend, ";
        }
        std::string calmarVersionStr = engineVersion;
        windowTitle += " Calmar Version " + calmarVersionStr;

        display->setTitle(windowTitle.c_str());

        /* Listening to events to be handled in the "handleEvents()" method. */
        evDispatcher.listen(windowCloseEvent::evType, EVENT_CALLBACK(application::handleEvents));

        evDispatcher.listen(windowResizeEvent::evType, EVENT_CALLBACK(application::handleEvents));

        /* Initializing backend specifc subsystems */
        input::init(windowProps.backened);

        appRenderer.initSubsystems(windowProps.renderBackend);

        entityComponentSystem.init();

        entityComponentSystem.registerComponent<transformComponent>();
        entityComponentSystem.registerComponent<spriteRendererComponent>();
        entityComponentSystem.registerComponent<tagComponent>();

        mImGuiHandler = new imGuiHandler();
        addAttachment(mImGuiHandler);
    }

    application::~application() {
        // Breaking out of the application loop by calling "close()"
        close();

        for (applicationAttachment* attachment : mAttachements) {
            attachment->shutdown();
        }
        delete mImGuiHandler;
    }

    void application::run() {
        /* Main application loop */
        while (mRunning) {
            // Breaking out of the loop if the window is closed
            if (display->closeRequested()) {
                close();
            }
            for (applicationAttachment* attachment : mAttachements) {
                attachment->update();
            }

            mImGuiHandler->beginImGui();
            for (applicationAttachment* attachment : mAttachements) {
                attachment->renderImGui();
            }
            mImGuiHandler->endImGui();

            input::update();
            display->stopTiming();
            mFps = display->getFps();
            mDeltaTime = display->getDeltaTime();
            
            display->update();

        }
    }

    void application::addAttachment(applicationAttachment* attachment) {
        mAttachements.push_back(attachment);
        attachment->init();
    }

    void application::removeAttachment(applicationAttachment* attachment) {
        auto iterator = std::find(mAttachements.begin(), mAttachements.end(), attachment);

        attachment->shutdown();
        if (iterator != mAttachements.end()) {
            mAttachements.erase(iterator);
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
        for (applicationAttachment* attachemnt : mAttachements) {
            attachemnt->handleEvents(ev);
        }
    }

}  // namespace calmar