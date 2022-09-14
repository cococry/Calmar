#pragma once

#include "calmar/core/defines.hpp"

#include "rendering_backend.hpp"

#include <memory>

namespace calmar {
    /* This is struct is used to abstract render API specific buffer flags*/
    struct renderBufferFlags {
        static void init();

        static u32 dynamicDraw;
        static u32 staticDraw;
    };

    /* This class is used to abstract the rendering API specific process of createing a vertex buffer into a class. */
    class vertexBuffer {
       public:
        vertexBuffer() {}
        virtual ~vertexBuffer() {}

        /// @brief This static function creates a vertex buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param data The data to pass into the vertex buffer
        /// @param size  The size of the data for the buffer
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly stack allocated allocated vertex buffer
        static vertexBuffer create(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw);

        /// @brief This static function creates a vertex buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param size  The size of the data for the buffer
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly stack allocated allocated vertex buffer
        static vertexBuffer create(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw);

        /// @brief This static function creates a shared pointer vertex buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param data The data to pass into the vertex buffer
        /// @param size  The size of the data for the buffer
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly allocated allocated shared pointer vertex buffer
        static std::shared_ptr<vertexBuffer> createRef(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw);

        /// @brief This static function creates a shared pointer vertex buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param size  The size of the data for the buffer
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly allocated allocated shared pointer vertex buffer
        static std::shared_ptr<vertexBuffer> createRef(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw);

        /// @brief This static function creates a unique pointer buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param size  The size of the data for the buffer
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly allocated allocated unique pointer vertex buffer
        static std::unique_ptr<vertexBuffer> createScoped(float* data, u32 size, u32 bufferFlags = renderBufferFlags::staticDraw);

        /// @brief This static function creates a unique pointer buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param data The data to pass into the vertex buffer
        /// @param size  The size of the data for the buffer
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly allocated allocated unique pointer vertex buffer
        static std::unique_ptr<vertexBuffer> createScoped(u32 size, u32 bufferFlags = renderBufferFlags::dynamicDraw);

        /// @brief This virtual member function is intended to be overwritten by subclasses. It is used to bind the
        /// render ID of the vertex buffer to use it
        virtual void bind() {}

        /// @brief This virtual member function is intended to be overwritten by subclasses. It is used to unnbind the
        /// render ID of the vertex buffer to unuse it
        virtual void unbind() {}

        /// @brief This virtual member function is intended to be overwritten by subclasses. It is used to unnbind the
        /// render ID of the vertex buffer to unuse it
        virtual void deleteId() {}

        /// @brief This member function returns the renderer ID of this vertex buffer
        /// @return Returns the "Id" member variable which is used to define the the renderer ID of the buffer
        inline render_id getId() const {
            return mId;
        }

        /// @brief This virtual member function is intended to be overwritten by subclasses. It is used to set the
        /// the data of the vertex buffer after creation (renderBufferFlags::dynamicDraw should be used at creation
        /// to avoid undefined behaviour
        /// @param data The data to pass in to the buffer
        /// @param size The size of the data to pass in
        /// @param offset The offset to which to insert the data to in bytes
        virtual void setData(float* data, u32 size, u32 offset = 0) {}

       protected:
        render_id mId;
    };

    class indexBuffer {
       public:
        indexBuffer() {}
        virtual ~indexBuffer() {}

        /// @brief This static function creates a index buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param data The data to pass into the index buffer
        /// @param size The count of the elements in the data
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly stack allocated allocated index buffer
        static indexBuffer create(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw);

        /// @brief This static function creates a shared pointer index buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param data The data to pass into the index buffer
        /// @param size The count of the elements in the data
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly allocated allocated shared pointer index buffer
        static std::shared_ptr<indexBuffer> createRef(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw);

        /// @brief This static function creates a shared pointer index buffer with the correct rendering APi that is used
        /// in the engine.
        /// @param data The data to pass into the index buffer
        /// @param size The count of the elements in the data
        /// @param bufferFlags Flags for how to treat the buffer data
        /// @return Returns a newly allocated allocated unique pointer index buffer
        static std::unique_ptr<indexBuffer> createScoped(u32* data, u32 count, u32 bufferFlags = renderBufferFlags::staticDraw);

        /// @brief This virtual member function is intended to be overwritten by subclasses. It is used to bind the
        /// render ID of the index buffer to use it
        virtual void bind() {}

        /// @brief This virtual member function is intended to be overwritten by subclasses. It is used to unnbind the
        /// render ID of the index buffer to unuse it
        virtual void unbind() {}

        /// @brief This virtual member function is intended to be overwritten by subclasses. It is used to unnbind the
        /// render ID of the index buffer to unuse it
        virtual void deleteId() {}

        /// @brief This member function returns the renderer ID of this index buffer
        /// @return Returns the "Id" member variable which is used to define the the renderer ID of the buffer
        inline render_id getId() const {
            return mId;
        }

        /// @brief This member function returns the element count of the data of this buffer
        /// @return Returns the "Count" member variable which is used to define the element count of the data of the buffer
        inline u32 getCount() const {
            return mCount;
        }

       protected:
        render_id mId;
        u32 mCount;
    };

}  // namespace calmar
