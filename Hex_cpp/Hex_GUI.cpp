//Code by Christopher Dilday

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "Hex_GUI.h"
#include <SFML/Graphics.hpp>

using namespace std;

void handleEvents(sf::RenderWindow* window, graph* board) {
	sf::Event event; 
	int x, y, tempx, tempy;
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window->close();
			return;
		case sf::Event::MouseButtonPressed:
			//cout << "Mouse pressed at " << event.mouseButton.x << ", " << event.mouseButton.y << endl;
			tempx = event.mouseButton.x + 17;
			tempy = event.mouseButton.y - 10;
			y = (int)((float)(tempy / 33));
			x = (tempx - 50 - (20 * y)) / 38;
			cout << "Row " << y-1 << ", Col " << x << endl;
			//if in bounds and the board is waiting for player input
			if (x >= 0 && x < 11 && y-1>= 0 && y-1 < 11 && board->getWaitingForPlayer() && !board->getIsThinking()) {
				board->aiSetMove('W', y-1, x);
				board->setWaitingForPlayer(false);
			}
			cout << board->getWaitingForPlayer();
			break;
		case sf::Event::KeyPressed:
			break;
		default:
			break;
		}
	}
}

void handleRendering(sf::RenderWindow* window, graph* board) {
	window->clear();
	//load font stuff
	sf::Font font;
	if (!font.loadFromFile("C:/Users/Chris/Documents/Hex_Game/Hex_cpp/arial.ttf"))
	{
		//error for loading fonts here
	}

	//draw board, start with column numbers
	for (int c = 0; c < 11; c++) {
		sf::Text colNum;
		colNum.setFont(font);
		colNum.setString(to_string(c + 1));
		colNum.setColor(sf::Color::White);
		colNum.setCharacterSize(24);
		//get information to center properly regardless of string size
		sf::FloatRect textRect = colNum.getLocalBounds();
		colNum.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		colNum.setPosition(68 + (c * 38), 22);
		window->draw(colNum);
	}
	//now the actual board
	for (int r = 0; r < 11; r++) {
		//row's number
		sf::Text rowNum;
		rowNum.setFont(font);
		rowNum.setString(to_string(r + 1));
		rowNum.setColor(sf::Color::White);
		rowNum.setCharacterSize(24);
		//get information to center properly regardless of string size
		sf::FloatRect textRect = rowNum.getLocalBounds();
		rowNum.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
		rowNum.setPosition(35 + (r * 20), 60 + (r * 33));
		window->draw(rowNum);
		//this row's hexagons
		for (int c = 0; c < 11; c++) {
			sf::CircleShape hexagon(20, 6);
			hexagon.setPosition(50 + (c * 38) + (r * 20), 40 + (r * 33));
			hexagon.setOutlineThickness(2);
			hexagon.setOutlineColor(sf::Color(150, 150, 150));
			window->draw(hexagon);
			//now check if a piece has been placed on it
			if (board->getEntryAt(r, c) == 'W') {
				//place white piece
				sf::CircleShape whitePiece(14);
				whitePiece.setFillColor(sf::Color::White);
				whitePiece.setOutlineColor(sf::Color::Black);
				whitePiece.setOutlineThickness(2);
				whitePiece.setPosition(56 + (c * 38) + (r * 20), 46 + (r * 33));
				window->draw(whitePiece);
			}
			else if (board->getEntryAt(r, c) == 'B') {
				sf::CircleShape blackPiece(14);
				blackPiece.setFillColor(sf::Color::Black);
				blackPiece.setOutlineColor(sf::Color::Black);
				blackPiece.setOutlineThickness(2);
				blackPiece.setPosition(56 + (c * 38) + (r * 20), 46 + (r * 33));
				window->draw(blackPiece);
			}
		}
	}
	//end frame
	window->display();
}

void GUIHandler(sf::RenderWindow* window, graph* board){
	while (window->isOpen()) {
		
		handleEvents(window, board);
		handleRendering(window, board);
	}
}