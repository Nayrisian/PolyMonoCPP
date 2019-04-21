#pragma once
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#ifndef NDEBUG
#define Log(...) {\
    char str[100];\
    snprintf(str, 100, __VA_ARGS__);\
    std::cout << "[" << __FILENAME__ << "][" << __FUNCTION__ << "][Line " << __LINE__ << "] " << str << std::endl;\
    }
#else
#define Log(...)
#endif
#define VK_CHECK_RESULT(f)																				\
{																										\
	VkResult res = (f);																					\
	if (res != VK_SUCCESS)																				\
	{																									\
		std::cout << "Fatal : VkResult is \"" << res << "\" in " << __FILE__ << " at line " << __LINE__ << std::endl; \
		assert(res == VK_SUCCESS);																		\
	}																									\
}