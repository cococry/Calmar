#include "scene.hpp"

#include "ecs_defines.hpp"
#include "ecs.hpp"
#include "calmar/core/application.hpp"

#include "calmar/renderer/batch_renderer_2d.hpp"
#include "calmar/renderer/entity_camera.hpp"

#include "calmar/renderer/resource_handler.hpp"

#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

namespace calmar {
    static b2BodyType toBox2dRigidBodyType(rigidBody2dComponent::bodyType type) {
        switch (type) {
            case rigidBody2dComponent::bodyType::Static:
                return b2_staticBody;
                break;
            case rigidBody2dComponent::bodyType::Dynamic:
                return b2_dynamicBody;
                break;
            default:
                CALMAR_ASSERT_MSG(false, "Invalid rigidbody type in use.");
                return b2_staticBody;
                break;
        }
    }
    scene::scene(bool setComponentSet) {
        mCameraTexture = resourceHandler::createTexture("../editor/assets/icons/camera-icon.png");
        if (setComponentSet) {
            componentSet listenedComponents;
            listenedComponents.set(ECS.getComponentType<transformComponent>());

            ECS.setSystemComponentSet<scene>(listenedComponents);
        }
    }
    void scene::init() {
        mCameraTexture = resourceHandler::createTexture("../editor/assets/icons/camera-icon.png");

        componentSet listenedComponents;
        listenedComponents.set(ECS.getComponentType<transformComponent>());

        ECS.setSystemComponentSet<scene>(listenedComponents);
    }
    scene::~scene() {
    }
    void scene::update() {
        batchRenderer2d::beginRender(application::getInstance()->getEditorAttachment()->camera);
        for (auto const& entity : mEntities) {
            auto const& transform = ECS.getComponent<transformComponent>(entity);
            if (ECS.hasComponent<spriteRendererComponent>(entity)) {
                auto const& spriteRenderer = ECS.getComponent<spriteRendererComponent>(entity);

                if (spriteRenderer.texture != nullptr)
                    batchRenderer2d::renderQuad(transform.getTransform(), spriteRenderer.texture, spriteRenderer.tint, entity);
                else {
                    batchRenderer2d::renderQuad(transform.getTransform(), spriteRenderer.tint, entity);
                }
            } else if (ECS.hasComponent<cameraComponent>(entity) && !ECS.hasComponent<spriteRendererComponent>(entity)) {
                batchRenderer2d::renderQuad(transform.getTransform(), mCameraTexture, glm::vec4(1.0f), entity);
            } else if (ECS.hasComponent<indexedTextureComponent>(entity) && !ECS.hasComponent<spriteRendererComponent>(entity)) {
                auto& indexedTextureComp = ECS.getComponent<indexedTextureComponent>(entity);
                if (indexedTextureComp.indexedTexture && indexedTextureComp.atlasTexture) {
                    batchRenderer2d::renderQuad(transform.getTransform(), indexedTextureComp.indexedTexture, indexedTextureComp.tint, entity);
                }
            }
        }
        batchRenderer2d::endRender();
    }

    void scene::updateRuntime() {
        entityCamera* renderCamera = nullptr;
        glm::mat4 cameraTransform;

        for (auto const& entty : mEntities) {
            if (ECS.hasComponent<cameraComponent>(entty)) {
                auto& cameraComp = ECS.getComponent<cameraComponent>(entty);
                auto& transformComp = ECS.getComponent<transformComponent>(entty);
                if (cameraComp.selectedForRendering) {
                    renderCamera = &cameraComp.camera;
                    cameraTransform = transformComp.getTransform();
                }
            }
        }

        {
            const i32 velocityIterations = 6;
            const i32 positionIterations = 2;
            mPhysicsWorld->Step(application::getInstance()->getDeltaTime(), velocityIterations, positionIterations);

            for (auto& entty : mEntities) {
                if (ECS.hasComponent<rigidBody2dComponent>(entty)) {
                    auto& transformComp = ECS.getComponent<transformComponent>(entty);
                    auto& rigidBody2dComp = ECS.getComponent<rigidBody2dComponent>(entty);

                    b2Body* body = (b2Body*)rigidBody2dComp.runtimeBody;

                    const auto& position = body->GetPosition();
                    transformComp.position.x = position.x;
                    transformComp.position.y = position.y;
                    transformComp.rotation.z = body->GetAngle();
                }
            }
        }
        if (renderCamera) {
            batchRenderer2d::beginRender(*renderCamera, cameraTransform);
            for (auto const& entty : mEntities) {
                if (ECS.hasComponent<transformComponent>(entty)) {
                    auto const& transformComp = ECS.getComponent<transformComponent>(entty);
                    if (ECS.hasComponent<spriteRendererComponent>(entty)) {
                        auto const& spriteRendererComp = ECS.getComponent<spriteRendererComponent>(entty);

                        if (spriteRendererComp.texture) {
                            batchRenderer2d::renderQuad(transformComp.getTransform(), spriteRendererComp.texture, spriteRendererComp.tint, entty);
                        } else {
                            batchRenderer2d::renderQuad(transformComp.position, glm::vec2(transformComp.scale.x, transformComp.scale.y), spriteRendererComp.tint, transformComp.rotation, entty);
                        }
                    }
                    if (ECS.hasComponent<indexedTextureComponent>(entty) && !ECS.hasComponent<spriteRendererComponent>(entty && !ECS.hasComponent<cameraComponent>(entty))) {
                        auto& indexedTextureComp = ECS.getComponent<indexedTextureComponent>(entty);
                        if (indexedTextureComp.indexedTexture) {
                            batchRenderer2d::renderQuad(transformComp.getTransform(), indexedTextureComp.indexedTexture, indexedTextureComp.tint, entty);
                        }
                    }
                }
            }
            batchRenderer2d::endRender();
        }
    }

    entity scene::getRenderCameraEntity() const {
        for (auto const& entty : mEntities) {
            if (ECS.hasComponent<cameraComponent>(entty)) {
                auto const& cameraComp = ECS.getComponent<cameraComponent>(entty);
                if (cameraComp.selectedForRendering) {
                    return entty;
                }
            }
        }
        return -1;
    }

    std::shared_ptr<scene> scene::copy(std::shared_ptr<scene> sceneToCopy) {
        std::shared_ptr<scene> newScene = std::make_shared<scene>();

        newScene->mEntities = sceneToCopy->mEntities;

        return newScene;
    }

    void scene::handleResize(u32 width, u32 height) {
        for (auto const& entty : mEntities) {
            if (ECS.hasComponent<cameraComponent>(entty)) {
                auto& cameraComp = ECS.getComponent<cameraComponent>(entty);

                cameraComp.camera.setViewportSize(width, height);
            }
        }
    }

    void scene::onRuntimeStart() {
        mPhysicsWorld = new b2World({0.0f, -9.8f});

        for (auto& entty : mEntities) {
            mEditorTransforms.push_back(ECS.getComponent<transformComponent>(entty));
            if (ECS.hasComponent<rigidBody2dComponent>(entty)) {
                auto& transformComp = ECS.getComponent<transformComponent>(entty);
                auto& rigidBody2dComp = ECS.getComponent<rigidBody2dComponent>(entty);

                b2BodyDef bodyDef;
                bodyDef.type = toBox2dRigidBodyType(rigidBody2dComp.type);
                bodyDef.position.Set(transformComp.position.x, transformComp.position.y);
                bodyDef.angle = transformComp.rotation.z;

                b2Body* body = mPhysicsWorld->CreateBody(&bodyDef);
                body->SetFixedRotation(rigidBody2dComp.fixedRotation);
                rigidBody2dComp.runtimeBody = body;

                if (ECS.hasComponent<boxCollider2dComponent>(entty)) {
                    auto& boxCollider2DComp = ECS.getComponent<boxCollider2dComponent>(entty);

                    b2PolygonShape boxShape;
                    boxShape.SetAsBox(boxCollider2DComp.size.x * transformComp.scale.x, boxCollider2DComp.size.y * transformComp.scale.y);

                    b2FixtureDef fixtureDef;
                    fixtureDef.shape = &boxShape;
                    fixtureDef.density = boxCollider2DComp.density;
                    fixtureDef.friction = boxCollider2DComp.friction;
                    fixtureDef.restitution = boxCollider2DComp.restitution;
                    fixtureDef.restitutionThreshold = boxCollider2DComp.restitutionThreshold;
                    body->CreateFixture(&fixtureDef);
                }
            }
        }
    }

    void scene::onRuntimeStop() {
        int i = 0;
        for (auto& entty : mEntities) {
            auto& transformComp = ECS.getComponent<transformComponent>(entty);

            transformComp.position = mEditorTransforms[i].position;
            transformComp.rotation = mEditorTransforms[i].rotation;
            transformComp.scale = mEditorTransforms[i].scale;
            i++;
        }
        delete mPhysicsWorld;
        mPhysicsWorld = nullptr;
    }
}  // namespace calmar
