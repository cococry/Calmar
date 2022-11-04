#include "pch.h"
#include "scripting_base.h"

#include "calmar/core/application.hpp"

#include <mono/jit/jit.h>

namespace calmar {
#define CSHARP_INTERNAL_CALL(methodName) mono_add_internal_call("Calmar.InternalCalls::" #methodName, methodName);

    static float GetDeltaTime() {
        return application::getInstance()->getDeltaTime();
    }

	void scriptingBase::registerMethods() {
        CSHARP_INTERNAL_CALL(GetDeltaTime);
	}
}