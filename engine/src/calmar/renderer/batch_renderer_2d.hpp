#pragma once

#include "calmar/renderer/texture.hpp"
#include "calmar/renderer/indexed_atlas_texture.hpp"
#include "calmar/renderer/orbit_camera.hpp"
#include "calmar/renderer/vertex_array.hpp"
#include "calmar/renderer/shader.hpp"
#include "calmar/renderer/entity_camera.hpp"

namespace calmar {
    struct quadVertex {
        glm::vec3 position;
        glm::vec4 tint;
        glm::vec2 texCoord;
        float texSlotIndex;

        i32 entityId;
    };
    class batchRenderer2d {
       public:
        static void init();

        static void shutdown();

        static void beginRender(const orbitCamera& camera);

        static void beginRender(const entityCamera& camera, const glm::mat4& transform);

        static void endRender();

        static void drawData();

        static void renderQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, glm::vec3 rotation = glm::vec3(0.0f), i32 entityId = -1);

        static void renderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, glm::vec3 rotation = glm::vec3(0.0f), i32 entityId = -1);

        static void renderQuad(const glm::vec2& position, const glm::vec2& scale, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint = glm::vec4(1.0f), glm::vec3 rotation = glm::vec3(0.0f), i32 entityId = -1);

        static void renderQuad(const glm::vec3& position, const glm::vec2& scale, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint = glm::vec4(1.0f), glm::vec3 rotation = glm::vec3(0.0f), i32 entityId = -1);

        static void renderQuad(const glm::vec2& position, const glm::vec2& scale, const std::shared_ptr<indexedAtlasTexture>& indexedTexture, const glm::vec4& tint = glm::vec4(1.0f), glm::vec3 rotation = glm::vec3(0.0f), i32 entityId = -1);

        static void renderQuad(const glm::vec3& position, const glm::vec2& scale, const std::shared_ptr<indexedAtlasTexture>& indexedTexture, const glm::vec4& tint = glm::vec4(1.0f), glm::vec3 rotation = glm::vec3(0.0f), i32 entityId = -1);

        static void renderQuad(const glm::mat4& transform, const glm::vec4& color, i32 entityId = -1);

        static void renderQuad(const glm::mat4& transform, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint = glm::vec4(1.0f), i32 entityId = -1);

        static void renderQuad(const glm::mat4& transform, const std::shared_ptr<indexedAtlasTexture>& indexedTexture, const glm::vec4& tint = glm::vec4(1.0f), i32 entityId = -1);

        struct renderStats {
            u32 drawCalls = 0;
            u32 numberOfQuads = 0;
            u32 numberOfCircles = 0;

            u32 verticesCount = (numberOfQuads + numberOfCircles) * 6;
            u32 indicesCount = (numberOfQuads + numberOfCircles) * 4;
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
