#pragma once

#include "event.hpp"

namespace calmar {
    /*  This class is as a subclass of the class "event". It is intended to be instantiated
        when a mouse button was pressed. It takes in the code of the pressed mouse button in
        the constructor and assignes the private member variable "Button" to the input.*/
    class mousePressedEvent : public event {
       public:
        /// @brief This constrcutor assinges the private member variable "Button" to the
        /// given "button" parameter input. The "Button" member varaible defines the button code
        /// of the pressed button.
        /// @param button The code of the button that was pressed
        mousePressedEvent(u32 button)
            : mButton(button) {
        }

        /// @brief This constant member function returns the code of the button that was pressed.
        /// Which is the member variable "Button" and is assigned at construction.
        /// @return Returns the member variable "Button" (const)
        inline u32 getButton() const {
            return mButton;
        }

        /// @brief This member function is overwritten from the subclass "event" and returns
        /// the type of the event ("mousePressedEvent")
        /// @return Returns the member variable evType (constexpr "mousePressedEvent")
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mousePressedEvent";

       private:
        u32 mButton;
    };

    /*  This class is as a subclass of the class "event". It is intended to be instantiated
        when a mouse button was released. It takes in the code of the pressed mouse button in
        the constructor and assignes the private member variable "Button" to the input.*/
    class mouseReleasedEvent : public event {
       public:
        /// @brief This constrcutor assinges the private member variable "Button" to the
        /// given "button" parameter input. The "Button" member varaible defines the button code
        /// of the released button.
        /// @param button The code of the button that was released
        mouseReleasedEvent(u32 button)
            : mButton(button) {
        }

        /// @brief This constant member function returns the code of the button that was released.
        /// Which is the member variable "Button" and is assigned at construction.
        /// @return Returns the member variable "Button" (const)
        inline u32 getButton() const {
            return mButton;
        }

        /// @brief This member function is overwritten from the subclass "event" and returns
        /// the type of the event ("mouseReleasedEvent")
        /// @return Returns the member variable evType (constexpr "mouseReleasedEvent")
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mouseReleasedEvent";

       private:
        u32 mButton;
    };

    /* This class is a subclass of the class "event". It is intended to be instantiated
        when the mouse cursor has been moved. It takes in the x and y position of the mouse cursor
        and stores both variables.*/
    class mouseMovedEvent : public event {
       public:
        /// @brief This constructor takes in the x and y position of the mouse cursor and
        /// assignes the member variables "Xpos" and "Ypos" to the corresponding input.*/
        /// @param xpos The x position of the cursor
        /// @param ypos The y position of the cursor
        mouseMovedEvent(u32 xpos, u32 ypos)
            : mXpos(xpos),
              mYpos(ypos) {
        }

        /// @brief This constant member function returns the member variable "Xpos" which
        /// was assigned at construction
        /// @return Returns the member variable "Xpos" (const)
        inline u32 getX() const {
            return mXpos;
        }

        /// @brief This constant member function returns the member variable "Ypos" which
        /// was assigned at construction
        /// @return Returns the member variable "Ypos" (const)
        inline u32 getY() const {
            return mYpos;
        }

        /// @brief This member function is overwritten from the subclass "event" and returns
        /// the type of the event ("mouseMovedEvent")
        /// @return Returns the member variable evType (constexpr "mouseMovedEvent")
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mouseMovedEvent";

       private:
        u32 mXpos, mYpos;
    };

    /* This class is a subclass of the class "event". It is intended to be instantiated if
        the scroll wheel of the mouse has changed. It takes in the x and y offset of the scrollwheel
        and stores the input.*/
    class mouseScrolledEvent : public event {
       public:
        /// @brief This constructor takes in the x and y offset of the scroll wheel
        /// and assinges the member variables "Xoffset" and "Yoffset" to the corresponding input
        /// @param xoffset The x offset of the scrollwheel
        /// @param yoffset The y offset of the scrollwheel
        mouseScrolledEvent(u32 xoffset, u32 yoffset)
            : mXoffset(xoffset), mYoffset(yoffset) {
        }

        /// @brief This constant member function returns the private member variable
        /// "Xoffset".
        /// @return Returns the member variable "Xoffset" (const)
        inline u32 getXOffset() const {
            return mXoffset;
        }

        /// @brief This constant member function returns the private member variable
        /// "Yoffset".
        /// @return Returns the member variable "Yoffset" (const)
        inline u32 getYOffset() const {
            return mYoffset;
        }

        /// @brief This member function is overwritten from the subclass "event" and returns
        /// the type of the event ("mouseScrolledEvent")
        /// @return Returns the member variable evType (constexpr "mouseScrolledEvent")
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "mouseScrolledEvent";

       private:
        u32 mXoffset, mYoffset;
    };
}  // namespace calmar
