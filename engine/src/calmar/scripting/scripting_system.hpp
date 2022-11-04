#pragma once

#include "calmar/ecs/scene.hpp"

extern "C" {
    typedef struct _MonoClass MonoClass;
    typedef struct _MonoObject MonoObject;
    typedef struct _MonoMethod MonoMethod;
    typedef struct _MonoAssembly MonoAssembly;
    typedef struct _MonoImage MonoImage;
}

namespace calmar {
    class scriptClass {
    public:
        scriptClass() = default;

        scriptClass(const std::string& nameSpace, const std::string& name);

        MonoObject* instantiate();

        MonoMethod* getMethod(const std::string& name, int paramCount);

        MonoObject* invokeMethod(MonoMethod* method, MonoObject* instance, void** params = nullptr);
    private:
        std::string mNameSpace = "", mName = "";
        MonoClass* mMonoClass = nullptr;
    };

    class scriptingSystem {
       public:
        static void init();
        static void shutdown();

        static void loadCSharpAssembly(const std::filesystem::path& filepath);

        static void onRuntimeStart(scene* sceneContext);

        static void onRuntimeStop(scene* sceneContext);

        static void onCreateEntity(entity entty);

        static void onUpdateEntity(entity entity);

        static bool entityClassExists(const std::string& className);

        static std::unordered_map<std::string, std::shared_ptr<scriptClass>> getEntityClasses();

       private:
        static void initMono();

        static MonoObject* instantiateClass(MonoClass* monoClass);
        static void loadCSharpAssemblyClasses(MonoAssembly* assembly);

        friend class scriptClass;
    };

    class scriptInstance {
    public:
        scriptInstance(const std::shared_ptr<scriptClass>& script_class);

        void invokeInitMethod();
        void invokeUpdateMethod();

    private:
        std::shared_ptr<scriptClass> mScriptClass;

        MonoObject* mInstance = nullptr;
        MonoMethod* mInitMethod = nullptr;
        MonoMethod* mUpdateMethod = nullptr;
    };

}  // namespace calmar
