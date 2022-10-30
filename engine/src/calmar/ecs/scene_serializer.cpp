#include "scene_serializer.hpp"

#include <yaml-cpp/yaml.h>
#ifdef YAML_CPP_API
#undef YAML_CPP_API
#endif
#include <glm/glm.hpp>

#include <algorithm>

#include "calmar/core/application.hpp"

#include "calmar/renderer/resource_handler.hpp"

#include <fstream>

namespace YAML {
    template <>
    struct convert<glm::vec3> {
        static Node encode(const glm::vec3& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        static bool decode(const Node& node, glm::vec3& rhs) {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };
    template <>
    struct convert<glm::vec2> {
        static Node encode(const glm::vec2& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        static bool decode(const Node& node, glm::vec2& rhs) {
            if (!node.IsSequence() || node.size() != 2)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            return true;
        }
    };
    template <>
    struct convert<glm::vec4> {
        static Node encode(const glm::vec4& rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }
        static bool decode(const Node& node, glm::vec4& rhs) {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };

}  // namespace YAML

namespace calmar {
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& value) {
        out << YAML::Flow;
        out << YAML::BeginSeq << value.x << value.y << YAML::EndSeq;
        return out;
    }
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& value) {
        out << YAML::Flow;
        out << YAML::BeginSeq << value.x << value.y << value.z << YAML::EndSeq;
        return out;
    }

    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& value) {
        out << YAML::Flow;
        out << YAML::BeginSeq << value.x << value.y << value.z << value.w << YAML::EndSeq;
        return out;
    }
    sceneSerialzer::sceneSerialzer(const std::shared_ptr<scene>& inScene)
        : mScene(inScene) {
    }

    static void serializeEntity(YAML::Emitter& out, entity entity) {
        out << YAML::BeginMap;  // map for the entity
        out << YAML::Key << "entity" << YAML::Value << (int)entity;

        if (ECS.hasComponent<tagComponent>(entity)) {
            out << YAML::Key << "tagComponent";
            out << YAML::BeginMap;  // map for the tagComponent

            auto& tagComp = ECS.getComponent<tagComponent>(entity);
            std::string tagStr = tagComp.tag;

            out << YAML::Key << "tag" << YAML::Value << tagStr;

            out << YAML::EndMap;  // map for the tagComponent
        }
        if (ECS.hasComponent<transformComponent>(entity)) {
            out << YAML::Key << "transformComponent";
            out << YAML::BeginMap;  // map for the transformComponent

            auto& transformComp = ECS.getComponent<transformComponent>(entity);
            out << YAML::Key << "position" << YAML::Value << transformComp.position;
            out << YAML::Key << "rotation" << YAML::Value << transformComp.rotation;
            out << YAML::Key << "scale" << YAML::Value << transformComp.scale;

            out << YAML::EndMap;  // map for the transformComponent
        }
        if (ECS.hasComponent<cameraComponent>(entity)) {
            out << YAML::Key << "cameraComponent";
            out << YAML::BeginMap;  // map for cameraComponent

            auto& cameraComp = ECS.getComponent<cameraComponent>(entity);
            auto& camera = cameraComp.camera;

            out << YAML::Key << "camera" << YAML::Value;
            out << YAML::BeginMap;  // map for camera
            out << YAML::Key << "projectionType" << YAML::Value << (int)camera.getProjectionType();
            out << YAML::Key << "perspectiveVerticalFOV" << YAML::Value << camera.getPerspectiveVerticalFOV();
            out << YAML::Key << "perspectiveNearClip" << YAML::Value << camera.getPerspectiveNearClip();
            out << YAML::Key << "perspectiveFarClip" << YAML::Value << camera.getPerspectiveFarClip();
            out << YAML::Key << "orthographicSize" << YAML::Value << camera.getOrthographicSize();
            out << YAML::Key << "orthographicNearClip" << YAML::Value << camera.getOrthographicNearClip();
            out << YAML::Key << "orthographicFarClip" << YAML::Value << camera.getOrthographicFarClip();
            out << YAML::EndMap;  // map for camera

            out << YAML::Key << "selectedForRendering" << YAML::Value << cameraComp.selectedForRendering;

            out << YAML::EndMap;  // map for cameraComponent
        }

        if (ECS.hasComponent<spriteRendererComponent>(entity)) {
            out << YAML::Key << "spriteRendererComponent";
            out << YAML::BeginMap;  // map for spriteRendererComponent

            auto& spriteRendererComp = ECS.getComponent<spriteRendererComponent>(entity);
            out << YAML::Key << "tint" << YAML::Value << spriteRendererComp.tint;
            if (spriteRendererComp.texture) {
                std::string filepath = spriteRendererComp.texture->getData().filepath;
                std::replace(filepath.begin(), filepath.end(), '\\', '/');
                out << YAML::Key << "texture" << YAML::Value << filepath;
            } else {
                out << YAML::Key << "texture" << YAML::Value << " ";
            }
            out << YAML::EndMap;  // map for spriteRendererComponent
        }

        if (ECS.hasComponent<indexedTextureComponent>(entity)) {
            out << YAML::Key << "indexedTextureComponent";
            out << YAML::BeginMap;  // map for indexedTextureComponent

            auto& indexedTextureComp = ECS.getComponent<indexedTextureComponent>(entity);

            if (indexedTextureComp.atlasTexture) {
                std::string filepath = indexedTextureComp.atlasTexture->getData().filepath;
                std::replace(filepath.begin(), filepath.end(), '\\', '/');
                out << YAML::Key << "texture" << YAML::Value << filepath;
            } else {
                out << YAML::Key << "texture" << YAML::Value << " ";
            }
            out << YAML::Key << "textureFilterMode" << YAML::Value << (int)indexedTextureComp.atlasTextureFilterMode;
            out << YAML::Key << "cellSize" << YAML::Value << indexedTextureComp.cellSize;
            out << YAML::Key << "coordsOnSheet" << YAML::Value << indexedTextureComp.coordsOnSheet;
            out << YAML::Key << "tint" << YAML::Value << indexedTextureComp.tint;

            out << YAML::EndMap;  // map for indexedTextureComponent
        }
        if (ECS.hasComponent<rigidBody2dComponent>(entity)) {
            out << YAML::Key << "rigidBody2dComponent";
            out << YAML::BeginMap;

            auto& rigidBody2dComp = ECS.getComponent<rigidBody2dComponent>(entity);

            out << YAML::Key << "bodyType" << (int)rigidBody2dComp.type;
            out << YAML::Key << "fixedRotation" << rigidBody2dComp.fixedRotation;

            out << YAML::EndMap;  // map for rigidBody2dComponent
        }
        if (ECS.hasComponent<boxCollider2dComponent>(entity)) {
            out << YAML::Key << "boxCollider2dComponent";
            out << YAML::BeginMap;

            auto& boxCollider2dComp = ECS.getComponent<boxCollider2dComponent>(entity);

            out << YAML::Key << "density" << boxCollider2dComp.density;
            out << YAML::Key << "friction" << boxCollider2dComp.friction;
            out << YAML::Key << "offset" << boxCollider2dComp.offset;
            out << YAML::Key << "restitution" << boxCollider2dComp.restitution;
            out << YAML::Key << "restitutionThreshold" << boxCollider2dComp.restitutionThreshold;
            out << YAML::Key << "size" << boxCollider2dComp.size;

            out << YAML::EndMap;  // map for rigidBody2dComponent
        }
        out << YAML::EndMap;  // map for the entity
    }
    void sceneSerialzer::serialize(const std::string& filepath) {
        YAML::Emitter out;
        out << YAML::BeginMap;  // scene
        out << YAML::Key << "scene" << YAML::Value << "untitled";
        out << YAML::Key << "entities" << YAML::Value << YAML::BeginSeq;
        for (entity entty : mScene->mEntities) {
            serializeEntity(out, entty);
        }
        out << YAML::EndSeq;  // entities
        out << YAML::EndMap;  // scene

        std::ofstream file(filepath);
        file << out.c_str();
    }

    bool sceneSerialzer::deserialize(const std::string& filepath) {
        YAML::Node data;
        try {
            data = YAML::LoadFile(filepath);
        } catch (YAML::ParserException e) {
            return false;
        }

        if (!data["scene"])
            return false;

        std::string sceneName = data["scene"].as<std::string>();

        auto entities = data["entities"];

        if (entities) {
            for (auto entty : entities) {
                std::string name;
                auto tagComponentNode = entty["tagComponent"];
                if (tagComponentNode)
                    name = tagComponentNode["tag"].as<std::string>();

                entity newEntity = ECS.createEntity();

                auto& tagComp = ECS.getComponent<tagComponent>(newEntity);
                tagComp.tag = name;

                auto transformComponentNode = entty["transformComponent"];
                if (transformComponentNode) {
                    auto& transformComp = ECS.getComponent<transformComponent>(newEntity);
                    transformComp.position = transformComponentNode["position"].as<glm::vec3>();
                    transformComp.rotation = transformComponentNode["rotation"].as<glm::vec3>();
                    transformComp.scale = transformComponentNode["scale"].as<glm::vec3>();
                }

                auto cameraComponentNode = entty["cameraComponent"];
                if (cameraComponentNode) {
                    ECS.addComponent(newEntity, cameraComponent());

                    auto& cameraComp = ECS.getComponent<cameraComponent>(newEntity);

                    auto cameraProps = cameraComponentNode["camera"];
                    cameraComp.camera.setProjectionType((entityCamera::projectionType)cameraProps["projectionType"].as<int>());
                    cameraComp.camera.setPerspectiveVerticalFOV(cameraProps["perspectiveVerticalFOV"].as<float>());
                    cameraComp.camera.setPerspectiveNearClip(cameraProps["perspectiveNearClip"].as<float>());
                    cameraComp.camera.setPerspectiveFarClip(cameraProps["perspectiveFarClip"].as<float>());
                    cameraComp.camera.setOrthographicSize(cameraProps["orthographicSize"].as<float>());
                    cameraComp.camera.setOrthographicNearClip(cameraProps["orthographicNearClip"].as<float>());
                    cameraComp.camera.setOrthographicNearClip(cameraProps["orthographicFarClip"].as<float>());

                    cameraComp.selectedForRendering = cameraComponentNode["selectedForRendering"].as<bool>();
                }
                auto spriteRendererComponentNode = entty["spriteRendererComponent"];
                if (spriteRendererComponentNode) {
                    ECS.addComponent(newEntity, spriteRendererComponent());
                    auto& spriteRendererComp = ECS.getComponent<spriteRendererComponent>(newEntity);
                    spriteRendererComp.tint = spriteRendererComponentNode["tint"].as<glm::vec4>();
                    if (spriteRendererComponentNode["texture"].as<std::string>() != " ")
                        spriteRendererComp.texture = resourceHandler::createTexture(spriteRendererComponentNode["texture"].as<std::string>());
                }

                auto indexedTextureComponentNode = entty["indexedTextureComponent"];
                if (indexedTextureComponentNode) {
                    ECS.addComponent(newEntity, indexedTextureComponent());

                    auto& indexedTextureComp = ECS.getComponent<indexedTextureComponent>(newEntity);

                    indexedTextureComp.atlasTextureFilterMode = (textureFilterMode)indexedTextureComponentNode["textureFilterMode"].as<int>();
                    indexedTextureComp.cellSize = indexedTextureComponentNode["cellSize"].as<glm::vec2>();
                    indexedTextureComp.coordsOnSheet = indexedTextureComponentNode["coordsOnSheet"].as<glm::vec2>();
                    indexedTextureComp.tint = indexedTextureComponentNode["tint"].as<glm::vec4>();

                    if (indexedTextureComponentNode["texture"].as<std::string>() != " ")
                        indexedTextureComp.atlasTexture = resourceHandler::createTexture(indexedTextureComponentNode["texture"].as<std::string>(), indexedTextureComp.atlasTextureFilterMode);

                    indexedTextureComp.indexedTexture = indexedAtlasTexture::createWithCoords(indexedTextureComp.atlasTexture, indexedTextureComp.coordsOnSheet, indexedTextureComp.cellSize);
                }

                auto rigidBody2dComponentNode = entty["rigidBody2dComponent"];
                if (rigidBody2dComponentNode) {
                    ECS.addComponent(newEntity, rigidBody2dComponent());

                    auto& rigidBody2dComp = ECS.getComponent<rigidBody2dComponent>(newEntity);

                    rigidBody2dComp.fixedRotation = rigidBody2dComponentNode["fixedRotation"].as<bool>();
                    rigidBody2dComp.type = (rigidBody2dComponent::bodyType)rigidBody2dComponentNode["bodyType"].as<int>();
                }

                auto boxCollider2dComponentNode = entty["boxCollider2dComponent"];
                if (boxCollider2dComponentNode) {
                    ECS.addComponent(newEntity, boxCollider2dComponent());

                    auto& boxCollider2dComp = ECS.getComponent<boxCollider2dComponent>(newEntity);

                    boxCollider2dComp.density = boxCollider2dComponentNode["density"].as<float>();
                    boxCollider2dComp.friction = boxCollider2dComponentNode["friction"].as<float>();
                    boxCollider2dComp.offset = boxCollider2dComponentNode["offset"].as<glm::vec2>();
                    boxCollider2dComp.restitution = boxCollider2dComponentNode["restitution"].as<float>();
                    boxCollider2dComp.restitutionThreshold = boxCollider2dComponentNode["restitutionThreshold"].as<float>();
                    boxCollider2dComp.size = boxCollider2dComponentNode["size"].as<glm::vec2>();
                }
            }
        }

        return true;
    }

}  // namespace calmar
