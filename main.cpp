#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::string;

#define RESETCOLOR "\033[0m"
#define GREEN "\033[22;32m"
#define RED "\033[22;31m"

const int WIDTH = 15;
const int HEIGHT = 15;
const vector<vector<int>> snakeStartCoords = { 
    {(WIDTH / 2) - 1, HEIGHT / 2},
    {WIDTH / 2, HEIGHT / 2}
};

enum eDirection {UP, DOWN, LEFT, RIGHT};

class Fruit
{
public:
    vector<int> coords;
    Fruit()
    {
        getNewCoords(snakeStartCoords);
    }

    void getNewCoords(vector<vector<int>> snakeCoords)  // Gets new fruit coords
    {
        coords.clear();
        coords = {rand() % WIDTH, rand() % HEIGHT};

        for (vector<int> snakePart : snakeCoords)
        {
            if (coords == snakePart)  // Get new coords if the apple is inside the snake
                getNewCoords(snakeCoords);
        }
    }
};

class Snake
{
public:
    vector<vector<int>> coords;
    eDirection direction;
    bool stopped;

    Snake()
    {
        direction = RIGHT;
        stopped = true;
        coords = snakeStartCoords;
    }

    void move()  // Move the snake
    {
        if (stopped)
            return;
        
        switch (direction)
        {
            case LEFT:
                coords.push_back({coords.back()[0] - 1, coords.back()[1]});
                break;
            
            case RIGHT:
                coords.push_back({coords.back()[0] + 1, coords.back()[1]});
                break;
            
            case UP:
                coords.push_back({coords.back()[0], coords.back()[1] - 1});
                break;
            
            case DOWN:
                coords.push_back({coords.back()[0], coords.back()[1] + 1});
                break;
        }
    }

    void shrink()
    {
        coords.erase(coords.begin());
    }

    bool loseCheck()
    {
        // Check if the head is out of the map
        if (coords.back()[0] < 0 || coords.back()[0] >= WIDTH || coords.back()[1] < 0 || coords.back()[1] >= HEIGHT)
        {
            return true;
        }

        else
        {
            // Iterate through the body of the snake to see if the head is in the body
            for (int i = 0; i < coords.size() - 1; i++)
            {
                if (coords.back()[0] == coords[i][0] && coords.back()[1] == coords[i][1])
                {
                    return true;
                }
            }
        }
        return false;
    }
    bool winCheck()
    {
        return coords.size() == WIDTH * HEIGHT;
    }
};

class Game
{
public:
    Snake snake;
    Fruit fruit;
    
    bool gameOver = false;

    void input()
    {
        if (_kbhit())
        {
            if (snake.stopped)
                snake.stopped = false;

            switch (_getch())
            {
                case 'a':  // Left
                    if (snake.direction != RIGHT)  // Prevents the snake from turning around
                        snake.direction = LEFT;
                    break;
                
                case 'd': // Right
                    if (snake.direction != LEFT)
                        snake.direction = RIGHT;
                    break;
                
                case 'w': // Up
                    if (snake.direction != DOWN)
                        snake.direction = UP;
                    break;
                
                case 's': // Down
                    if (snake.direction != UP)
                        snake.direction = DOWN;
                    break;

                case 'x':
                    exit(0);
                    break;
            }
        }
    }

    void printBoard()
    {

        vector<string> board = {""};

        // Print the top border
        for (int x = 0; x < WIDTH * 2 + 2; x++)
            board[0] += "#";

        // Iterate through all positions on the board
        for (int y = 0; y < HEIGHT; y++)
        {
            board.push_back("");
            // Print the left border
            board[y + 1] += "#";
            for (int x = 0; x < WIDTH; x++)
            {
                // Print the snake head
                if (vector<int> {x, y} == snake.coords.back())
                    board[y + 1] += string(GREEN) + "O" + string(RESETCOLOR);
                
                // Print the snake body
                else if (vector<int> {x, y} == fruit.coords)
                    board[y + 1] += string(RED)+ "*" + string(RESETCOLOR);

                else
                {
                    // Iterate through the snake parts to see if the position is in the body
                    bool snakePart = false;

                    for (vector<int> coord : snake.coords)
                    {
                        if (vector<int> {x, y} == coord)
                        {
                            board[y + 1] += string(GREEN) + "o" + string(RESETCOLOR);
                            snakePart = true;
                            break;
                        }
                    }
                    
                    // If the position is not in the body, print a blank space
                    if (!snakePart)
                        board[y + 1] += " ";
                }   
                board[y + 1] += " ";  // Make the x evenly spaced with the y
            }
            // Print the right border and a newline
            board[y + 1] += "#";
        }

        board.push_back("");

        // Print the bottom border
        for (int x = 0; x < WIDTH * 2 + 2; x++)
            board[board.size() - 1] += "#";

        // Print the board
        system("cls");  // Windows only
        for (string line : board)
            cout << line << "\n";

        // Print more information
        cout << "Score: " << snake.coords.size() * 10 << "\n";
        
        if (snake.stopped)
            cout << GREEN << "Press WASD to start moving." << RESETCOLOR;
    }

    void runGame()
    {
        while (!gameOver)
        {
            printBoard(); 

            input();
            snake.move();

            if (snake.coords.back() != fruit.coords && !snake.stopped)
                snake.shrink();
            
            else if (snake.winCheck())  // Win check
            {
                cout << "You Win!\n";
                gameOver = true;
            }

            else if (snake.coords.back() == fruit.coords)  // If the snake eats the
                fruit.getNewCoords(snake.coords);

            if (snake.loseCheck() && !gameOver)  // Lose check
            {
                cout << "You Lose!\n";
                gameOver = true;
            }

            Sleep(75); // Sleep for n ms to make the game run at a reasonable speed
        }   
    }
};

bool playAgain()
{
    char input;
    cout << "Play again? (y/n): ";
    cin >> input;

    if (input == 'y' || input == 'Y')
        return true;
    
    else if (input == 'n' || input == 'N')
        return false;
    
    else
    {
        cout << RED << "Invalid input.\n" << RESETCOLOR;
        return playAgain();
    }
}

int main()
{
    while (true)
    {
        Game game;
        game.runGame();

        if (!playAgain())
            break;
    }
    return 0;
}
