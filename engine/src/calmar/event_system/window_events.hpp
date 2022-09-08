#pragma once

#include "calmar/core/defines.hpp"

#include "event.hpp"

namespace calmar {
    /* This class is a subclass of the class "event". It is intended to be instantiated
        if the window has been resized */
    class windowResizeEvent : public event {
       public:
        windowResizeEvent(u32 width, u32 height)
            : mWidth(width), mHeight(height) {
        }

        virtual eventType type() const override {
            return evType;
        }

        inline u32 getWidth() const {
            return mWidth;
        }

        inline u32 getHeight() const {
            return mHeight;
        }

        static constexpr eventType evType = "windowResizeEvent";

       private:
        u32 mWidth, mHeight;
    };

    class windowCloseEvent : public event {
       public:
        windowCloseEvent() {
        }

        virtual eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "windowCloseEvent";

       private:
    };
}  // namespace calmar
