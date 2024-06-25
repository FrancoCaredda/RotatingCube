cmake_minimum_required(VERSION 3.8)

project("RotatingCubeApplication")
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
add_executable(${PROJECT_NAME} "src/main.cpp" 
                               "include/Application.h" 
                               "src/Application.cpp" 
                               "include/DebugLayer.h" 
                               "src/DebugLayer.cpp" 
                               "include/Window.h" 
                               "src/Window.cpp" 
                               "include/PhysicalDevice.h" 
                               "src/PhysicalDevice.cpp" 
                               "include/Renderer.h" 
                               "src/Renderer.cpp" 
                               "include/Specification.h" 
                               "src/Specification.cpp" 
                               "include/QueueFamilyIndices.h" 
                               "include/WindowManager.h"
                               "src/WindowManager.cpp" 
                               "include/MainWindow.h"
                               "src/MainWindow.cpp")

target_compile_definitions(${PROJECT_NAME} PUBLIC VK_USE_PLATFORM_WIN32_KHR
                                           PUBLIC GLFW_INCLUDE_VULKAN)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
                                                  "${CMAKE_SOURCE_DIR}/vulkan/include"
                                                  "${CMAKE_SOURCE_DIR}/glfw/include")
target_link_directories(${PROJECT_NAME} PUBLIC "${CMAKE_SOURCE_DIR}/vulkan/bin"
                                               "${CMAKE_SOURCE_DIR}/glfw/lib-vc2022")
target_link_libraries(${PROJECT_NAME} PUBLIC "vulkan-1.lib"
                                             "glfw3.lib")