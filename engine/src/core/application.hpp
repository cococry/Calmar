#ifndef CALMAR_APPLICATION_INCLUDED
#define CALMAR_APPLICATION_INCLUDED

#include "defines.hpp"

namespace calmar {
    class CALMAR_API application {
       public:
        application();

        ~application();

        void run();

        inline void close() {
            mRunning = false;
        }

        static inline application* getInstance() {
            return mInstance;
        }

       private:
        bool mRunning = false;

        static application* mInstance;
    };
}  // namespace calmar

#endif