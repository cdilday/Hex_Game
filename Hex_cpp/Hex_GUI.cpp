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
			if (board->getPlayer() == '0') {
				//check if inbound of white piece
				if (tempy > 300 && tempy < 400) {
					cout << "In y bounds" << endl;
					if (tempx > 200 && tempx < 300)
						board->setPlayer('W');
					else if (tempx > 550 && tempx < 650)
						board->setPlayer('B');
				}
			}
			else {
				y = (int)((float)(tempy / 33));
				x = (tempx - 50 - (20 * y)) / 38;
				cout << "Row " << y - 1 << ", Col " << x << endl;
				//if in bounds and the board is waiting for player input
				if (x >= 0 && x < 11 && y - 1 >= 0 && y - 1 < 11 && board->getWaitingForPlayer() && !board->getIsThinking()) {
					board->aiSetMove(board->getPlayer(), y - 1, x);
					board->setWaitingForPlayer(false);
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