#include "pch.h"
#include "scripting_base.h"
#include "scripting_system.hpp"

#include "calmar/core/application.hpp"
#include "calmar/input/input.hpp"

#include "calmar/input/key_codes.hpp"

#include <mono/jit/jit.h>

namespace calmar {
#define CSHARP_INTERNAL_CALL(methodName) mono_add_internal_call("Calmar.InternalCalls::" #methodName, methodName);

    static float GetDeltaTime() {
        return application::getInstance()->getDeltaTime();
    }

    static void SetEntityPosition(entity entty, glm::vec3* position) {
        scene* sceneContext = scriptingSystem::getSceneContext();

        auto& transformComp = ECS.getComponent<transformComponent>(entty);
        transformComp.position = *position;
    }

    static void GetEntityPosition(entity entty, glm::vec3* oPosition) {
        scene* sceneContext = scriptingSystem::getSceneContext();

        *oPosition = ECS.getComponent<transformComponent>(entty).position;
    }
    static bool IsKeyPressed(keyCode kc) {
        return input::isKeyDown(kc);
    }

	void scriptingBase::registerMethods() {
        CSHARP_INTERNAL_CALL(GetDeltaTime);
        CSHARP_INTERNAL_CALL(SetEntityPosition);
        CSHARP_INTERNAL_CALL(GetEntityPosition);
        CSHARP_INTERNAL_CALL(IsKeyPressed);
	}
}