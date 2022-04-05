#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::string;

const int WIDTH = 15;
const int HEIGHT = 15;
const vector<int> snakeStartCoords = {WIDTH / 2, HEIGHT / 2};

#define RESETCOLOR "\033[0m"
#define GREEN "\033[22;32m"
#define RED "\033[22;31m"

class Fruit
{
public:
    vector<int> coords;
    Fruit()
    {
        getNewCoords({snakeStartCoords});
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
    int direction = 0;  // 0 = STOP, 1 = LEFT, 2 = RIGHT, 3 = UP, 4 = DOWN

    Snake()
    {
        coords.push_back(snakeStartCoords);
    }

    void move()  // Move the snake
    {
        switch (direction)
        {
            case 1:
                coords.push_back({coords[coords.size() - 1][0] - 1, coords[coords.size() - 1][1]});
                break;
            
            case 2:
                coords.push_back({coords[coords.size() - 1][0] + 1, coords[coords.size() - 1][1]});
                break;
            
            case 3:
                coords.push_back({coords[coords.size() - 1][0], coords[coords.size() - 1][1] - 1});
                break;
            
            case 4:
                coords.push_back({coords[coords.size() - 1][0], coords[coords.size() - 1][1] + 1});
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
            switch (_getch())
            {
                case 'a':  // Left
                    if (snake.direction != 2)  // Prevents the snake from turning around
                        snake.direction = 1;
                    break;
                
                case 'd': // Right
                    if (snake.direction != 1)
                        snake.direction = 2;
                    break;
                
                case 'w': // Up
                    if (snake.direction != 4)
                        snake.direction = 3;
                    break;
                
                case 's': // Down
                    if (snake.direction != 3)
                        snake.direction = 4;
                    break;

                case 'x':
                    exit(0);
                    break;
            }
        }
    }

    void printBoard()
    {
        system("cls");

        // Print the top border
        for (int x = 0; x < WIDTH * 2 + 3; x++)
            cout << "#";
        cout << "\n";

        // Iterate through all positions on the board
        for (int y = 0; y < HEIGHT; y++)
        {
            for (int x = -1; x <= WIDTH; x++)
            {
                // Print the left and right border
                if (x == -1 || x == WIDTH)
                    cout << "#";
                
                // Print the snake head
                else if (vector<int> {x, y} == snake.coords.back())
                    cout << GREEN << "O" << RESETCOLOR;
                
                // Print the snake body
                else if (vector<int> {x, y} == fruit.coords)
                    cout << RED << "*" << RESETCOLOR;

                else
                {
                    // Iterate through the snake parts to see if the position is in the body
                    bool snakePart = false;

                    for (vector<int> coord : snake.coords)
                    {
                        if (vector<int> {x, y} == coord)
                        {
                            cout << GREEN << "o" << RESETCOLOR;
                            snakePart = true;
                            break;
                        }
                    }
                    
                    // If the position is not in the body, print a blank space
                    if (!snakePart)
                        cout << " ";
                }   
                cout << " ";  // Make the x evenly spaced with the y
            }
            cout << "\n";
        }

        // Print the bottom border
        for (int x = 0; x < WIDTH * 2 + 3; x++)
            cout << "#";
        cout << "\n";

        cout << "Score: " << snake.coords.size() - 1 << "\n";
    }

    void runGame()
    {
        while (!gameOver)
        {
            printBoard(); 

            input();
            snake.move();

            if (snake.coords.back() != fruit.coords && snake.direction != 0)
            {
                snake.shrink();
            }
            
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

            Sleep(75); // Sleep for 100ms to make the game run at a reasonable speed
        }   
    }
};

int main()
{
    while (true)
    {
        Game game;
        game.runGame();

        cout << "Play again? (y/n): ";
        char playAgain;
        cin >> playAgain;

        if (playAgain == 'n')
            break;
        
        else if (playAgain == 'y')
            continue;
    }
    return 0;
}
