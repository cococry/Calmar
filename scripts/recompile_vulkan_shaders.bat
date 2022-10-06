@echo off

glslc.exe ../engine/assets/shaders/vulkan/default2d.vert -o ../engine/assets/shaders/vulkan/bin/default2d_vert.spv
glslc.exe ../engine/assets/shaders/vulkan/default2d.frag -o ../engine/assets/shaders/vulkan/bin/default2d_frag.spv
