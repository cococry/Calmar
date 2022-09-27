#pragma once

#include "calmar/core/defines.hpp"
#include "calmar/renderer/texture.hpp"
#include "calmar/renderer/orbit_camera.hpp"
#include "calmar/renderer/vertex_array.hpp"
#include "calmar/renderer/shader.hpp"

#include <glm/glm.hpp>
#include <memory>
#include <array>

namespace calmar {
    struct quadVertex {
        glm::vec3 position;
        glm::vec4 tint;
        glm::vec2 texCoord;
        float texSlotIndex;
    };
    class CALMAR_API batchRenderer2d {
       public:
        static void init();

        static void shutdown();

        static void beginRender(const orbitCamera& camera);

        static void endRender();

        static void drawData();

        static void renderQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, float rotation = 0.0f);

        static void renderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotation = 0.0f);

        static void renderQuad(const glm::vec2& position, const glm::vec2& scale, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint = glm::vec4(1.0f), float rotation = 0.0f);

        static void renderQuad(const glm::vec3& position, const glm::vec2& scale, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint = glm::vec4(1.0f), float rotation = 0.0f);

        static void renderQuad(const glm::mat4& transform, const glm::vec4& color);

        static void renderQuad(const glm::mat4& transform, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint = glm::vec4(1.0f));

        struct renderStats {
            u32 drawCalls = 0;
            u32 numberOfQuads = 0;

            u32 verticesCount = numberOfQuads * 6;
            u32 indicesCount = numberOfQuads * 4;
        };

        struct renderData {
            static const u32 maxQuadsBatch = 10000;
            static const u32 maxVerticesBatch = maxQuadsBatch * 4;
            static const u32 maxIndicesBatch = maxQuadsBatch * 6;
            static const u32 maxTextureSlotsBatch = 32;

            std::shared_ptr<vertexArray> quadVertexArray;
            std::shared_ptr<vertexBuffer> quadVertexBuffer;
            std::shared_ptr<shader> quadShader;
            std::shared_ptr<texture2d> textureWhite;

            u32 quadIndexCount = 0;

            quadVertex* quadVertexBufferBase = nullptr;
            quadVertex* quadVertexBufferPointer = nullptr;

            std::array<std::shared_ptr<texture2d>, maxTextureSlotsBatch> textures;
            u32 textureSlotIndex = 1;

            glm::vec4 quadVertexPositions[4];

            renderStats stats;
        };

        static renderStats getStats();

        static void resetStats();

        static inline const renderData& getRenderData() {
            return mData;
        }

       private:
        static renderData mData;

        static void drawReset();
    };
}  // namespace calmar
