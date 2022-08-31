#ifndef CALMAR_WINDOW_INCLUDED
#define CALMAR_WINDOW_INCLUDED

#include <memory>

#include "defines.hpp"

namespace calmar {
    enum class windowingBackend {
        GLFW,
        SDL2,
        WINDOWS
    };
    struct CALMAR_API windowProperties {
        windowProperties() {}

        windowProperties(u32 _width, u32 _height, const char* _title, windowingBackend _backend, bool _vsync = true, bool _resizable = true)
            : width(_width), height(_height), title(_title), backened(_backend), vsync(_vsync), resizable(_resizable) {}

        u32 width = 0, height = 0;
        const char* title = "";
        windowingBackend backened;
        bool vsync = false;
        bool resizable = false;
    };
    class CALMAR_API window {
       public:
        static window create(const windowProperties& props);

        static std::shared_ptr<window> createRef(const windowProperties& props);

        static std::unique_ptr<window> createScoped(const windowProperties& props);

        virtual ~window() {}

        virtual void update() {}

        virtual void* getBackendHandle() const { return nullptr; }

        const windowProperties& getProperties() const;

        virtual void setWidth(u32 width) {}

        virtual void setHeight(u32 height) {}

        virtual void setTitle(const char* title) {}

        virtual void setVsync(bool vsync) {}

        virtual void setResizable(bool resizable) {}

        virtual bool closeRequested() const { return false; }

       protected:
        windowProperties mProps;

       protected:
        virtual void initBackend() {}

        virtual void shutdownBackend() {}
    };

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