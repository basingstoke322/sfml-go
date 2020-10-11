/*
COLORS:
     0. EMPTY
     1. BLACK
     2. WHITE
     3. BOTH
    -1. EDGE OF BOARD
*/
#define LINE_SPACE 25 //width and height of board in px
#define STONE_RADIUS 10
#define BAR_HEIGHT 28
#define DEPTH 3

int BOARD_SIZE, COLOR;

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>

#include "move.hpp"
#include "additionalFunctions.hpp"
#include "board.hpp"
#include "player.hpp"
#include "computerPlayer.hpp"

void initWindow(int &BOARD_SIZE, int &COLOR, sf::Font font)
{
    sf::RenderWindow window(sf::VideoMode(400, 200), "SETTINGS");
    window.setFramerateLimit(30);

    BOARD_SIZE = 9;
    COLOR = 1;

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setString("SIZE: 1. 9x9 2. 13x13 3. 19x19\nCOLOR: 4.BLACK 5.WHITE\n\nPRESS ENTER TO START");
    text.setPosition(0, 0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if ((event.type == sf::Event::Closed)||(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)))
                window.close();
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
                BOARD_SIZE = 9;
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
                BOARD_SIZE = 13;
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
                BOARD_SIZE = 19;
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
                COLOR = 1;
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
                COLOR = 2;
        }
        window.clear(sf::Color::White);
        window.draw(text);
        window.display();
    }
}

void DrawBoardLines(sf::RenderWindow* window, const int WINDOW_SIZE)
{
    for (int i = 0; i < BOARD_SIZE; i++)
        {
            //draw vertical line
            sf::Vertex linev[] =
            {
                sf::Vertex(sf::Vector2f(LINE_SPACE * (i + 1), 0), sf::Color::Black),
                sf::Vertex(sf::Vector2f(LINE_SPACE * (i + 1), WINDOW_SIZE), sf::Color::Black)
            };
            window->draw(linev, 2, sf::Lines);
            //draw horizontal line
            sf::Vertex lineh[] =
            {
                sf::Vertex(sf::Vector2f(0, LINE_SPACE * (i + 1)), sf::Color::Black),
                sf::Vertex(sf::Vector2f(WINDOW_SIZE, LINE_SPACE * (i + 1)), sf::Color::Black)
            };
            window->draw(lineh, 2, sf::Lines);
        }
}

void DrawStones(sf::RenderWindow* window, board* Board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            sf::CircleShape stone(STONE_RADIUS, 32);
            stone.setOutlineThickness(2);
            stone.setPosition(LINE_SPACE * (i + 1) - STONE_RADIUS, LINE_SPACE * (j + 1) - STONE_RADIUS);
            switch (Board->getCell(i, j)->getColor())
            {
                case 0:
                    stone.setFillColor(sf::Color(0,0,0,0));
                    stone.setOutlineColor(sf::Color(0,0,0,0));
                    break;
                case 1:
                    stone.setFillColor(sf::Color::Black);
                    stone.setOutlineColor(sf::Color::Black);
                    break;
                case 2:
                    stone.setFillColor(sf::Color::White);
                    stone.setOutlineColor(sf::Color::Black);
                    break;
            }
            window->draw(stone);
        }
    }
}

void DrawBar(sf::RenderWindow* window, int blackScore, int whiteScore, sf::Font font, const int WINDOW_SIZE)
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(BOARD_SIZE);
    text.setFillColor(sf::Color::Black);

    text.setPosition(0, WINDOW_SIZE);
    text.setString("You: " + std::to_string(blackScore));
    window->draw(text);

    text.setPosition(130, WINDOW_SIZE);
    text.setString("PC: " + std::to_string(whiteScore));
    window->draw(text);

    if(BOARD_SIZE > 9)
    {
        text.setPosition(WINDOW_SIZE - 110, WINDOW_SIZE);
        text.setString("Help: F12");
        window->draw(text);
    }
    
}

int toIndex(int a)
{
    return (a - LINE_SPACE / 2) / LINE_SPACE;
}

void showHelp(sf::Font font)
{
    sf::RenderWindow window(sf::VideoMode(400, 200), "HELP");
    window.setFramerateLimit(10);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setString("This is a Go game program which \nimplements a Tromp-Taylor rules.\nYour goal is to round as\nmuch territory as you can.");
    text.setPosition(0, 0);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::White);
        window.draw(text);
        window.display();
    }
}

bool showWinner(sf::Font font, bool isAIWinner)
{
    bool l = false;
    sf::RenderWindow window(sf::VideoMode(400, 200), "END GAME");
    window.setFramerateLimit(10);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    text.setString(isAIWinner ? "Computer win!!!\nPress Q to quit" : "You win!!!\nPress Q to quit");
    text.setPosition(0, 0);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                l = true;
                window.close();
            }    
        }
        window.clear(sf::Color::White);
        window.draw(text);
        window.display();
    }
    return l;
}

int main()
{
    //setting font
    sf::Font font;
    font.loadFromFile("font.otf");

    initWindow(BOARD_SIZE, COLOR, font);
	board Board(BOARD_SIZE);
	player Player(COLOR);
	computerPlayer AI(getOppositeColor(COLOR));
	int i, j, passes = 0;
    bool f = (AI.getPlayerColor() == 1);

	const int WINDOW_SIZE = LINE_SPACE * (BOARD_SIZE + 1);
    const int PADDING = LINE_SPACE - STONE_RADIUS;

    //creating window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE + BAR_HEIGHT), "Go", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(30);

    //setting sound
    sf::SoundBuffer buffer;
    sf::Sound sound;
    buffer.loadFromFile("sound.wav");
    sound.setBuffer(buffer);
    sound.setVolume(50);
    sound.setPitch(1);

    srand(time(0));
    
    //main gameloop
    while (window.isOpen())
    {
        sf::Event event;
        if (f) //bot is black
        {
            move mov, 
            mov1 = AI.deepMoveSearch(DEPTH, AI.getPlayerColor(), AI.getPlayerColor(), -32000, 32000, &Board),
            mov2 = AI.shortMoveSearch(AI.getPlayerColor(), &Board);
            mov = mov1.score >= mov2.score ? mov1 : mov2;
            AI.makeMove(mov.i, mov.j, &Board);
            f = false;
        }
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                i = toIndex(localPosition.x);
                j = toIndex(localPosition.y);
                if (Board.isFree(i, j))
                {                    
                	Player.makeMove(i, j, &Board);
                    sound.play();
                    move mov, 
                    mov1 = AI.deepMoveSearch(DEPTH, AI.getPlayerColor(), AI.getPlayerColor(), -32000, 32000, &Board),
                    mov2 = AI.shortMoveSearch(AI.getPlayerColor(), &Board);
                    mov = mov1.score >= mov2.score ? mov1 : mov2;
                    if(Board.isFree(mov.i,mov.j))
                        AI.makeMove(mov.i, mov.j, &Board);
                }                      
            } 
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            {
                passes++;
                sound.play();
                if (AI.getPoints() > Player.getPoints())
                    passes ++;
                else
                {
                    move mov, 
                    mov1 = AI.deepMoveSearch(DEPTH, AI.getPlayerColor(), AI.getPlayerColor(), -32000, 32000, &Board),
                    mov2 = AI.shortMoveSearch(AI.getPlayerColor(), &Board);
                    mov = mov1.score >= mov2.score ? mov1 : mov2;
                    if(mov.score > AI.getPoints())
                        AI.makeMove(mov.i, mov.j, &Board);
                    else
                        passes++;
                }            
            }
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::F12))
            {
                showHelp(font);
            }
            if(passes == 2)
            {
                if (showWinner(font, AI.getPoints() > Player.getPoints()))
                    window.close();
                passes = 0;
            }
            passes = 0;
        } 
        
        //drawing some stuff
        window.clear(sf::Color::White);
        DrawBoardLines(&window, WINDOW_SIZE);
        DrawStones(&window, &Board);
        DrawBar(&window, Player.getPoints(), AI.getPoints(), font, WINDOW_SIZE);
           
        window.display();
    }

	return 0;
}