#include "batch_renderer_2d.hpp"

#include "render_command.hpp"

#include "calmar/core/logging.hpp"

#include "calmar/core/application.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace calmar {
    batchRenderer2d::renderData batchRenderer2d::mData;

    void batchRenderer2d::init() {
        mData.quadVertexArray = vertexArray::createRef(10);

        mData.quadVertexBuffer = vertexBuffer::createRef(mData.maxVerticesBatch * sizeof(quadVertex));
        mData.quadVertexArray->addVertexBuffer(mData.quadVertexBuffer);

        mData.quadVertexArray->setVertexLayoutAttribute(layoutAttributeType::VEC3);
        mData.quadVertexArray->setVertexLayoutAttribute(layoutAttributeType::VEC4);
        mData.quadVertexArray->setVertexLayoutAttribute(layoutAttributeType::VEC2);
        mData.quadVertexArray->setVertexLayoutAttribute(layoutAttributeType::FLOAT);

        mData.quadVertexBufferBase = new quadVertex[mData.maxVerticesBatch];

        u32* quadIndices = new u32[mData.maxIndicesBatch];

        u32 quadOffset = 0;
        for (u32 i = 0; i < mData.maxIndicesBatch; i += 6) {
            quadIndices[i + 0] = quadOffset + 0;
            quadIndices[i + 1] = quadOffset + 1;
            quadIndices[i + 2] = quadOffset + 2;

            quadIndices[i + 3] = quadOffset + 2;
            quadIndices[i + 4] = quadOffset + 3;
            quadIndices[i + 5] = quadOffset + 0;

            quadOffset += 4;
        }
        std::shared_ptr<indexBuffer> quadIndexBuffer = indexBuffer::createRef(quadIndices, mData.maxIndicesBatch);
        mData.quadVertexArray->setIndexBuffer(quadIndexBuffer);
        delete[] quadIndices;

        mData.textureWhite = texture2d::createRef(1, 1);

        u32 whiteTextureData = 0xffffffff;
        mData.textureWhite->setData(&whiteTextureData, sizeof(u32));

        i32 samplers[mData.maxTextureSlotsBatch];
        for (u32 i = 0; i < mData.maxTextureSlotsBatch; i++)
            samplers[i] = i;

        mData.quadShader = shader::createRef("../engine/assets/shaders/opengl/default_vertex2d.glsl", "../engine/assets/shaders/opengl/default_fragment2d.glsl");

        mData.quadShader->bind();
        mData.quadShader->setIntArray("uTextures", samplers, mData.maxTextureSlotsBatch);

        mData.textures[0] = mData.textureWhite;
        mData.quadVertexPositions[0] = {-0.5f, -0.5f, -0.0f, 1.0f};
        mData.quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        mData.quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        mData.quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};
    }
    void batchRenderer2d::shutdown() {
    }
    void batchRenderer2d::beginRender(const orbitCamera& camera) {
        if (USING_COMPATABLE_RENDERING_API) {
            mData.quadShader->bind();
            mData.quadShader->setMatrix4f("uViewProj", camera.getViewProjection());

            mData.quadIndexCount = 0;
            mData.quadVertexBufferPointer = mData.quadVertexBufferBase;

            mData.textureSlotIndex = 1;
            resetStats();
        }
    }
    void batchRenderer2d::endRender() {
        if (USING_COMPATABLE_RENDERING_API) {
            drawData();
        }
    }
    void batchRenderer2d::drawData() {
        if (mData.quadIndexCount == 0)
            return;

        u32 dataSize = (u32)((u8*)mData.quadVertexBufferPointer - (u8*)mData.quadVertexBufferBase);
        mData.quadVertexBuffer->setData(mData.quadVertexBufferBase, dataSize);

        for (u32 i = 0; i < mData.textureSlotIndex; i++)
            mData.textures[i]->bindUnit(i);

        renderCommand::drawIndexed(mData.quadIndexCount);

        mData.stats.drawCalls++;
    }
    void batchRenderer2d::renderQuad(const glm::vec2& position, const glm::vec2& scale, const glm::vec4& color, float rotation) {
        if (USING_COMPATABLE_RENDERING_API)
            renderQuad({position.x, position.y, 0.0f}, scale, color, rotation);
    }
    void batchRenderer2d::renderQuad(const glm::vec3& position, const glm::vec2& scale, const glm::vec4& color, float rotation) {
        if (USING_COMPATABLE_RENDERING_API) {
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});
            renderQuad(transform, color);
        }
    }
    void batchRenderer2d::renderQuad(const glm::vec2& position, const glm::vec2& scale, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint, float rotation) {
        if (USING_COMPATABLE_RENDERING_API)
            renderQuad({position.x, position.y, 0.0f}, scale, texture, tint, rotation);
    }
    void batchRenderer2d::renderQuad(const glm::vec3& position, const glm::vec2& scale, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint, float rotation) {
        if (USING_COMPATABLE_RENDERING_API) {
            constexpr u32 quadVertexCount = 4;
            constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

            if (mData.quadIndexCount >= renderData::maxIndicesBatch) {
                drawReset();
            }

            float textureIndex = 0.0f;

            for (u32 i = 1; i < mData.textureSlotIndex; i++) {
                if (*mData.textures[i].get() == *texture.get()) {
                    textureIndex = (float)i;
                    break;
                }
            }

            if (textureIndex == 0.0f) {
                textureIndex = (float)mData.textureSlotIndex;
                mData.textures[mData.textureSlotIndex] = texture;
                mData.textureSlotIndex++;
            }

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), {scale.x, scale.y, 1.0f});

            for (u32 i = 0; i < quadVertexCount; i++) {
                mData.quadVertexBufferPointer->position = transform * mData.quadVertexPositions[i];
                mData.quadVertexBufferPointer->tint = tint;
                mData.quadVertexBufferPointer->texCoord = textureCoords[i];
                mData.quadVertexBufferPointer->texSlotIndex = textureIndex;
                mData.quadVertexBufferPointer++;
            }
            mData.quadIndexCount += 6;

            mData.stats.numberOfQuads++;
        }
    }

    void batchRenderer2d::renderQuad(const glm::mat4& transform, const glm::vec4& color) {
        if (USING_COMPATABLE_RENDERING_API) {
            if (mData.quadIndexCount >= mData.maxIndicesBatch)
                drawReset();

            const float texIndex = 0.0f;
            constexpr size_t quadVertexCount = 4;
            constexpr glm::vec2 textureCoords[] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

            for (u32 i = 0; i < quadVertexCount; i++) {
                mData.quadVertexBufferPointer->position = transform * mData.quadVertexPositions[i];
                mData.quadVertexBufferPointer->tint = color;
                mData.quadVertexBufferPointer->texCoord = textureCoords[i];
                mData.quadVertexBufferPointer->texSlotIndex = texIndex;
                mData.quadVertexBufferPointer++;
            }

            mData.quadIndexCount += 6;

            mData.stats.numberOfQuads++;
        }
    }

    void batchRenderer2d::renderQuad(const glm::mat4& transform, const std::shared_ptr<texture2d>& texture, const glm::vec4& tint) {
        if (USING_COMPATABLE_RENDERING_API) {
            if (mData.quadIndexCount >= mData.maxIndicesBatch)
                drawReset();

            constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
            constexpr uint32_t quadVertexCount = 4;
            constexpr glm::vec2 textureCoords[] = {{
                                                       0.0f,
                                                       0.0f,
                                                   },
                                                   {1.0f, 0.0f},
                                                   {1.0f, 1.0f},
                                                   {0.0f, 1.0f}};

            float textureIndex = 0.0f;

            for (uint32_t i = 1; i < mData.textureSlotIndex; i++) {
                if (*mData.textures[i].get() == *texture.get()) {
                    textureIndex = (float)i;
                    break;
                }
            }

            if (textureIndex == 0.0f) {
                textureIndex = (float)mData.textureSlotIndex;
                mData.textures[mData.textureSlotIndex] = texture;
                mData.textureSlotIndex++;
            }

            for (uint32_t i = 0; i < quadVertexCount; i++) {
                mData.quadVertexBufferPointer->position = transform * mData.quadVertexPositions[i];
                mData.quadVertexBufferPointer->tint = color;
                mData.quadVertexBufferPointer->texCoord = textureCoords[i];
                mData.quadVertexBufferPointer->texSlotIndex = textureIndex;
                mData.quadVertexBufferPointer++;
            }
            mData.quadIndexCount += 6;

            mData.stats.numberOfQuads++;
        }
    }

    batchRenderer2d::renderStats batchRenderer2d::getStats() {
        return mData.stats;
    }
    void batchRenderer2d::resetStats() {
        mData.stats.drawCalls = 0;
        mData.stats.indicesCount = 0;
        mData.stats.verticesCount = 0;
        mData.stats.numberOfQuads = 0;
    }

    void batchRenderer2d::drawReset() {
        endRender();

        mData.quadIndexCount = 0;
        mData.quadVertexBufferPointer = mData.quadVertexBufferBase;

        mData.textureSlotIndex = 1;
    }
}  // namespace calmar
