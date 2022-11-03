#ifndef CALMAR_WINDOW_INCLUDED
#define CALMAR_WINDOW_INCLUDED

#include <memory>

#include "defines.hpp"

#include "calmar/renderer/rendering_backend.hpp"

namespace calmar {
    /* An enumaration to define different kinds of backends for windowing*/
    enum class windowingBackend {
        GLFW = 0
    };

    /*A structure which contains information about the application's window like
      it's width, height and title. It also contains general information which effect the
      runtime of the application like the rendering and windowing backend in which the engine
      starts in.*/
    struct windowProperties {
        /// @brief Default constructor which does not initialize any member variables
        windowProperties() {}

        /// @brief Assings member variables to the given parameter information
        /// @param _width The width with that the window is created
        /// @param _height The height with that the window is created
        /// @param _title The title with that the window is created
        /// @param _backend The windowing backend with that the window is created
        /// @param _renderBackend The rendering backend with that the engine starts
        /// @param _xpos The x position on the screen with that the window is cretaed
        /// @param _ypos The y position on the screen with that the window is cretaed
        /// @param _vsync Sets vsync on/off in the window
        /// @param _fullscreen Defines if the window should start fullscreend (only GLFW)
        /// @param _resizable Sets resizing on/off in the window
        windowProperties(u32 _width, u32 _height, const char* _title, windowingBackend _backend, renderingBackend _renderBackend,
                         u32 _xpos = 0, u32 _ypos = 30, bool _vsync = true, bool _fullscreen = false, bool _resizable = true)
            : width(_width),
              height(_height),
              title(_title),
              backened(_backend),
              renderBackend(_renderBackend),
              xpos(_xpos),
              ypos(_ypos),
              vsync(_vsync),
              fullscreen(_fullscreen),
              resizable(_resizable) {}

        u32 width = 0, height = 0;
        const char* title = "";
        windowingBackend backened;
        renderingBackend renderBackend;
        u32 xpos = 0, ypos = 0;
        bool vsync = false;
        bool fullscreen = false;
        bool resizable = false;
    };
    class window {
       public:
        /// @brief Creates a window with the given parameters. It creates it
        /// with the given windowing backend which is a subclass f this class
        /// and returns it.
        /// @param props The window properties that are being passed to the created window
        /// @return A windowing backend specific window which is a subclass of this class
        static window create(const windowProperties& props);

        /// @brief Creates a shared pointer window with the given parameters. It creates it
        /// with the given windowing backend which is a subclass of this class
        /// and returns it.
        /// @param props The window properties that are being passed to the created window
        /// @return A windowing backend specific shared pointer window which is a subclass of this class
        static std::shared_ptr<window> createRef(const windowProperties& props);

        /// @brief Creates a unique pointer window with the given parameters. It creates it
        /// with the given windowing backend which is a subclass of this class
        /// and returns it.
        /// @param props The window properties that are being passed to the created window
        /// @return A windowing backend specific unique pointer window which is a subclass of this class
        static std::unique_ptr<window> createScoped(const windowProperties& props);

        virtual ~window() {}

        /// @brief Virtual method which is overwritten by subclasses and handles window updating like
        /// swapping the windows's buffers.
        virtual void update() {}

        /// @brief Virtual method which returns a windowing API specific handle that defines the window
        /// in the windowing API
        /// @return The windowing API specific backend handle of the window
        virtual void* getBackendHandle() const { return nullptr; }

        /// @brief Returns the properties of the window which are intialized at construction
        /// @return
        const windowProperties& getProperties() const;

        /// @brief Virtual method which is ovewritten by subclasses and sets the width of the window
        /// @param width The width to set the window's width to
        virtual void setWidth(u32 width) {}

        /// @brief Virtual method which is ovewritten by subclasses and sets the height of the window
        /// @param height The height to set the window's height to
        virtual void setHeight(u32 height) {}

        /// @brief Virtual method which is ovewritten by subclasses and sets the title of the window
        /// @param title The title to set the window's title to
        virtual void setTitle(const char* title) {}

        /// @brief Virtual method which is overwritten by subclasses and toggles vsync in the window
        /// @param vsync Defines if vsync should be turned on/off
        virtual void setVsync(bool vsync) {}

        /// @brief Virtual method which is overwritten by subclasses and toggles resizability for the window
        /// @param resizable Defines if resizability should be turned on/off
        virtual void setResizable(bool resizable) {}

        /// @brief Virtual method which is overwritten by subclasses and sets a new positon for the window
        /// @param xpos The new x position for the window
        /// @param ypos The new y position for the window
        virtual void setPosition(u32 xpos, u32 ypos) {}

        /// @brief Virtual method which is overwritten by subclasses and returns if the window should close (e.g X button)
        /// @return If the window should close
        virtual bool closeRequested() const { return false; }

        /// @brief Returns the absolute time which has passed since the initialization of the window
        /// @return
        virtual float getDeltaTime() const { return 0.0; };

        virtual void startTiming() {}
        
        virtual void stopTiming() {}

        virtual float getFps() const { return 0.0f; }

        virtual void initRenderBackend() {}

       protected:
        windowProperties mProps;

       protected:
        /// @brief Initalizes the windowing backend of subclasses
        virtual void initBackend() {}

        /// @brief Shuuts down the windowing backend of subclasses
        virtual void shutdownBackend() {}
    };

    /* A window which does not define anything and is used to prevent undefined behaviour*/
    class windowUndefined : public window {
       public:
        windowUndefined() {
            mProps = windowProperties();
        }

        ~windowUndefined() {}

        virtual void update() override {}

        virtual void* getBackendHandle() const override {
            return nullptr;
        }

       private:
    };
}  // namespace calmar

#endif  // CALMAR_WINDOW_INCLUDED