#pragma once

#include "calmar/core/defines.hpp"

#include "event.hpp"

namespace calmar {

    /*  This is a subclass of the "event" class. It contains a single
        member variable which defines the keycode of the key that is recived.
        This class is intended to be overwritten by subclasses (e.g keyReleasedEvent) and
        should not be instantiated.*/
    class keyEvent : public event {
       public:
        /// @brief This constructor simply assignes the member variable "Key" to the given "key"
        /// parameter
        /// @param key The keycode which is recived by the event system
        keyEvent(u32 key)
            : mKey(key) {
        }

        /// @brief This constant member function returns the member variable "Key" which defines the keycode of the recived key
        /// which was assinged at construction
        /// @return Returns the "Key" member variable (const)
        inline u32 getKey() const {
            return mKey;
        }

        /// @brief This member function is overwritten from the subclass "event" and returns
        /// the type of the event ("keyEvent")
        /// @return Returns the member variable evType (constexpr "keyEvent")
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "keyEvent";

       protected:
        u32 mKey;
    };

    /*  This is a subclass of the "keyEvent" class. It is intended to be instantiated when
        a key was pressed. It inherents the keycode data of
        it's base class and in summary, it is the same class as the "keyEvent" class just that
        the event type string is modified ("keyPressedEvent")*/
    class keyPressedEvent : public keyEvent {
       public:
        /// @brief This constrcutor constructs the base class "keyEvent" and passes the
        /// given parameter "key" to it.
        /// @param key The keycode of the key that was pressed
        keyPressedEvent(u32 key)
            : keyEvent(key) {
        }

        /// @brief This member function is overwritten from the subclass "event" and returns
        /// the type of the event ("keyPressedEvent")
        /// @return Returns the member variable evType (constexpr "keyPressedEvent")
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "keyPressedEvent";

       private:
    };

    /*  This is a subclass of the "keyEvent" class. It is intended to be instantiated when
        a key was released. It inherents the keycode data of
        it's base class and in summary, it is the same class as the "keyEvent" class just that
        the event type string is modified ("keyReleasedEvent")*/
    class keyReleasedEvent : public keyEvent {
       public:
        keyReleasedEvent(u32 key)
            : keyEvent(key) {
        }

        /// @brief This member function is overwritten from the subclass "event" and returns
        /// the type of the event ("keyReleasedEvent")
        /// @return Returns the member variable evType (constexpr "keyReleasedEvent")
        inline eventType type() const override {
            return evType;
        }

        static constexpr eventType evType = "keyReleasedEvent";

       private:
    };

}  // namespace calmar
