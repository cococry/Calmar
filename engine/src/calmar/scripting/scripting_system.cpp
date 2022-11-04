#include "pch.h"
#include "scripting_system.hpp"

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

#include <iostream>

#include "calmar/core/util.hpp"
#include "calmar/core/application.hpp"
#include "scripting_base.h"

namespace calmar {
    struct scriptingSystemData {
        MonoDomain* monoRootDomain = nullptr;
        MonoDomain* monoAppDomain = nullptr;

        MonoAssembly* monoCoreAssmbly = nullptr;
        MonoImage* monoCoreAssmblyImage = nullptr;

        scene* sceneContext = nullptr;

        std::unordered_map<std::string, std::shared_ptr<scriptClass>> entityClasses;
        std::unordered_map<entity, std::shared_ptr<scriptInstance>> entityInstances;
    };

    static scriptingSystemData* scriptingData;

    namespace mono {
        static MonoAssembly* loadAssembly(const std::filesystem::path& assemblyPath) {
            uint32_t fileSize = 0;
            char* fileData = util::getFileContentsBytes(assemblyPath.string(), &fileSize);

            MonoImageOpenStatus status;
            MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

            if (status != MONO_IMAGE_OK) {
                const char* errorMessage = mono_image_strerror(status);
                CALMAR_ERROR("Failed to load C# assembly with Mono.");
                CALMAR_TRACE(errorMessage);
                return nullptr;
            }

            std::string pathStr = assemblyPath.string();
            MonoAssembly* assembly = mono_assembly_load_from_full(image, pathStr.c_str(), &status, 0);
            mono_image_close(image);

            delete[] fileData;

            return assembly;
        }
        namespace debug {
            void printAssemblyTypes(MonoAssembly* assembly) {
                MonoImage* image = mono_assembly_get_image(assembly);
                const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
                u32 numTypes = mono_table_info_get_rows(typeDefinitionsTable);

                for (u32 i = 0; i < numTypes; i++) {
                    u32 cols[MONO_TYPEDEF_SIZE];
                    mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

                    const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
                    const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

                    printf("%s.%s\n", nameSpace, name);
                }
            }
        }
    }

    void scriptingSystem::init() {
        scriptingData = new scriptingSystemData();
        initMono();
        loadCSharpAssembly("res/scripting/calmar_scripting.dll");
        loadCSharpAssemblyClasses(scriptingData->monoCoreAssmbly);

        scriptingBase::registerMethods();
    }
    void scriptingSystem::shutdown() {
        scriptingData->monoAppDomain = nullptr;
        scriptingData->monoRootDomain = nullptr;
        delete scriptingData;
    }

    void scriptingSystem::loadCSharpAssembly(const std::filesystem::path& filepath) {
        scriptingData->monoAppDomain = mono_domain_create_appdomain("CalmarEngineMonoScriptingRuntime", nullptr);
        mono_domain_set(scriptingData->monoAppDomain, true);

        scriptingData->monoCoreAssmbly = mono::loadAssembly(filepath);
        scriptingData->monoCoreAssmblyImage = mono_assembly_get_image(scriptingData->monoCoreAssmbly);
    }
    std::unordered_map<std::string, std::shared_ptr<scriptClass>> scriptingSystem::getEntityClasses() {
        return scriptingData->entityClasses;
    }


    void scriptingSystem::initMono() {
        mono_set_assemblies_path("mono/lib");

        MonoDomain* monoRootDomain = mono_jit_init("CalamarEngineMonoJITRuntime");
        CALMAR_ASSERT_MSG(monoRootDomain, "Failed to create mono root-domain.");

        scriptingData->monoRootDomain = monoRootDomain;
    }

    MonoObject* scriptingSystem::instantiateClass(MonoClass* monoClass) {
        MonoObject* instance = mono_object_new(scriptingData->monoAppDomain, monoClass);
        mono_runtime_object_init(instance);
        return instance;
    }

    void scriptingSystem::loadCSharpAssemblyClasses(MonoAssembly* assembly) {
        scriptingData->entityClasses.clear();

        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        u32 numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        MonoClass* entityClass = mono_class_from_name(image, "Calmar", "Entity");

        for (u32 i = 0; i < numTypes; i++) {
            u32 cols[MONO_TYPEDEF_SIZE];

            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            std::string formattedClassName;
            if (strlen(nameSpace) != 0) {
                formattedClassName = fmt::format("{}.{}", nameSpace, name);
            } else {
                formattedClassName = name;
            }
            MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);

            if (monoClass == entityClass) continue;

            bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
            if (isEntity) {
                scriptingData->entityClasses[formattedClassName] = std::make_shared<scriptClass>(nameSpace, name);
                CALMAR_INFO("Added entity {0}.{1}", nameSpace, name);
            }
        }
    }
    void scriptingSystem::onRuntimeStart(scene* sceneContext) {
        scriptingData->sceneContext = sceneContext;
    }

    void scriptingSystem::onRuntimeStop(scene* sceneContext) {
        scriptingData->sceneContext = nullptr;

        scriptingData->entityInstances.clear();
    }
    void scriptingSystem::onCreateEntity(entity entty) {
        if (ECS.hasComponent<cSharpScriptComponent>(entty)) {
            const auto& cSharpScriptComp = ECS.getComponent<cSharpScriptComponent>(entty);
            if (entityClassExists(cSharpScriptComp.name)) {
                std::shared_ptr<scriptInstance> instance = std::make_shared<scriptInstance>(scriptingData->entityClasses[cSharpScriptComp.name]);
                scriptingData->entityInstances[entty] = instance;
                instance->invokeInitMethod();
            }
        }
    }
    void scriptingSystem::onUpdateEntity(entity entty) {
        if (ECS.hasComponent<cSharpScriptComponent>(entty)) {
            const auto& cSharpScriptComp = ECS.getComponent<cSharpScriptComponent>(entty);
            if (entityClassExists(cSharpScriptComp.name)) {
                std::shared_ptr<scriptInstance> instance = std::make_shared<scriptInstance>(scriptingData->entityClasses[cSharpScriptComp.name]);
                scriptingData->entityInstances[entty] = instance;
                instance->invokeUpdateMethod();
            }
        }
    }
    bool scriptingSystem::entityClassExists(const std::string& className) {
        return scriptingData->entityClasses.find(className) != scriptingData->entityClasses.end();
    }
    scriptClass::scriptClass(const std::string& nameSpace, const std::string& name)
        : mNameSpace(nameSpace), mName(name) {
        mMonoClass = mono_class_from_name(scriptingData->monoCoreAssmblyImage, nameSpace.c_str(), name.c_str());
    }

    MonoObject* scriptClass::instantiate() {
        return scriptingSystem::instantiateClass(mMonoClass);
    }

    MonoMethod* scriptClass::getMethod(const std::string& name, int paramCount) {
        return mono_class_get_method_from_name(mMonoClass, name.c_str(), paramCount);
    }


    MonoObject* scriptClass::invokeMethod(MonoMethod* method, MonoObject* instance, void** params) {
        return mono_runtime_invoke(method, instance, params, nullptr);
    }

    scriptInstance::scriptInstance(const std::shared_ptr<scriptClass>& script_class) 
        : mScriptClass(script_class) {
        mInstance = script_class->instantiate();

        mInitMethod = script_class->getMethod("Init", 0);
        mUpdateMethod = script_class->getMethod("Update", 0);
    }

    void scriptInstance::invokeInitMethod() {
        mScriptClass->invokeMethod(mInitMethod, mInstance);

    }
    void scriptInstance::invokeUpdateMethod() {
        mScriptClass->invokeMethod(mUpdateMethod, mInstance);
    }

}  // namespace calmar