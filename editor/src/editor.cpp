#include "editor.hpp"

#include "calmar/event_system/window_events.hpp"

#include "calmar/core/application.hpp"

#include "calmar/event_system/mouse_events.hpp"

namespace calmarEd {

    void editorAttachment::init() {
        application::getInstance()->evDispatcher.listen(mouseScrolledEvent::evType, std::bind(&editorAttachment::handleEvents, this, std::placeholders::_1));
        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f};

        u32 indices[] = {0, 1, 2, 2, 3, 0};

        mVertexArray = vertexArray::createRef(9);

        std::shared_ptr<vertexBuffer> vb = vertexBuffer::createRef(vertices, sizeof(vertices));

        mVertexArray->addVertexBuffer(vb);

        std::shared_ptr<indexBuffer> ib = indexBuffer::createRef(indices, sizeof(indices) / sizeof(u32));

        mVertexArray->setIndexBuffer(ib);

        mVertexArray->setVertexLayoutAttribute(3);
        mVertexArray->setVertexLayoutAttribute(4);
        mVertexArray->setVertexLayoutAttribute(2);

        mShader = shader::createRef("../engine/assets/shaders/default_vertex2d.glsl", "../engine/assets/shaders/default_fragment2d.glsl");
        mShader->bind();
        mShader->setInt("uTexture", 0);
        mShader->unbind();

        mTexture = texture2d::createRef("../engine/assets/textures/cpplogo.png");

        mEditorCamera = orbitCamera(cameraProperties());
    }

    void editorAttachment::update() {
        renderCommand::clearBuffers(clearBuffers::colorBuffer);
        renderCommand::clearColor({0.2f, 0.3f, 0.8f, 1.0f});

        mShader->bind();
        mShader->setMatrix4f("uViewProj", mEditorCamera.getViewProjection());
        mTexture->bind();
        mTexture->activateSlot(0);
        renderCommand::drawIndexed(mVertexArray);
        mShader->unbind();
        mTexture->unbind();

        mEditorCamera.update();
    }

    void editorAttachment::shutdown() {
    }

    void editorAttachment::handleEvents(const event& ev) {
        if (ev.type() == windowResizeEvent::evType) {
            const windowResizeEvent& resizeEvent = static_cast<const windowResizeEvent&>(ev);
            mEditorCamera.resize(resizeEvent.getWidth(), resizeEvent.getHeight());
        }
        mEditorCamera.handleEvents(ev);
    }
}  // namespace calmarEd
