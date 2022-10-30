#include "scene_hirarchy.hpp"

#include <calmar/core/application.hpp>
#include <calmar/core/util.hpp>
#include <calmar/input/input.hpp>
#include <calmar/input/mouse_codes.hpp>
#include <calmar/input/key_codes.hpp>
#include <calmar/renderer/resource_handler.hpp>
#include <calmar/core/asset_pool.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include <calmar/renderer/batch_renderer_2d.hpp>
#include <calmar/renderer/render_command.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace calmarEd {
    void sceneHirarchyPanel::init(const std::shared_ptr<scene>& scene) {
        setScene(scene);

        mDefaultTexture = resourceHandler::createTexture("../engine/assets/textures/checkerboardicon.png");

        mPreviewSubtexture = std::make_shared<indexedAtlasTexture>();
    }
    void sceneHirarchyPanel::update() {
        handleInput();
    }
    void sceneHirarchyPanel::renderImGui() {
        ImGui::Begin("Scene Hirarchy");
        if (input::isMouseButtonDown(button::Left) && ImGui::IsWindowHovered()) {
            mSelectedEntity = -1;
        }
        if (ImGui::BeginPopupContextWindow(0, 1, false)) {
            if (ImGui::MenuItem("Create Entity")) {
                entity entty = ECS.createEntity();
                if (!ECS.hasComponent<transformComponent>(entty))
                    ECS.addComponent(entty, transformComponent());
                if (!ECS.hasComponent<tagComponent>(entty))
                    ECS.addComponent(entty, tagComponent());
                mSelectedEntity = entty;
            }
            ImGui::EndPopup();
        }

        for (const entity& entty : mScene->mEntities) {
            ImGui::PushID(entty);
            renderImGuiEntityNode(entty);
            ImGui::PopID();
        }
        ImGui::End();

        ImGui::Begin("Components");
        if (mSelectedEntity != -1) {
            renderImGuiEntityComponents(mSelectedEntity);
        }
        ImGui::End();

        if (mDeletedEntity != -1) {
            ECS.destroyEntity(mDeletedEntity);
            mDeletedEntity = -1;
        }
    }

    void sceneHirarchyPanel::renderImGuiEntityNode(entity entty) {
        const auto& tag = ECS.getComponent<tagComponent>(entty);
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        flags |= mSelectedEntity == entty ? ImGuiTreeNodeFlags_Selected : 0;
        bool opened = ImGui::TreeNodeEx((void*)(intptr_t)entty, flags, "%s", tag.tag.c_str());

        if (ImGui::IsItemClicked()) {
            mSelectedEntity = entty;
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Duplicate")) {
                entity duplicatedEntity = duplicateEntity(entty);
                mSelectedEntity = duplicatedEntity;
            }
            if (ImGui::MenuItem("Delete")) {
                mDeletedEntity = entty;
                if (mSelectedEntity == entty) {
                    mSelectedEntity = -1;
                }
            }
            ImGui::EndPopup();
        }
        if (opened) {
            ImGui::TreePop();
        }
    }
    void sceneHirarchyPanel::renderImGuiEntityComponents(entity entty) {
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent")) {
            if (!ECS.hasComponent<spriteRendererComponent>(mSelectedEntity)) {
                if (ImGui::MenuItem("Sprite Renderer")) {
                    ECS.addComponent(entty, spriteRendererComponent());
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!ECS.hasComponent<transformComponent>(mSelectedEntity)) {
                if (ImGui::MenuItem("Transform")) {
                    ECS.addComponent(entty, transformComponent(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)));
                    ImGui::CloseCurrentPopup();
                }
            }

            if (!ECS.hasComponent<cameraComponent>(mSelectedEntity)) {
                if (ImGui::MenuItem("Camera")) {
                    ECS.addComponent(entty, cameraComponent());
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!ECS.hasComponent<indexedTextureComponent>(mSelectedEntity)) {
                if (ImGui::MenuItem("Sprite Atlas Texture")) {
                    ECS.addComponent(entty, indexedTextureComponent());
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!ECS.hasComponent<rigidBody2dComponent>(mSelectedEntity)) {
                if (ImGui::MenuItem("Rigidbody 2D")) {
                    ECS.addComponent(entty, rigidBody2dComponent());
                    ImGui::CloseCurrentPopup();
                }
            }
            if (!ECS.hasComponent<boxCollider2dComponent>(mSelectedEntity)) {
                if (ImGui::MenuItem("Box Collider 2D")) {
                    ECS.addComponent(entty, boxCollider2dComponent());
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::EndPopup();
        }

        if (ECS.hasComponent<tagComponent>(entty)) {
            auto& tag = ECS.getComponent<tagComponent>(entty);

            char buffer[128];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.tag.c_str());

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {
                tag.tag = std::string(buffer);
            }
        }
        if (ECS.hasComponent<transformComponent>(entty)) {
            auto& transform = ECS.getComponent<transformComponent>(entty);

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(spriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap, "Transform");

            if (open) {
                renderImGuiVec3Slider("Position", transform.position, entty);
                renderImGuiVec3Slider("Rotation", transform.rotation, entty, 0.0f);

                if (!ECS.hasComponent<cameraComponent>(entty))
                    renderImGuiVec3Slider("Scale", transform.scale, entty, 1.0f);

                ImGui::TreePop();
            }
            ImGui::PopStyleVar();
        }
        if (ECS.hasComponent<spriteRendererComponent>(entty)) {
            auto& spriteRenderer = ECS.getComponent<spriteRendererComponent>(entty);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(spriteRendererComponent).hash_code(),
                                          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                              ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap,
                                          "Sprite Renderer");

            ImGui::SameLine();
            if (ImGui::Button("-")) {
                ECS.removeComponent<spriteRendererComponent>(entty);
            }
            if (open) {
                ImGui::PushItemWidth(300.0f);
                ImGui::ColorEdit4("Color", glm::value_ptr(spriteRenderer.tint));
                ImGui::PopItemWidth();

                ImGui::Text("Texture:");
                if (spriteRenderer.texture) {
                    ImGui::Image((void*)(uintptr_t)spriteRenderer.texture->getId(), ImVec2{110.0f, 110.0f},
                                 ImVec2{0, 1}, ImVec2{1, 0});
                } else {
                    ImGui::Image((void*)(uintptr_t)mDefaultTexture->getId(), ImVec2{110.0f, 110.0f}, ImVec2{0, 1}, ImVec2{1, 0});
                }

                ImGui::SameLine();
                if (ImGui::Button("...")) {
                    std::string filepath = platform::fileDialogs::openFile("Image (*.png) (*.jpg)\0*.png;*.jpg\0");
                    if (!filepath.empty()) {
                        std::shared_ptr<texture2d> texture = resourceHandler::createTexture(filepath);
                        spriteRenderer.texture = texture;
                    }
                }

                if (spriteRenderer.texture) {
                    const char* filterTypeStrings[] = {"Nearest", "Linear"};
                    const char* currentFilterTypeString = filterTypeStrings[(int)spriteRenderer.texture->getData().filterMode];

                    if (ImGui::BeginCombo("Filter Mode", currentFilterTypeString)) {
                        for (int32_t i = 0; i < 2; ++i) {
                            bool isSelected = currentFilterTypeString == filterTypeStrings[i];
                            if (ImGui::Selectable(filterTypeStrings[i], isSelected)) {
                                if (spriteRenderer.texture->getData().filterMode != (textureFilterMode)i) {
                                    std::string filepath = spriteRenderer.texture->getData().filepath;
                                    spriteRenderer.texture = resourceHandler::createTexture(filepath, (textureFilterMode)i);
                                    spriteRenderer.texture->setFilterMode((textureFilterMode)i);
                                }
                            }
                            if (isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                }

                if (spriteRenderer.texture) {
                    if (ImGui::Button("Reset Texture")) {
                        spriteRenderer.texture = nullptr;
                    }
                }
                ImGui::TreePop();
            }
            ImGui::PopStyleVar();
        }
        if (ECS.hasComponent<cameraComponent>(entty)) {
            ImGuiIO io = ImGui::GetIO();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::Separator();

            bool open = ImGui::TreeNodeEx((void*)typeid(cameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap, "Camera");
            ImGui::SameLine();
            if (ImGui::Button("-")) {
                ECS.removeComponent<cameraComponent>(entty);
            }

            if (open) {
                if (ECS.hasComponent<cameraComponent>(entty)) {
                    auto& cameraComp = ECS.getComponent<cameraComponent>(entty);
                    auto& camera = cameraComp.camera;
                    ImGui::Checkbox("Select for Rendering", &cameraComp.selectedForRendering);

                    const char* projectionTypeStrings[] = {"Perspective", "Orthographic"};
                    const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.getProjectionType()];

                    if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                        for (int32_t i = 0; i < 2; ++i) {
                            bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
                            if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                                currentProjectionTypeString = projectionTypeStrings[i];
                                camera.setProjectionType((entityCamera::projectionType)i);
                            }
                            if (isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }
                    if (camera.getProjectionType() == entityCamera::projectionType::Perspective) {
                        float verticalFov = glm::degrees(camera.getPerspectiveVerticalFOV());
                        if (ImGui::DragFloat("Vertical FOV", &verticalFov))
                            camera.setPerspectiveVerticalFOV(glm::radians(verticalFov));

                        float perspectiveNear = camera.getPerspectiveNearClip();
                        if (ImGui::DragFloat("Near", &perspectiveNear))
                            camera.setPerspectiveNearClip(perspectiveNear);

                        float perspectiveFar = camera.getPerspectiveFarClip();
                        if (ImGui::DragFloat("Far", &perspectiveFar))
                            camera.setPerspectiveFarClip(perspectiveFar);
                    }
                    if (camera.getProjectionType() == entityCamera::projectionType::Orthographic) {
                        float orthoSize = camera.getOrthographicSize();
                        if (ImGui::DragFloat("Size", &orthoSize))
                            camera.setOrthographicSize(orthoSize);

                        float orthoNear = camera.getOrthographicNearClip();
                        if (ImGui::DragFloat("Near", &orthoNear))
                            camera.setOrthographicNearClip(orthoNear);

                        float orthoFar = camera.getOrthographicFarClip();
                        if (ImGui::DragFloat("Far", &orthoFar))
                            camera.setOrthographicFarClip(orthoFar);
                    }
                }
                ImGui::TreePop();
            }

            ImGui::PopStyleVar();
        }
        if (ECS.hasComponent<indexedTextureComponent>(mSelectedEntity)) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::Separator();

            bool open = ImGui::TreeNodeEx((void*)typeid(indexedTextureComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap, "Sprite Atlas Sub-Texture");
            ImGui::SameLine();
            if (ImGui::Button("-")) {
                ECS.removeComponent<indexedTextureComponent>(entty);
            }

            if (open) {
                if (ECS.hasComponent<indexedTextureComponent>(entty)) {
                    auto& indexedTextureComp = ECS.getComponent<indexedTextureComponent>(entty);

                    ImGui::Text("Position on Sheet");
                    ImGui::Separator();
                    ImGui::PushItemWidth(100);
                    ImGui::PushID(0);
                    if (ImGui::DragFloat("X", &indexedTextureComp.coordsOnSheet.x))
                        mChangedSubtexture = true;
                    ImGui::PopID();
                    ImGui::SameLine();
                    ImGui::PushID(1);
                    if (ImGui::DragFloat("Y", &indexedTextureComp.coordsOnSheet.y))
                        mChangedSubtexture = true;
                    ImGui::PopID();

                    ImGui::Text("Individual Cell Size");
                    ImGui::Separator();
                    ImGui::PushID(2);
                    if (ImGui::DragFloat("X", &indexedTextureComp.cellSize.x))
                        mChangedSubtexture = true;
                    ImGui::PopID();
                    ImGui::SameLine();
                    ImGui::PushID(3);
                    if (ImGui::DragFloat("Y", &indexedTextureComp.cellSize.y))
                        mChangedSubtexture = true;
                    ImGui::PopID();
                    ImGui::PopItemWidth();

                    ImGui::PushItemWidth(300.0f);
                    if (ImGui::ColorEdit4("Color", glm::value_ptr(indexedTextureComp.tint)))
                        mChangedSubtexture = true;
                    ImGui::PopItemWidth();

                    ImGui::Text("Atlas Texture");
                    ImGui::SameLine();

                    if (!indexedTextureComp.atlasTexture) {
                        ImGui::Image((void*)(uintptr_t)mDefaultTexture->getId(), ImVec2{110.0f, 110.0f}, ImVec2{0, 1}, ImVec2{1, 0});
                    } else {
                        ImGui::Image((void*)(uintptr_t)indexedTextureComp.atlasTexture->getId(), ImVec2{110.0f, 110.0f},
                                     ImVec2{0, 1}, ImVec2{1, 0});
                    }

                    if (indexedTextureComp.atlasTexture) {
                        ImGui::SameLine();
                        ImGui::Text("Preview");
                        ImGui::SameLine();
                        mPreviewSubtexture = indexedAtlasTexture::createWithCoords(indexedTextureComp.atlasTexture, indexedTextureComp.coordsOnSheet, indexedTextureComp.cellSize);
                        ImGui::Image((void*)(intptr_t)indexedTextureComp.atlasTexture->getId(),
                                     ImVec2{110.0f, 110.0f}, ImVec2{mPreviewSubtexture->getTextureCoords()[0].x, mPreviewSubtexture->getTextureCoords()[2].y},
                                     ImVec2{mPreviewSubtexture->getTextureCoords()[2].x, mPreviewSubtexture->getTextureCoords()[0].y});
                    }
                    if (ImGui::Button("...")) {
                        std::string filepath = platform::fileDialogs::openFile("Image (*.png) (*.jpg)\0*.png;*.jpg\0");
                        if (!filepath.empty()) {
                            std::shared_ptr<texture2d> texture = resourceHandler::createTexture(filepath, indexedTextureComp.atlasTextureFilterMode);
                            indexedTextureComp.atlasTexture = texture;
                        }
                    }
                    const char* filterTypeStrings[] = {"Nearest", "Linear"};
                    const char* currentFilterTypeString = filterTypeStrings[(int)indexedTextureComp.atlasTextureFilterMode];

                    if (ImGui::BeginCombo("Filter Mode", currentFilterTypeString)) {
                        for (int32_t i = 0; i < 2; ++i) {
                            bool isSelected = currentFilterTypeString == filterTypeStrings[i];
                            if (ImGui::Selectable(filterTypeStrings[i], isSelected)) {
                                if (indexedTextureComp.atlasTextureFilterMode != (textureFilterMode)i) {
                                    indexedTextureComp.atlasTextureFilterMode = (textureFilterMode)i;
                                    if (indexedTextureComp.atlasTexture) {
                                        std::string filepath = indexedTextureComp.atlasTexture->getData().filepath;
                                        indexedTextureComp.atlasTexture = resourceHandler::createTexture(filepath, indexedTextureComp.atlasTextureFilterMode);
                                    }
                                }
                            }
                            if (isSelected)
                                ImGui::SetItemDefaultFocus();
                        }
                        ImGui::EndCombo();
                    }

                    ImGui::Separator();

                    if (ImGui::Button("Create Sub Texture") && indexedTextureComp.atlasTexture) {
                        indexedTextureComp.indexedTexture = indexedAtlasTexture::createWithCoords(indexedTextureComp.atlasTexture, indexedTextureComp.coordsOnSheet, indexedTextureComp.cellSize);
                    }

                    if (indexedTextureComp.indexedTexture) {
                        if (ImGui::Button("Reset Texture")) {
                            indexedTextureComp.atlasTexture = nullptr;
                            indexedTextureComp.indexedTexture = nullptr;
                        }
                    }
                }
                ImGui::TreePop();
            }
            ImGui::PopStyleVar();
        }
        if (ECS.hasComponent<rigidBody2dComponent>(entty)) {
            auto& rigidBody2dComp = ECS.getComponent<rigidBody2dComponent>(entty);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(rigidBody2dComponent).hash_code(),
                                          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                              ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap,
                                          "Rigidbody 2D");

            ImGui::SameLine();
            if (ImGui::Button("-")) {
                ECS.removeComponent<rigidBody2dComponent>(entty);
            }
            if (open) {
                const char* bodyTypeStrings[] = {"Static", "Dynamic"};
                const char* currentBodyTypeString = bodyTypeStrings[(int)rigidBody2dComp.type];

                if (ImGui::BeginCombo("Filter Mode", currentBodyTypeString)) {
                    for (int32_t i = 0; i < 2; ++i) {
                        bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                        if (ImGui::Selectable(bodyTypeStrings[i], isSelected)) {
                            rigidBody2dComp.type = (rigidBody2dComponent::bodyType)i;
                        }
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                ImGui::Checkbox("Fixed Rotation", &rigidBody2dComp.fixedRotation);

                ImGui::TreePop();
            }
            ImGui::PopStyleVar();
        }
        if (ECS.hasComponent<boxCollider2dComponent>(entty)) {
            auto& boxCollider2dComp = ECS.getComponent<boxCollider2dComponent>(entty);
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(boxCollider2dComponent).hash_code(),
                                          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed |
                                              ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap,
                                          "Box Collider 2D");

            ImGui::SameLine();
            if (ImGui::Button("-")) {
                ECS.removeComponent<boxCollider2dComponent>(entty);
            }
            if (open) {
                ImGui::DragFloat2("Offset", glm::value_ptr(boxCollider2dComp.offset));
                ImGui::DragFloat2("Size", glm::value_ptr(boxCollider2dComp.size));
                ImGui::DragFloat("Density", &boxCollider2dComp.density, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Friction", &boxCollider2dComp.friction, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Restitution", &boxCollider2dComp.restitution, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Restitution Threshold", &boxCollider2dComp.restitutionThreshold, 0.01f, 0.0f, 1.0f);

                ImGui::TreePop();
            }

            ImGui::PopStyleVar();
        }
    }

    void sceneHirarchyPanel::renderImGuiVec3Slider(const std::string& label, glm::vec3& vec, entity entty, float resetValue) {
        ImGuiIO io = ImGui::GetIO();
        ImFont* font = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 135.0f);
        ImGui::Text("%s", label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{1, 0});

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = {lineHeight + 6.0f, lineHeight};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{RGBA_COLOR(190.0f, 65.0f, 65.0f, 255.0f)});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{RGBA_COLOR(210.0f, 80.0f, 80.0f, 255.0f)});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{RGBA_COLOR(165.0f, 50.0f, 50.0f, 255.0f)});

        ImGui::PushFont(font);
        if (ImGui::Button("X", buttonSize))
            vec.x = resetValue;
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##X", &vec.x, 0.15f, 0.0f, 0.0f, "%.3f");

        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushFont(font);
        if (ImGui::Button("Y", buttonSize))
            vec.y = resetValue;
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &vec.y, 0.15f, 0.0f, 0.0f, "%.3f");

        ImGui::PopItemWidth();

        ImGui::SameLine();

        ImGui::PushFont(font);
        if (ImGui::Button("Z", buttonSize))
            vec.z = resetValue;
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &vec.z, 0.15f, 0.0f, 0.0f, "%.3f");

        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);
        ImGui::Columns(1);
        ImGui::PopID();
    }

    void sceneHirarchyPanel::handleInput() {
        if (input::isKeyDown(key::LeftControl) && input::keyWentDown(key::D) && mSelectedEntity != -1) {
            duplicateEntity(mSelectedEntity);
        }
    }

    entity sceneHirarchyPanel::duplicateEntity(entity& entty) {
        entity newEntity = ECS.createEntity();

        duplicateComponentIfHas<tagComponent>(entty, newEntity);

        auto& tagComp = ECS.getComponent<tagComponent>(newEntity);
        auto& tagCompSrc = ECS.getComponent<tagComponent>(entty);
        tagComp.tag = tagCompSrc.tag;
        if (tagComp.tag.find(" (Copy)") == std::string::npos) {
            tagComp.tag += std::string(" (Copy)");
        }

        duplicateComponentIfHas<transformComponent>(entty, newEntity);

        auto& transformComp = ECS.getComponent<transformComponent>(newEntity);
        auto& transformCompSrc = ECS.getComponent<transformComponent>(entty);
        transformComp.position = transformCompSrc.position;
        transformComp.rotation = transformCompSrc.rotation;
        transformComp.scale = transformCompSrc.scale;

        duplicateComponentIfHas<spriteRendererComponent>(entty, newEntity);
        duplicateComponentIfHas<cameraComponent>(entty, newEntity);
        duplicateComponentIfHas<indexedTextureComponent>(entty, newEntity);
        duplicateComponentIfHas<rigidBody2dComponent>(entty, newEntity);
        duplicateComponentIfHas<boxCollider2dComponent>(entty, newEntity);

        return newEntity;
    }

    template <typename T>
    void sceneHirarchyPanel::duplicateComponentIfHas(entity source, entity dest) {
        if (!ECS.hasComponent<T>(dest)) {
            if (ECS.hasComponent<T>(source)) {
                ECS.addComponent(dest, ECS.getComponent<T>(source));
            }
        }
    }

}  // namespace calmarEd
