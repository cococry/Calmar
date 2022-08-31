#ifndef CALMAR_APPLICATION_INCLUDED
#define CALMAR_APPLICATION_INCLUDED

#include <memory>

#include "defines.hpp"
#include "window.hpp"

namespace calmar {

    class CALMAR_API application {
       public:
        application(const windowProperties& props);

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

       private:
        bool mRunning = false;

        static application* mInstance;

        std::shared_ptr<window> mWindow;
    };
}  // namespace calmar

#endif