#pragma once

#include "rendering_backend.hpp"
#include "vertex_array.hpp"

#include <glad/glad.h>


namespace calmar {
    /*  This struct stores definitions of clear buffers. The purpose of it is
        rendering API abstraction
    */
    struct clearBuffers {
        /// @brief Initializes clearing buffers for a specific rendering backend
        /// @param backend The rendering backend to use
        static void init();

        static u32 colorBuffer;
        static u32 depthBuffer;
        static u32 stencilBuffer;
    };
    /* This class is used to abstract general functionality that does not need
        initialization or larger systems to implement. */
    class renderCommand {
       public:
        /// @brief Initializes the render commands by setting a rendering API to
        /// render with
        /// @param backend The rendering backend to use
        static void init(const renderingBackend& backend);

        /// @brief This is an abstraction for clearing the screen buffers
        /// @param buffers The buffer flags to clear
        static void clearBuffers(u32 buffers);

        /// @brief This is an abstraction for clearing the screen with a given
        /// color
        /// @param color The color to clear the screen with
        static void clearColor(const glm::vec4& color);

        /// @brief This is an abstraction for setting a viewport
        /// @param width The width of the viewport
        /// @param height The height of the viewport
        static void setViewport(u32 width, u32 height);

        static void drawIndexed(u32 count);

        static void drawIndexed(const std::shared_ptr<vertexArray>& va);

        static void drawIndexed(const std::shared_ptr<vertexArray>& va, u32 count);

       private:
        static renderingBackend mBackend;
    };
}  // namespace calmar
