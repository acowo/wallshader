
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <vulkan/vulkan_core.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <vulkan/vulkan_raii.hpp>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_to_string.hpp>


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return 1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "wallshader",
        nullptr,
        nullptr
    );

    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return 1;
    }

    try {
        if (!glfwVulkanSupported()) {
            throw std::runtime_error(
                "GLFW reports that Vulkan is unavailable"
            );
        }

        // Create the Vulkan context.
        vk::raii::Context context;

        // Query the Vulkan loader's API version.
        const auto apiVersion =
            context.enumerateInstanceVersion();

        std::cout
            << "Vulkan loader version: "
            << VK_VERSION_MAJOR(apiVersion) << "."
            << VK_VERSION_MINOR(apiVersion) << "."
            << VK_VERSION_PATCH(apiVersion)
            << '\n';

        // Describe our application.
        constexpr vk::ApplicationInfo appInfo{
            .pApplicationName = "wallshader",
            .applicationVersion = VK_MAKE_VERSION(0, 1, 0),
            .pEngineName = "wallshader",
            .engineVersion = VK_MAKE_VERSION(0, 1, 0),
            .apiVersion = vk::ApiVersion14
        };

        // Get extensions required by GLFW.
        uint32_t glfwExtensionCount = 0;

        const char** glfwExtensions =
            glfwGetRequiredInstanceExtensions(
                &glfwExtensionCount
            );

        if (!glfwExtensions) {
            throw std::runtime_error(
                "Failed to get GLFW Vulkan extensions"
            );
        }

        // Create the instance.
        vk::InstanceCreateInfo createInfo{
            .pApplicationInfo = &appInfo,
            .enabledExtensionCount = glfwExtensionCount,
            .ppEnabledExtensionNames = glfwExtensions
        };

        vk::raii::Instance instance{
            context,
            createInfo
        };

        std::cout << "Vulkan instance created!\n";

        // Enumerate available GPUs.
        auto physicalDevices =
            instance.enumeratePhysicalDevices();

        if (physicalDevices.empty()) {
            throw std::runtime_error(
                "No Vulkan-compatible GPUs found"
            );
        }

        std::cout
            << "Found "
            << physicalDevices.size()
            << " Vulkan physical device(s):\n";

        for (const auto& physicalDevice : physicalDevices) {
            const auto properties =
                physicalDevice.getProperties();

            std::cout
                << "  - "
                << properties.deviceName
                << '\n'
                << "  - "
                << properties.apiVersion
                << '\n'
                << "  - "
                << vk::to_string(properties.deviceType)
                << '\n'
                << "  - "
                << properties.vendorID
                << '\n'
                << "  - "
                << properties.deviceID
                << '\n';
        }

        std::cout << "Vulkan initialization successful!\n";

    } catch (const std::exception& e) {
        std::cerr << "Vulkan error: "
                  << e.what() << '\n';

        glfwDestroyWindow(window);
        glfwTerminate();

        return 1;
    }

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
