#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <thread>

sf::Font font;        


int points = 0;
using namespace std;

class Sprite {
    protected:
    sf::Texture texture;
    sf::Sprite sprite;

    public:
    virtual void move(sf::Vector2f offset) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~Sprite() {}
};

class Spaceship: public Sprite {
public:
    virtual void move(sf::Vector2f offset) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual ~Spaceship() {}
};

class PlayerSpaceShip : public Spaceship {
public:
    virtual void move(sf::Vector2f offset) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool contains(sf::Vector2f point){ return 0;}
    virtual ~PlayerSpaceShip() {}
    float getPositionX(){
        return sprite.getPosition().x;
    }
    float getPositionY(){
        return sprite.getPosition().y;
    }
    float getWidth(){
        return sprite.getGlobalBounds().width;
    }
    sf::FloatRect getBounds(){
        return sprite.getGlobalBounds();
    }
};

class OrangeSpaceShip : public PlayerSpaceShip {

public:
    OrangeSpaceShip() {
        if (!texture.loadFromFile("OrangeSpaceShip.png")) {
            std::cerr << "Failed to Load OrangeSpaceShip texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setScale(0.2f, 0.2f);
        sprite.setPosition(275.f, 500.f);
    }

    void move(sf::Vector2f offset){
        if(offset.x < 0 ){
            offset.x = offset.x + 2.0f;
        }else offset.x = offset.x - 2.0f; 
        sprite.move(offset);
    }
    void scale(sf::Vector2f offset) {
        sprite.setScale(offset);
    }
    void draw(sf::RenderWindow& window){
        window.draw(sprite);
    }
    bool contains(sf::Vector2f point){
        return sprite.getGlobalBounds().contains(point);
    }
};

class WhiteSpaceShip : public PlayerSpaceShip {

public:
    WhiteSpaceShip() {
        if (!texture.loadFromFile("WhiteSpaceShip.png")) {
            std::cerr << "Failed to Load WhiteSpaceShip texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setScale(0.125f, 0.125f);
        sprite.setPosition(275.0f, 500.0f);
    }

    void move(sf::Vector2f offset){
        if(offset.x < 0){
            offset.x = offset.x - 10.0f;
        }else offset.x = offset.x + 10.0f;
        
        sprite.move(offset);
    }
    
    void scale(sf::Vector2f offset) {
        sprite.setScale(offset);
    }

    void draw(sf::RenderWindow& window){
        window.draw(sprite);
    }

    bool contains(sf::Vector2f point){
        return sprite.getGlobalBounds().contains(point);
    }
};

class RedSpaceShip : public PlayerSpaceShip {

public:
    RedSpaceShip() {
        if (!texture.loadFromFile("RedSpaceShip.png")) {
            std::cerr << "Failed to Load RedSpaceShip texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setScale(0.16f, 0.16f);
        sprite.setPosition(275.0f, 500.0f);
    }

    void move(sf::Vector2f offset){
        sprite.move(offset);
    }
    
    void scale(sf::Vector2f offset) {
        sprite.setScale(offset);
    }

    void draw(sf::RenderWindow& window){
        window.draw(sprite);
    }

    bool contains(sf::Vector2f point){
        return sprite.getGlobalBounds().contains(point);
    }
};

class EnemySpaceShip : public PlayerSpaceShip{

private:
    bool alive;
public:
    EnemySpaceShip(){
        if (!texture.loadFromFile("EnemySpaceShip.png")) {
            std::cerr << "Failed to Load EnemySpaceShip texture!" << std::endl;
        }
        alive = false;
        sprite.setTexture(texture);
        sprite.setScale(0.03f, 0.03f);
        sprite.setPosition(0, -10.0f);
    }
    void spawn(sf::Vector2f offset){
        sprite.setPosition(offset);
        alive=true;
    }
    void move(sf::Vector2f offset) {
        sprite.move(offset);
        alive = true;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
    bool isAlive(){
        return alive;
    }
    void destroy(){
        alive = false;
    }
    float getPositionY(){
        return sprite.getPosition().y;
    }
    float getPositionX(){
        return sprite.getPosition().y;
    }
};

class Bullet : public Sprite {

public:
    Bullet(){
        if(!texture.loadFromFile("bullet.jpg")){
            cout<<"Couldn't Load Bullet Image";
        }
        sprite.setTexture(texture);
        sprite.setScale(0.05f, 0.05f);
        sprite.setPosition(10.f, 10.f);
    }
    void move(sf::Vector2f offset) {
        sprite.move(offset);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
    void setPosition(float x , float y){
        sprite.setPosition(x,y);
    }

    sf::FloatRect getBounds(){
        return sprite.getGlobalBounds();
    }
    

};
class Life : public Sprite {
public:
    Life(float X){
        if(!texture.loadFromFile("heart.png")){
            cout<<"Couldn't Load Heart Image";
        }
        sprite.setTexture(texture);
        sprite.setScale(0.2f, 0.2f);
        sprite.setPosition(X, 10.f);
    }
    void move(sf::Vector2f offset) {
        sprite.move(offset);
    }
    void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }
};

class SpaceShooterGame{
private:
    sf::RenderWindow window;
    PlayerSpaceShip* player;
    EnemySpaceShip enemies[10];
    bool spaceshipChosen;
    int i = 0;
    Life *Heart1 = new Life(490.0f);
    Life *Heart2 = new Life(520.0f);
    Life *Heart3 = new Life(550.0f);
    vector<Bullet> bullets;

    vector<Life *> hearts = {Heart1, Heart2, Heart3};

public:
    SpaceShooterGame() : window(sf::VideoMode(600, 600), "Space Shooter", sf::Style::Close), spaceshipChosen(false){}
    void chooseSpaceship() {
        
        sf::Text gamename;
        sf::Text orangeShipDescription;
        sf::Text redShipDescription;
        sf::Text whiteShipDescription;

        gamename.setFont(font);
        gamename.setString("SPACE SHOOTER");
        gamename.setCharacterSize(40);
        gamename.setFillColor(sf::Color::Red);
        gamename.setPosition(130.0f,150.0f);

        orangeShipDescription.setFont(font);
        orangeShipDescription.setString("Size: Large\nSpeed: Slow");
        orangeShipDescription.setCharacterSize(15);
        orangeShipDescription.setFillColor(sf::Color(255,165,0));
        orangeShipDescription.setPosition(160.0f,375.0f);

        redShipDescription.setFont(font);
        redShipDescription.setString("Size: Normal\nSpeed: Normal");
        redShipDescription.setCharacterSize(15);
        redShipDescription.setFillColor(sf::Color::Red);
        redShipDescription.setPosition(260.0f,375.0f);

        whiteShipDescription.setFont(font);
        whiteShipDescription.setString("Size: Small\nSpeed: Fast");
        whiteShipDescription.setCharacterSize(15);
        whiteShipDescription.setFillColor(sf::Color::White);
        whiteShipDescription.setPosition(370.0f,375.0f);
        
        OrangeSpaceShip orangeShip;
        WhiteSpaceShip whiteShip;
        RedSpaceShip redShip;

        redShip.move(sf::Vector2f(0.0f,-200.0f));
        orangeShip.move(sf::Vector2f(-100.0f, -200.0f));
        whiteShip.move(sf::Vector2f(100.0f, -200.0f));
        orangeShip.scale(sf::Vector2f(0.2f,0.2f));
        whiteShip.scale(sf::Vector2f(0.2f,0.2f));
        redShip.scale(sf::Vector2f(0.2f,0.2f));

        while (window.isOpen() && !spaceshipChosen) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) {
                    window.close();
                }
                if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPosition = window.mapPixelToCoords(mousePosition);
                    
                    if (orangeShip.contains(worldPosition)) {
                        player = new OrangeSpaceShip();
                        spaceshipChosen = true;
                    } else if (whiteShip.contains(worldPosition)) {
                        player = new WhiteSpaceShip();
                        spaceshipChosen = true;
                    } else if (redShip.contains(worldPosition)){
                        player = new RedSpaceShip();
                        spaceshipChosen = true;
                    }
                }
            }

            window.clear();

            
            orangeShip.draw(window);
            whiteShip.draw(window);
            redShip.draw(window);
            window.draw(gamename);
            window.draw(orangeShipDescription);
            window.draw(redShipDescription);
            window.draw(whiteShipDescription);
            window.display();
        }
        run();
    }

    void run() {
        sf::Clock clock;
        sf::Text score;
        score.setFont(font);
        score.setCharacterSize(24);
        score.setString(to_string(points));
        score.setPosition(540.0f,45.0f);
        score.setFillColor(sf::Color::Yellow);
        while (window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) {
                    window.close();
                }
                if (spaceshipChosen && ev.type == sf::Event::KeyPressed) {
                    if (ev.key.code == sf::Keyboard::A && player && player->getPositionX() > 0) {
                        player->move(sf::Vector2f(-10.0f, 0.0f));
                    }
                    if (ev.key.code == sf::Keyboard::D && player && ((player->getPositionX()+ player->getWidth())<600)) {
                        player->move(sf::Vector2f(10.0f, 0.0f));
                    }
                    if (ev.key.code == sf::Keyboard::Space && player && clock.getElapsedTime().asSeconds() > 0.3){
                        Bullet bullet;
                        bullets.push_back(bullet);
                        bullets.back().setPosition(((player->getPositionX())+(player->getWidth()/2)), player->getPositionY());
                        clock.restart();
                    }
                }
            }
             if (rand() % 2000 == 0) { 
                spawnEnemy();
            }

            window.clear();
              for (auto& enemy : enemies) {
                if (enemy.isAlive()) {
                    if(points < 150){
                        enemy.move(sf::Vector2f(0.0f,0.03f));
                    }
                    else if(points < 250){
                        enemy.move(sf::Vector2f(0.0f, 0.05f));
                    }else if (points < 500){
                        enemy.move(sf::Vector2f(0.0f, 0.07f));
                    }else if (points < 750){
                        enemy.move(sf::Vector2f(0.0f, 0.1f));
                    }
                    if(enemy.getPositionX() > 0 && (enemy.getPositionX() + enemy.getWidth()) < 600){
                        enemy.move(sf::Vector2f( i%2==0 ? -0.03f : 0.03f , 0.0f));
                        i = 1-i;
                    }
                    enemy.draw(window);
                    if (enemy.getPositionY() > 600 || enemy.getBounds().intersects(player->getBounds())) {
                        delete hearts.back();
                        hearts.pop_back(); 
                        enemy.destroy();
                        if(hearts.size()==0){
                            showScore();
                        }
                    }else {
                        for(auto& bullet : bullets){
                            if (enemy.getBounds().intersects(bullet.getBounds())){
                            points +=10;
                            score.setString(to_string(points));
                            enemy.destroy();
                            bullet.move(sf::Vector2f(-1000.0f,1000.0f));
                            }
                        }  
                    }
                }
            }
            for(auto& bullet : bullets){
                bullet.draw(window);
                bullet.move(sf::Vector2f(0.0f,-1.0f));
            }
            if (spaceshipChosen) {
                for(int i=0;i<hearts.size();i++){
                    hearts.at(i)->draw(window);
                }
                window.draw(score);
                player->draw(window);
            }
            
            window.display();
        }
    }
    void spawnEnemy() {
        for (auto& enemy : enemies) {
            if (!enemy.isAlive()) {
                enemy.spawn(sf::Vector2f((float)(rand() % 300) + 100, -10.0f));
                break;
            }
        }
    }
    void showScore(){
        
        sf::Text heading;
        heading.setFont(font);
        heading.setCharacterSize(50);
        heading.setString("Score");
        heading.setFillColor(sf::Color::Red);
        heading.setPosition(250.0f,250.0f);
        
        sf::Text score;
        score.setFont(font);
        score.setCharacterSize(25);
        score.setString(to_string(points));
        score.setFillColor(sf::Color::Yellow);
        score.setPosition(300.0f,325.0f);

        while (window.isOpen()) {
            sf::Event ev;
            while (window.pollEvent(ev)) {
                if (ev.type == sf::Event::Closed) {
                    window.close();
                }
            }
        window.clear();
        window.draw(heading);
        window.draw(score);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        window.close();
        }
    }
    ~SpaceShooterGame(){
        delete player;
    }
};

int SpaceShooter() {
    srand(time(NULL));
    font.loadFromFile("arial.ttf");
    SpaceShooterGame game;
    game.chooseSpaceship();
    return 0;
}