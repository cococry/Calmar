#include "render_data_types.hpp"

#include "calmar/core/application.hpp"

#include <glad/glad.h>

namespace calmar {
    u32 renderDataTypes::byte8;
    u32 renderDataTypes::unsignedByte8;
    u32 renderDataTypes::short16;
    u32 renderDataTypes::unsignedShort16;
    u32 renderDataTypes::int32;
    u32 renderDataTypes::unsignedInt32;
    u32 renderDataTypes::float32;

    void
    renderDataTypes::init() {
        switch (application::getInstance()->renderBackend) {
            case renderingBackend::OPENGL:
                byte8 = 0x1400;
                unsignedByte8 = 0x1401;
                short16 = 0x1402;
                unsignedShort16 = 0x1403;
                int32 = 0x1404;
                unsignedInt32 = 0x1405;
                float32 = 0x1406;
                break;
            default:
                byte8 = 0;
                unsignedByte8 = 0;
                short16 = 0;
                unsignedShort16 = 0;
                int32 = 0;
                unsignedInt32 = 0;
                float32 = 0;
                break;
        }
    }
}  // namespace calmar
