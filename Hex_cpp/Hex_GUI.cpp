//Code by Christopher Dilday

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>
#include "Hex_GUI.h"
#include <SFML/Graphics.hpp>

using namespace std;

float distance(int x1, int y1, int x2, int y2) {
	return sqrt(((x2 - x1)*(x2 - x1)) + ((y2 - y1)*(y2 - y1)));
}

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
			tempx = event.mouseButton.x;
			tempy = event.mouseButton.y;
			if (board->getPlayer() == '0') {
				//check if inbound of white piece
				if (distance(tempx, tempy, 250, 350) <= 50) {
					board->setPlayer('W');
				}
				else if (distance(tempx, tempy, 600, 350) <= 50) {
					board->setPlayer('B');
				}
			}
			else {
				//check if the reset button has been pressed
				if (distance(tempx, tempy, 675, 125) <= 25) {
					board->resetBoard();
					cout << "Reseting game" << endl;
				}
				else {
					//board clicking
					tempx += 17;
					tempy -= 10;
					y = (int)((float)(tempy / 33));
					x = (tempx - 50 - (20 * y)) / 38;
					//if in bounds and the board is waiting for player input
					if (x >= 0 && x < 11 && y - 1 >= 0 && y - 1 < 11 && board->getWaitingForPlayer() && !board->getIsThinking()) {
						cout << "Row " << y - 1 << ", Col " << x << endl;
						board->aiSetMove(board->getPlayer(), y - 1, x);
						board->setWaitingForPlayer(false);
					}
				}
			}
			break;
		case sf::Event::KeyPressed:
			break;
		default:
			break;
		}
	}
}

void drawPlayerSelect(sf::RenderWindow* window, graph* board) {
	sf::Font font;
	if (!font.loadFromFile("C:/Users/Chris/Documents/Hex_Game/Hex_cpp/arial.ttf"))
	{
		//error for loading fonts here
	}
	//draw instructions
	sf::Text instructText;
	instructText.setFont(font);
	instructText.setString("Select your color. White pieces go first");
	instructText.setCharacterSize(30);
	instructText.setColor(sf::Color::White);
	sf::FloatRect instructTextRect = instructText.getLocalBounds();
	instructText.setOrigin(instructTextRect.left + instructTextRect.width / 2.0f, instructTextRect.top + instructTextRect.height / 2.0f);
	instructText.setPosition(400, 22);
	window->draw(instructText);

	sf::Text instructText2;
	instructText2.setFont(font);
	instructText2.setString("White tries to build a path from West to East\n      Black tries to build North to South");
	instructText2.setCharacterSize(30);
	instructText2.setColor(sf::Color::White);
	sf::FloatRect instructTextRect2 = instructText2.getLocalBounds();
	instructText2.setOrigin(instructTextRect2.left + instructTextRect2.width / 2.0f, instructTextRect2.top + instructTextRect2.height / 2.0f);
	instructText2.setPosition(400, 550);
	window->draw(instructText2);

	//draw pieces that select the color
	sf::CircleShape whitePiece(50);
	whitePiece.setFillColor(sf::Color::White);
	whitePiece.setOutlineColor(sf::Color::Black);
	whitePiece.setOutlineThickness(2);
	whitePiece.setPosition(200, 300);
	window->draw(whitePiece);

	sf::CircleShape blackPiece(50);
	blackPiece.setFillColor(sf::Color::Black);
	blackPiece.setOutlineColor(sf::Color::White);
	blackPiece.setOutlineThickness(2);
	blackPiece.setPosition(550, 300);
	window->draw(blackPiece);

}

void drawBoard(sf::RenderWindow* window, graph* board) {
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
	// reset Button & text
	sf::CircleShape resetButton(25);
	resetButton.setFillColor(sf::Color::Red);
	resetButton.setOutlineColor(sf::Color::White);
	resetButton.setOutlineThickness(2);
	resetButton.setPosition(650, 100);
	window->draw(resetButton);
	sf::Text resetText;
	resetText.setFont(font);
	resetText.setColor(sf::Color::Red);
	resetText.setCharacterSize(20);
	resetText.setPosition(642, 150);
	resetText.setString("RESET");
	window->draw(resetText);

	//now for text that displays the general state of the game
	sf::Text loadText;
	loadText.setFont(font);
	loadText.setColor(sf::Color::White);
	loadText.setCharacterSize(24);
	loadText.setPosition(25, 500);
	//now for states
	if (board->getWinner() != '0') {
		if (board->getWinner() == 'W') {
			loadText.setString("White has made a path from left to right and won!");
			window->draw(loadText);
		}
		else {
			loadText.setString("Blacj has made a path from top to bottom and won!");
			window->draw(loadText);
		}
	}
	else if (board->getIsThinking()) {
		string loadString = "Ai is thinking";
		time_t currTimer;
		int dotNum = time(&currTimer)% 4;
		while(dotNum >= 0){
			loadString += '.';
			dotNum--;
		}
		loadText.setString(loadString);
		window->draw(loadText);
	}
	else if (board->getWaitingForPlayer()) {
		loadText.setString("Click the space you want to move");
		window->draw(loadText);
	}
}

void handleRendering(sf::RenderWindow* window, graph* board) {
	window->clear();
	
	if (board->getPlayer() == '0') {
		drawPlayerSelect(window, board);
	}
	else {
		drawBoard(window, board);
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