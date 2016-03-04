#include "Hex_Graph.h"

using namespace std;

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL");

	// deactivate its OpenGL context
	window.setActive(false);

	srand(time(0));
	graph test;

	// launch the rendering thread
	sf::Thread renderThread(bind(&renderingThread, &window, &test));
	renderThread.launch();

	playAIGame(test);
	system("PAUSE");
	return 0;
}