#include "application.hpp"

#include "logging.hpp"
#include "asserting.hpp"

#include "calmar/event_system/window_events.hpp"

#include "calmar/input/input.hpp"

#include "calmar/renderer/render_command.hpp"

#include "calmar/input/key_codes.hpp"

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
            mWindow->update();
            mDeltaTime = mWindow->getDeltaTime();

            for (applicationAttachment* attachemnt : mAttachements) {
                attachemnt->update();
            }

            input::update();
            mWindow->stopTiming();
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