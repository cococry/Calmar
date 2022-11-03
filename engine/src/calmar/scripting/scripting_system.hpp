#pragma once

namespace calmar {
    class scriptingSystem {
       public:
        static void init();
        static void shutdown();

       private:
        static void initMono();
    };
}  // namespace calmar
