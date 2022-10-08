#include "editor.hpp"

#include "calmar/event_system/window_events.hpp"
#include "calmar/event_system/mouse_events.hpp"

#include "calmar/core/application.hpp"
#include "calmar/core/random.hpp"

#include "calmar/renderer/render_command.hpp"

#include "calmar/ecs/ecs.hpp"
#include "calmar/ecs/components.hpp"

namespace calmarEd {

    void editorAttachment::init() {
        application::getInstance()->evDispatcher.listen(mouseScrolledEvent::evType, std::bind(&editorAttachment::handleEvents, this, std::placeholders::_1));

        mGrassTexture = texture2d::createRef("../engine/assets/textures/grasstexture.jpg");
        mPlayerTexture = texture2d::createRef("../engine/assets/textures/playertexture.png");
        mTreeTexture = texture2d::createRef("../engine/assets/textures/treetexture.png");

        application::getInstance()
            ->entityComponentSystem.registerComponent<transformComponent>();
        application::getInstance()->entityComponentSystem.registerComponent<spriteRendererComponent>();

        mRenderingSystem = application::getInstance()->entityComponentSystem.registerSystem<renderSystem>();

        componentSet systemComponentSet;
        systemComponentSet.set(application::getInstance()->entityComponentSystem.getComponentType<transformComponent>());
        systemComponentSet.set(application::getInstance()->entityComponentSystem.getComponentType<spriteRendererComponent>());
        application::getInstance()->entityComponentSystem.setSystemComponentSet<renderSystem>(systemComponentSet);

        mEditorCamera = orbitCamera(cameraProperties());

        for (u32 y = 0; y < 25; y++) {
            for (u32 x = 0; x < 25; x++) {
                entity entty = application::getInstance()->entityComponentSystem.createEntity();

                application::getInstance()->entityComponentSystem.addComponent(entty, transformComponent(glm::vec3(x, y, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
                application::getInstance()->entityComponentSystem.addComponent(entty, spriteRendererComponent(mGrassTexture));

                if (x % 4 == 0) {
                    entity tree = application::getInstance()->entityComponentSystem.createEntity();
                    random rnd = random();

                    i32 randX = rnd.nextInt(0, 24);
                    i32 randY = rnd.nextInt(0, 24);
                    while (randX == 12 || randY == 12) {
                        randX = rnd.nextInt(0, 24);
                        randY = rnd.nextInt(0, 24);
                    }
                    application::getInstance()->entityComponentSystem.addComponent(tree, transformComponent(glm::vec3(randX, randY, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
                    application::getInstance()->entityComponentSystem.addComponent(tree, spriteRendererComponent(mTreeTexture));
                }
            }
        }

        entity player = application::getInstance()->entityComponentSystem.createEntity();

        application::getInstance()->entityComponentSystem.addComponent(player, transformComponent(glm::vec3(12.0f, 12.0f, 0.1f), glm::vec3(0.0f), glm::vec3(1.0f)));
        application::getInstance()->entityComponentSystem.addComponent(player, spriteRendererComponent(mPlayerTexture));
    }

    void editorAttachment::update() {
        mEditorCamera.update();

        renderCommand::clearBuffers(clearBuffers::colorBuffer);
        renderCommand::clearColor({0.2f, 0.3f, 0.8f, 1.0f});
        batchRenderer2d::beginRender(mEditorCamera);
        mRenderingSystem->render();
        batchRenderer2d::endRender();
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
