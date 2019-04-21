#include "VulkanGame.h"
int main() {
	VulkanGame game;
	try {
		game.Init();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}