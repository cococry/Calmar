
#include "scripting_system.hpp"

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

#include "calmar/core/asserting.hpp"

#include <fstream>
#include <sstream>

namespace calmar {

    struct scriptingSystemData {
        MonoDomain* monoRootDomain = nullptr;
        MonoDomain* monoAppDomain = nullptr;

        MonoAssembly* monoCoreAssmbly = nullptr;
    };

    static scriptingSystemData* scriptingData;

    char* readBytes(const std::string& filepath, uint32_t* outSize) {
        std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

        if (!stream) {
            CALMAR_ERROR("Failed to open file at location '{0}'.", filepath);
            return nullptr;
        }

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = end - stream.tellg();

        if (size == 0) {
            CALMAR_WARN("Tried to load bytes of empty file: '{0}'.", filepath);
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);
        stream.close();

        *outSize = size;
        return buffer;
    }

    MonoAssembly* loadCSharpAssembly(const std::string& assemblyPath) {
        uint32_t fileSize = 0;
        char* fileData = readBytes(assemblyPath, &fileSize);

        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK) {
            const char* errorMessage = mono_image_strerror(status);
            CALMAR_ERROR("Failed to load C# assembly with Mono.");
            CALMAR_TRACE(errorMessage);
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
        mono_image_close(image);

        delete[] fileData;

        return assembly;
    }

    void printAssemblyTypes(MonoAssembly* assembly) {
        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++) {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            printf("%s.%s\n", nameSpace, name);
        }
    }

    void scriptingSystem::init() {
        scriptingData = new scriptingSystemData();
        initMono();

    }
    void scriptingSystem::shutdown() {
        delete scriptingData;
    }

    void scriptingSystem::initMono() {
        mono_set_assemblies_path("mono/lib");

        MonoDomain* monoRootDomain = mono_jit_init("CalamarJITRuntime");
        CALMAR_ASSERT_MSG(monoRootDomain, "Failed to create mono root-domain.");

        scriptingData->monoRootDomain = monoRootDomain;

        scriptingData->monoAppDomain = mono_domain_create_appdomain((char*)"CalmarScriptRuntime", nullptr);
        mono_domain_set(scriptingData->monoAppDomain, true);

        scriptingData->monoCoreAssmbly = loadCSharpAssembly("res/scripting/calmar_scripting.dll");
        printAssemblyTypes(scriptingData->monoCoreAssmbly);
    }
}  // namespace calmar