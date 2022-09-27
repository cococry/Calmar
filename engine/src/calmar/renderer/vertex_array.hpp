#pragma once

#include "calmar/core/defines.hpp"

#include "render_buffers.hpp"

#include "render_data_types.hpp"

#include <memory>

#include <vector>

#include <glad/glad.h>

namespace calmar {
    enum class layoutAttributeType {
        INTEGER = 0,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4
    };

    /* This class is used to abstract away the render API specific process of creating a vertex array */
    class CALMAR_API vertexArray {
       public:
        vertexArray() {}
        virtual ~vertexArray() {}

        /// @brief This static function creates a renderer API specific vertex array
        /// @param vertexStride The stride of one vertex in the vertices data in bytes
        /// @return Returns a newly stack allocated vertex array
        static vertexArray create(u32 vertexStride);

        /// @brief This static function creates a renderer API specific vertex array
        /// @param vertexStride The stride of one vertex in the vertices data in bytes
        /// @return Returns a newly allocated shared pointer vertex array
        static std::shared_ptr<vertexArray> createRef(u32 vertexStride);

        /// @brief This static function creates a renderer API specific vertex array
        /// @param vertexStride The stride of one vertex in the vertices data in bytes
        /// @return Returns a newly allocated unique pointer vertex array
        static std::unique_ptr<vertexArray> createScoped(u32 vertexStride);

        /// @brief This virtual member function is intended to be overwritten by subclasses.
        /// It is used to bind the renderer ID of the vertex array in order to use it
        virtual void bind() {}

        /// @brief This virtual member function is intended to be overwritten by subclasses.
        /// It is used to unbind the renderer ID of the vertex array in order to unuse it
        virtual void unbind() {}

        /// @brief This virtual member function is intended to be overwritten by subclasses.
        /// It is used to delete the renderer ID of the vertex array which makes it unusable
        virtual void deleteId() {}

        /// @brief This virtual member function is intended to be overwritten by subclassses.
        /// it is used to set a layout attribute of the vertex data of the assotiated vertex buffer.
        /// @param size The count of elements in the attribute
        /// @param stride The stride of a single vertex
        /// @param type The type of the data in the attribute
        /// @param normalized Defines if the data is normalized (lin alg. only)
        virtual void setVertexLayoutAttribute(layoutAttributeType dataTypeCount, u32 stride = 0, u32 type = renderDataTypes::float32, bool normalized = false) {}

        /// @brief This constant member function returns the list of vertex buffer assotiated with this vertex array
        /// @return Returns the private member variable "VertexBuffers"
        inline const std::vector<std::shared_ptr<vertexBuffer>>& getVertexBuffers() const {
            return mVertexBuffers;
        }

        /// @brief This constatnt member function returns the index buffer assotiated with this vertex array
        /// @return Returns the private member variable "IndexBuffer"
        inline const std::shared_ptr<indexBuffer>& getIndexBuffer() const {
            return mIndexBufer;
        }

        /// @brief This member function adds a vertex buffer to the vertex buffers assotiated
        /// to this vertex array
        /// @param buffer The vertex buffer to add to the vertex array
        inline void addVertexBuffer(const std::shared_ptr<vertexBuffer>& buffer) {
            mVertexBuffers.push_back(buffer);
        }

        /// @brief This member function sets a index buffer to the assotiated index buffer
        /// with this vertex array
        /// @param buffer The index buffer to set to the vertex array
        inline void setIndexBuffer(const std::shared_ptr<indexBuffer>& buffer) {
            mIndexBufer = buffer;
        }

        /// @brief This constant member function returns the renderer Id of this vertex array.
        /// @return Returns the private member variable "Id" which is used to define the renderer Id of the vertex array
        inline render_id getId() const {
            return mId;
        }

       protected:
        render_id mId;

        std::vector<std::shared_ptr<vertexBuffer>> mVertexBuffers;
        std::shared_ptr<indexBuffer> mIndexBufer;
    };
}  // namespace calmar
