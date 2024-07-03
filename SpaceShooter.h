#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <thread>

class Sprite {
protected:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    virtual void move(sf::Vector2f offset) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~Sprite() {}
};

class Spaceship : public Sprite {
public:
    virtual void move(sf::Vector2f offset) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~Spaceship() {}
};

class PlayerSpaceShip : public Spaceship {
public:
    virtual void move(sf::Vector2f offset) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool contains(sf::Vector2f point) { return false; }
    virtual ~PlayerSpaceShip() {}

    float getPositionX();
    float getPositionY();
    float getWidth();
    sf::FloatRect getBounds();
};

class OrangeSpaceShip : public PlayerSpaceShip {
public:
    OrangeSpaceShip();
    void move(sf::Vector2f offset);
    void scale(sf::Vector2f offset);
    void draw(sf::RenderWindow& window);
    bool contains(sf::Vector2f point);
};

class WhiteSpaceShip : public PlayerSpaceShip {
public:
    WhiteSpaceShip();
    void move(sf::Vector2f offset);
    void scale(sf::Vector2f offset);
    void draw(sf::RenderWindow& window);
    bool contains(sf::Vector2f point);
};

class RedSpaceShip : public PlayerSpaceShip {
public:
    RedSpaceShip();
    void move(sf::Vector2f offset);
    void scale(sf::Vector2f offset);
    void draw(sf::RenderWindow& window);
    bool contains(sf::Vector2f point);
};

class EnemySpaceShip : public PlayerSpaceShip {
private:
    bool alive;

public:
    EnemySpaceShip();
    void spawn(sf::Vector2f offset);
    void move(sf::Vector2f offset);
    void draw(sf::RenderWindow& window);
    bool isAlive();
    void destroy();
    float getPositionY();
    float getPositionX();
};

class Bullet : public Sprite {
public:
    Bullet();
    void move(sf::Vector2f offset);
    void draw(sf::RenderWindow& window);
    void setPosition(float x, float y);
    sf::FloatRect getBounds();
};

class Life : public Sprite {
public:
    Life(float X);
    void move(sf::Vector2f offset);
    void draw(sf::RenderWindow& window);
};

class SpaceShooterGame {
private:
    sf::RenderWindow window;
    PlayerSpaceShip* player;
    EnemySpaceShip enemies[10];
    bool spaceshipChosen;
    int i = 0;
    Life* Heart1;
    Life* Heart2;
    Life* Heart3;
    std::vector<Bullet> bullets;
    std::vector<Life*> hearts;

public:
    SpaceShooterGame();
    void chooseSpaceship();
    void run();
    void spawnEnemy();
    void showScore();
    ~SpaceShooterGame();
};
int SpaceShooter();
#endif
