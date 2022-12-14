#pragma once

#include "defines.hpp"
#include "window.hpp"
#include "application_attachment.hpp"

#include "calmar/event_system/event_dispatcher.hpp"

#include "calmar/renderer/renderer.hpp"
#include "calmar/renderer/rendering_backend.hpp"
#include "calmar/renderer/batch_renderer_2d.hpp"
#include "calmar/ecs/ecs.hpp"
#include "calmar/ecs/components.hpp"

#include "calmar/imgui/imgui_handler.hpp"

#include "../../editor/src/editor.hpp"


namespace calmar {

    /*  This class defines the entry point and program loop of the engine.
        It is desinged around the singelton pattern and can not be instantiated
        twice. The also contains gernal instances of classes which are needed for
        varios systems in the engines runtime. The class also keeps track of the applications
        window and handles functionaly of it like rezise events and swaping its buffers.
    */
    class renderSystem;
    class application {
       public:
        /// @brief Instantiates the application and subsystems like the renderer and more.
        /// @param windowProps The struct that defines properties of the window like its width and height
        /// and also its rendering backend and more.
        application(const windowProperties& windowProps);

        /* Deinitalizes varios systems with RAII and closes the application window */
        ~application();

        /// @brief The application loop in which the program runs. The application windows is updated here
        /// and the entire state of the engine are being updated in this method. This is the main method that
        /// runs when the engine is running.
        void run();

        /// @brief This method adds an attachment to the application for the application to start using,
        /// handling and updating it
        /// @param attachment The attachment to push to the attachment stack
        void addAttachment(applicationAttachment* attachment);

        /// @brief This method removes an attachments from the application and the application stops
        /// using it in any way.
        /// @param attachment The attachment to remove from the attachment stack
        void removeAttachment(applicationAttachment* attachment);

        /// @brief Closes the application by setting the "running" member variable to false and with that exits out
        /// of the main running loop.
        inline void close() {
            mRunning = false;
        }

        /// @brief This method returns the static instance of the application. In summary, this method makes the
        /// entire class and and all of its member staticly accessable.
        /// @return The instance of the application that is assigned after construction.
        static inline application* getInstance() {
            return mInstance;
        }

        inline float getDeltaTime() const {
            return mDeltaTime;
        }

        inline calmarEd::editorAttachment* getEditorAttachment() {
            // attachments at index 1 is reserved for the editor attachment
            return static_cast<calmarEd::editorAttachment*>(mAttachements[1]);
        }
        inline imGuiHandler* getImGuiHandler() {
            return mImGuiHandler;
        }
        /// @brief The universal instance of the event dispatcher to dispatch evnts in the engine
        eventDispatcher evDispatcher;

        /// @brief The universal instance of the renderer class
        renderer appRenderer;

        /// @brief This variable is used to universally define
        /// which rendering backend the application uses
        renderingBackend renderBackend;

        /// @brief The shared pointer window instance which represents the
        /// window in which the engine runs in.
        std::shared_ptr<window> display;

        ecs entityComponentSystem;

       private:
        /// @brief This method handles general events which are specific to the application like window resizing
        /// and handeling closing the window
        /// @param ev The event which is internally recived.
        void handleEvents(event ev);

        /// @brief This method sets up the display for the application
        /// @param windowProps The properties of the display
        void setupDisplay(const windowProperties& windowProps);

        /// @brief This method initializes all of the subsystems of the engine
        void initSubsystems(const windowProperties& windowProps);

       private:
        bool mRunning = false;

        float mDeltaTime = 0.0f;
        float mFps = 0.0f;

        /// @brief Instance of the application to handle singelton behaviour.
        static application* mInstance;

        std::vector<applicationAttachment*> mAttachements;

        imGuiHandler* mImGuiHandler;
    };
}  // namespace calmar

#define USING_COMPATABLE_RENDERING_API calmar::application::getInstance()->renderBackend == calmar::renderingBackend::OPENGL

#define ECS application::getInstance()->entityComponentSystem