#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <ctime>
#include <cstdlib>
#include <deque>
#include <string>
#include <iostream>

int CELL_SIZE = 24;
int GRID_WIDTH = 24;
int GRID_HEIGHT = 24;

class SnakeGame {
    
public:
    SnakeGame() {
        srand(static_cast<unsigned>(time(NULL)));
        window.create(sf::VideoMode(900, 900), "Snake Game");
        
        window.setFramerateLimit(10);
        
        // Initialize the snake with grid coordinates
        directions.push_front(sf::Vector2i(1, 0));
        directions.push_front(sf::Vector2i(1, 0));
        snake.push_front(sf::Vector2i(5, 5));
        snake.push_front(sf::Vector2i(6, 5));
        
        generateFood();
        
        if (!font.loadFromFile("ArialBold.ttf")) {
            std::cout << "Error loading font" << std::endl;
        }
        apple.loadFromFile("apple2.png");
        appleSprite.setTexture(apple);
        headUpTexture.loadFromFile("headUp.png");
        headLeftTexture.loadFromFile("headLeft.png");
        headRightTexture.loadFromFile("headRight.png");
        headDownTexture.loadFromFile("headDown.png");
        
        headUpRightTexture.loadFromFile("headUpRight.png");
        headUpLeftTexture.loadFromFile("headUpLeft.png");
        headDownRightTexture.loadFromFile("headDownRight.png");
        headDownLeftTexture.loadFromFile("headDownLeft.png");
        
        bodyUpTexture.loadFromFile("bodyUp.png");
        bodyHorizTexture.loadFromFile("bodyHoriz.png");
        
        tailUpTexture.loadFromFile("tailUp.png");
        tailLeftTexture.loadFromFile("tailLeft.png");
        tailRightTexture.loadFromFile("tailRight.png");
        tailDownTexture.loadFromFile("tailDown.png");
        
        score = 0;
        eatenFoodCount = 0;
        
        // Dynamically calculate grid dimensions
        int frameWidth = window.getSize().x * 0.8;  // Example: Rectangle takes 80% of window width
        int frameHeight = window.getSize().y * 0.8; // Example: Rectangle takes 80% of window height
        GRID_WIDTH = frameWidth / CELL_SIZE;
        GRID_HEIGHT = frameHeight / CELL_SIZE;

        // Set the rectangle to match the grid size
        frame.setSize(sf::Vector2f(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE));
        frame.setOutlineThickness(2.0f);
        frame.setOutlineColor(sf::Color::White);
        frame.setFillColor(sf::Color::Transparent);

        // Center the rectangle in the window
        float frameX = (window.getSize().x - frame.getSize().x) / 2.0f;
        float frameY = (window.getSize().y - frame.getSize().y) / 2.0f;
        frame.setPosition(frameX, frameY);

    }
    
    void run() {
        sf::Time deltaTime;

        while (window.isOpen()) {
            deltaTime = clock.restart();
            timeSinceLastMove += deltaTime;

            proccessEvents();

            window.clear(sf::Color::Black);

            if (isStartScreen) {
                renderStartScreen();
            } else if (isGameOver) {
                renderGameOverScreen();
            } else {
                if (timeSinceLastMove >= timePerMove) {
                    update();
                    timeSinceLastMove = sf::Time::Zero;
                }
                render();
            }

            window.display();
        }
    }
    
private:
    sf::RenderWindow window;
    std::deque<sf::Vector2i> snake;
    sf::Vector2i food;
    sf::Vector2i direction = {1, 0};
    std::deque<sf::Vector2i> directions;
    
    bool isGameOver = false;
    
    bool isStartScreen = true;
    
    sf::Clock clock;
    sf::Time timePerMove = sf::seconds(0.2f);
    sf::Time timeSinceLastMove = sf::Time::Zero;
    
    sf::Texture apple;
    sf::Sprite appleSprite;
    
    sf::Texture headUpTexture;
    sf::Texture headLeftTexture;
    sf::Texture headRightTexture;
    sf::Texture headDownTexture;
    
    sf::Texture headUpRightTexture;
    sf::Texture headUpLeftTexture;
    sf::Texture headDownRightTexture;
    sf::Texture headDownLeftTexture;
    
    sf::Texture bodyUpTexture;
    sf::Texture bodyHorizTexture;
    
    sf::Texture tailUpTexture;
    sf::Texture tailRightTexture;
    sf::Texture tailLeftTexture;
    sf::Texture tailDownTexture;
    
    sf::Font font;
    int score;
    int eatenFoodCount;
    bool isTongueVisible = false;
    
    sf::RectangleShape startButton;
    sf::Text startButtonText;

    sf::RectangleShape restartButton;
    sf::Text restartButtonText;
    
    sf::RectangleShape frame;
    
    void proccessEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if (isStartScreen && event.type == sf::Event::MouseButtonPressed) {
                if (startButton.getGlobalBounds().contains(mousePos)) {
                    isStartScreen = false;
                }
            }

            if (isGameOver && event.type == sf::Event::MouseButtonPressed) {
                if (restartButton.getGlobalBounds().contains(mousePos)) {
                    restartGame();
                }
            }

            if (!isStartScreen && !isGameOver && event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up && direction.y == 0) {
                    direction = {0, -1};
                } else if (event.key.code == sf::Keyboard::Down && direction.y == 0) {
                    direction = {0, 1};
                } else if (event.key.code == sf::Keyboard::Left && direction.x == 0) {
                    direction = {-1, 0};
                } else if (event.key.code == sf::Keyboard::Right && direction.x == 0) {
                    direction = {1, 0};
                }
            }
        }
    }

    void update() {
        if (isGameOver) {
            return;
        }

        // Calculate new head position
        sf::Vector2i newHead = snake.front() + direction;

        // Get frame boundaries
        sf::Vector2f gridOffset(frame.getPosition());
        float frameRight = gridOffset.x + frame.getSize().x;
        float frameBottom = gridOffset.y + frame.getSize().y;

        if (newHead.x * CELL_SIZE + gridOffset.x < gridOffset.x ||       // Left boundary
            newHead.x * CELL_SIZE + gridOffset.x >= frameRight ||        // Right boundary
            newHead.y * CELL_SIZE + gridOffset.y < gridOffset.y ||       // Top boundary
            newHead.y * CELL_SIZE + gridOffset.y >= frameBottom ||       // Bottom boundary
            std::find(snake.begin(), snake.end(), newHead) != snake.end()) { // Self-collision
            std::cout << "Collision detected! Game over.\n";
            isGameOver = true;
            return;
        }

        snake.push_front(newHead);
        directions.push_front(direction);

        if (newHead == food) {
            generateFood();
            score += 10 + eatenFoodCount;
            eatenFoodCount++;
        } else {
            snake.pop_back(); // Remove tail if food not eaten
            directions.pop_back();
        }
    }

    void renderStartScreen() {
        int windowWidth = window.getSize().x;
        int windowHeight = window.getSize().y;

        // Render start screen background
        sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
        overlay.setFillColor(sf::Color(0, 0, 0, 150)); // Semi-transparent black
        window.draw(overlay);

        int popUpWidth = 400;
        int popUpHeight = 400;
        sf::RectangleShape popupBackground(sf::Vector2f(popUpWidth, popUpHeight));
        popupBackground.setFillColor(sf::Color(50, 50, 50, 240));
        popupBackground.setPosition((windowWidth - popUpWidth) / 2, (windowHeight - popUpHeight) / 2);
        window.draw(popupBackground);

        // Title
        sf::Text titleText;
        titleText.setFont(font);
        titleText.setString("Snake Game :)");
        titleText.setCharacterSize(36);
        titleText.setFillColor(sf::Color::White);
        titleText.setStyle(sf::Text::Bold);
        titleText.setPosition(
            popupBackground.getPosition().x + (popUpWidth - titleText.getLocalBounds().width) / 2,
            popupBackground.getPosition().y + 50
        );
        window.draw(titleText);

        // Start Button
        startButton.setSize(sf::Vector2f(200, 50));
        startButton.setFillColor(sf::Color::Green);
        startButton.setPosition(
            popupBackground.getPosition().x + (popUpWidth - startButton.getSize().x) / 2,
            popupBackground.getPosition().y + 250
        );
        window.draw(startButton);

        // Start Button Text
        startButtonText.setFont(font);
        startButtonText.setString("Start Game");
        startButtonText.setCharacterSize(24);
        startButtonText.setFillColor(sf::Color::Black);
        startButtonText.setPosition(
            startButton.getPosition().x + (startButton.getSize().x - startButtonText.getLocalBounds().width) / 2,
            startButton.getPosition().y + (startButton.getSize().y - startButtonText.getLocalBounds().height) / 2 - 5
        );
        window.draw(startButtonText);
    }

    void renderGameOverScreen() {
        int windowWidth = window.getSize().x;
        int windowHeight = window.getSize().y;

        sf::RectangleShape overlay(sf::Vector2f(windowWidth, windowHeight));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        int popUpWidth = 400;
        int popUpHeight = 400;
        sf::RectangleShape popupBackground(sf::Vector2f(popUpWidth, popUpHeight));
        popupBackground.setFillColor(sf::Color(50, 50, 50, 240));
        popupBackground.setPosition((windowWidth - popUpWidth) / 2, (windowHeight - popUpHeight) / 2);
        window.draw(popupBackground);

        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setString("Game Over :(");
        gameOverText.setCharacterSize(48);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setStyle(sf::Text::Bold);
        gameOverText.setPosition(
            popupBackground.getPosition().x + (popUpWidth - gameOverText.getLocalBounds().width) / 2,
            popupBackground.getPosition().y + 50
        );
        window.draw(gameOverText);

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(
            popupBackground.getPosition().x + (popUpWidth - scoreText.getLocalBounds().width) / 2,
            popupBackground.getPosition().y + 130
        );
        window.draw(scoreText);

        sf::Text applesText;
        applesText.setFont(font);
        applesText.setString("Apples: " + std::to_string(eatenFoodCount));
        applesText.setCharacterSize(24);
        applesText.setFillColor(sf::Color::White);
        applesText.setPosition(
            popupBackground.getPosition().x + (popUpWidth - applesText.getLocalBounds().width) / 2,
            popupBackground.getPosition().y + 180
        );
        window.draw(applesText);

        restartButton.setSize(sf::Vector2f(200, 50));
        restartButton.setFillColor(sf::Color::Green);
        restartButton.setPosition(
            popupBackground.getPosition().x + (popUpWidth - restartButton.getSize().x) / 2,
            popupBackground.getPosition().y + 250
        );
        window.draw(restartButton);

        restartButtonText.setFont(font);
        restartButtonText.setString("Restart Game");
        restartButtonText.setCharacterSize(24);
        restartButtonText.setFillColor(sf::Color::Black);
        restartButtonText.setPosition(
            restartButton.getPosition().x + (restartButton.getSize().x - restartButtonText.getLocalBounds().width) / 2,
            restartButton.getPosition().y + (restartButton.getSize().y - restartButtonText.getLocalBounds().height) / 2 - 5
        );
        window.draw(restartButtonText);
    }

    void restartGame() {
        snake.clear();
        directions.clear();

        snake.push_front(sf::Vector2i(5, 5));
        snake.push_front(sf::Vector2i(6, 5));
        direction = {1, 0};
        directions.push_front(direction);
        directions.push_front(direction);

        score = 0;
        eatenFoodCount = 0;

        generateFood();

        isGameOver = false;
        isStartScreen = false;
    }

    void render() {
        
        window.draw(frame);
        
        sf::Vector2f gridOffset(frame.getPosition());
        
        appleSprite.setPosition(
                gridOffset.x + food.x * CELL_SIZE, // X position in pixels
                gridOffset.y + food.y * CELL_SIZE  // Y position in pixels
            );
            window.draw(appleSprite);

        for (int i = 0; i < snake.size(); i++) {
            sf::Sprite sprite;

            if (i == 0) {
                if (direction.y == -1) {
                    sprite.setTexture(headUpTexture);
                } else if (direction.y == 1) {
                    sprite.setTexture(headDownTexture);
                } else if (direction.x == 1) {
                    sprite.setTexture(headRightTexture);
                } else if (direction.x == -1) {
                    sprite.setTexture(headLeftTexture);
                }
            } else if (i == snake.size() - 1) {
                sf::Vector2i tailDirection = snake[snake.size() - 2] - snake[snake.size() - 1];
                if (tailDirection.y == -1) {
                    sprite.setTexture(tailUpTexture);
                } else if (tailDirection.y == 1) {
                    sprite.setTexture(tailDownTexture);
                } else if (tailDirection.x == 1) {
                    sprite.setTexture(tailRightTexture);
                } else if (tailDirection.x == -1) {
                    sprite.setTexture(tailLeftTexture);
                }
            } else {
                sf::Vector2i prevDirection = directions[i];
                sf::Vector2i nextDirection = directions[i - 1]; // eg current, to detect turn pos
                
                if (prevDirection != nextDirection) {  // Turn part of body
                    if (prevDirection == sf::Vector2i(0, -1) && nextDirection == sf::Vector2i(1, 0)) {
                        sprite.setTexture(headUpRightTexture);
                    } else if (prevDirection == sf::Vector2i(0, -1) && nextDirection == sf::Vector2i(-1, 0)) {
                        sprite.setTexture(headUpLeftTexture);
                    } else if (prevDirection == sf::Vector2i(0, 1) && nextDirection == sf::Vector2i(1, 0)) {
                        sprite.setTexture(headDownRightTexture);
                    } else if (prevDirection == sf::Vector2i(0, 1) && nextDirection == sf::Vector2i(-1, 0)) {
                        sprite.setTexture(headDownLeftTexture);
                    } else if (prevDirection == sf::Vector2i(1, 0) && nextDirection == sf::Vector2i(0, -1)) {
                        sprite.setTexture(headDownLeftTexture);
                    } else if (prevDirection == sf::Vector2i(1, 0) && nextDirection == sf::Vector2i(0, 1)) {
                        sprite.setTexture(headUpLeftTexture);
                    } else if (prevDirection == sf::Vector2i(-1, 0) && nextDirection == sf::Vector2i(0, -1)) {
                        sprite.setTexture(headDownRightTexture);
                    } else if (prevDirection == sf::Vector2i(-1, 0) && nextDirection == sf::Vector2i(0, 1)) {
                        sprite.setTexture(headUpRightTexture);
                    }
                } else {
                    if (prevDirection.x == 1 || prevDirection.x == -1) {
                        sprite.setTexture(bodyHorizTexture);
                    } else if (prevDirection.y == 1 || prevDirection.y == -1) {
                        sprite.setTexture(bodyUpTexture);
                    }
                }
            }

            sf::Vector2f gridOffset(frame.getPosition());
            
            sprite.setPosition(gridOffset.x + snake[i].x * CELL_SIZE, gridOffset.y + snake[i].y * CELL_SIZE);
            window.draw(sprite);
        }

        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + std::to_string(score));
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(90, 10);
        window.draw(scoreText);

        sf::Text foodCountText;
        foodCountText.setFont(font);
        foodCountText.setString("Apples: " + std::to_string(eatenFoodCount));
        foodCountText.setCharacterSize(24);
        foodCountText.setFillColor(sf::Color::White);
        foodCountText.setPosition(250, 10);
        window.draw(foodCountText);
    }
    
    
    void generateFood() {
        sf::Vector2f gridOffset = frame.getPosition();

        do {
            food = {
                rand() % GRID_WIDTH,
                rand() % GRID_HEIGHT
            };
        } while (std::find(snake.begin(), snake.end(), food) != snake.end());

        food.x = food.x;
        food.y = food.y;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}

