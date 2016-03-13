#include "Hex_Graph.h"
#include <SFML/Graphics.hpp>

float distance(int, int, int, int);

//responsible for the vent loop and what to do in reaction to events
void handleEvents(sf::RenderWindow* window, graph* board);

//holds all code for drawing in the window
void handleRendering(sf::RenderWindow* window, graph* board);

//main loop for SFML related window processes. This runs all the related handlers
void GUIHandler(sf::RenderWindow* window, graph* board);

