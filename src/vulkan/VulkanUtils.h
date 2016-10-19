#pragma once

#include <vector>
#include <string>
#include <stdexcept>

#include "VulkanFwd.h"

using namespace std;
namespace vulkan
{
    template<typename E, typename R>
    vector<R> getVector(
        VkResult(*func)(E, uint32_t*, R*),
        E entity, string errorMessage)
    {
        uint32_t count;
        if(func(entity, &count, nullptr) != VK_SUCCESS) {
            throw new runtime_error(errorMessage);
        }
        vector<R> result(count);
        if(func(entity, &count, result.data()) != VK_SUCCESS) {
            throw new runtime_error(errorMessage);
        }
        return result;
    }

    template<typename E1, typename E2, typename R>
    vector<R> getVector(
        VkResult(*func)(E1, E2, uint32_t*, R*),
        E1 entity1, E2 entity2, string errorMessage)
    {
        uint32_t count;
        if(func(entity1, entity2, &count, nullptr) != VK_SUCCESS) {
            throw new runtime_error(errorMessage);
        }
        vector<R> result(count);
        if(func(entity1, entity2, &count, result.data()) != VK_SUCCESS) {
            throw new runtime_error(errorMessage);
        }
        return result;
    }

    template<typename E, typename R>
    vector<R> getVector(void(*func)(E, uint32_t*, R*), E entity)
    {
        uint32_t count;
        func(entity, &count, nullptr);
        vector<R> result(count);
        func(entity, &count, result.data());
        return result;
    }
}
