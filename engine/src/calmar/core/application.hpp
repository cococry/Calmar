#ifndef CALMAR_APPLICATION_INCLUDED
#define CALMAR_APPLICATION_INCLUDED

#include <memory>

#include "defines.hpp"
#include "window.hpp"

#include "calmar/renderer/rendering_backend.hpp"
#include "calmar/event_system/event_dispatcher.hpp"

#include "calmar/renderer/renderer.hpp"

namespace calmar {

    class CALMAR_API application {
       public:
        application(const windowProperties& windowProps);

        ~application();

        void run();

        inline void close() {
            mRunning = false;
        }

        static inline application* getInstance() {
            return mInstance;
        }

        inline const std::shared_ptr<window>& getWindow() const {
            return mWindow;
        }

        eventDispatcher evDispatcher;

        renderer appRenderer;

       private:
        void handleEvents(const event& ev);

       private:
        bool mRunning = false;

        static application* mInstance;

        std::shared_ptr<window> mWindow;
    };
}  // namespace calmar

#endif