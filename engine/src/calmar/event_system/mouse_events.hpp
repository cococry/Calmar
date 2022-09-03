#pragma once

#include "calmar/core/defines.hpp"

#include "event.hpp"

namespace calmar {
    class mousePressedEvent : public event {
       public:
        mousePressedEvent(u32 button)
            : mButton(button) {
        }

        inline u32 getButton() const {
            return mButton;
        }

        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mousePressedEvent";

       private:
        u32 mButton;
    };

    class mouseReleasedEvent : public event {
       public:
        mouseReleasedEvent(u32 button)
            : mButton(button) {
        }

        inline u32 getButton() const {
            return mButton;
        }

        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mouseReleasedEvent";

       private:
        u32 mButton;
    };

    class mouseMovedEvent : public event {
       public:
        mouseMovedEvent(u32 xpos, u32 ypos)
            : mXpos(xpos), mYpos(ypos) {
        }

        inline u32 getX() const {
            return mXpos;
        }

        inline u32 getY() const {
            return mYpos;
        }

        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mouseMovedEvent";

       private:
        u32 mXpos, mYpos;
    };

    class mouseScrolledEvent : public event {
       public:
        mouseScrolledEvent(u32 xoffset, u32 yoffset)
            : mXoffset(xoffset), mYoffset(yoffset) {
        }

        inline u32 getXOffset() const {
            return mXoffset;
        }

        inline u32 getYOffset() const {
            return mYoffset;
        }

        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mouseScrolledEvent";

       private:
        u32 mXoffset, mYoffset;
    };
}  // namespace calmar
