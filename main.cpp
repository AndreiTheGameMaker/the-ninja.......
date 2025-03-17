#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Ninja {
public:
    int hp;
    sf::RectangleShape healthBar;
    sf::RectangleShape player;
    bool hasSniper;

    Ninja() {
        hp = 100;
        hasSniper = false; // Start with the ninja stick

        // Initialize player shape
        player.setSize(sf::Vector2f(50, 50));
        player.setFillColor(sf::Color::Blue);
        player.setPosition(375, 275); // Start in the middle

        // Health bar setup
        healthBar.setSize(sf::Vector2f(100, 10));
        healthBar.setFillColor(sf::Color::Green);
        healthBar.setPosition(10, 10);
    }

    void move(float dx, float dy) {
        player.move(dx, dy);
    }

    void takeDamage(int damage) {
        hp -= damage;
        if (hp < 0) hp = 0;

        healthBar.setSize(sf::Vector2f(hp, 10));
        if (hp < 50) healthBar.setFillColor(sf::Color::Yellow);
        if (hp < 20) healthBar.setFillColor(sf::Color::Red);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(player);
        window.draw(healthBar);
    }
};

class Enemy {
public:
    sf::RectangleShape enemy;
    int hp;

    Enemy(float x, float y) {
        hp = 50; // Enemy starts with 50 HP
        enemy.setSize(sf::Vector2f(40, 40));
        enemy.setFillColor(sf::Color::Red);
        enemy.setPosition(x, y);
    }

    void moveTowards(Ninja &player) {
        sf::Vector2f playerPos = player.player.getPosition();
        sf::Vector2f enemyPos = enemy.getPosition();
        sf::Vector2f direction = playerPos - enemyPos;

        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction /= length;
        }

        enemy.move(direction * 0.5f);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(enemy);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ninja Battle");
    Ninja player;
    std::vector<Enemy> enemies;

    // Spawn random enemies
    srand(static_cast<unsigned>(time(0)));
    for (int i = 0; i < 3; i++) {
        float x = rand() % 700 + 50;
        float y = rand() % 500 + 50;
        enemies.emplace_back(x, y);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player.move(0, -2);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player.move(0, 2);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.move(-2, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.move(2, 0);

        // Switch weapon
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) player.hasSniper = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) player.hasSniper = true;

        // Enemy AI
        for (auto &enemy : enemies) {
            enemy.moveTowards(player);

            // Simulate enemy attack when close
            if (player.player.getGlobalBounds().intersects(enemy.enemy.getGlobalBounds())) {
                player.takeDamage(1); // Lose 1 HP per frame
            }
        }

        // End game if HP reaches 0
        if (player.hp <= 0) {
            std::cout << "Game Over!" << std::endl;
            window.close();
        }

        window.clear();
        player.draw(window);
        for (auto &enemy : enemies) {
            enemy.draw(window);
        }
        window.display();
    }

    return 0;
}

