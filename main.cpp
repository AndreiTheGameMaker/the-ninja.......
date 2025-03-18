#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>

class Ninja {
public:
    int hp;
    sf::RectangleShape healthBar;
    sf::RectangleShape player;
    bool hasSniper;

    Ninja() {
        hp = 100;
        hasSniper = false; // Default weapon: ninja stick

        // Initialize player
        player.setSize(sf::Vector2f(50, 50));
        player.setFillColor(sf::Color::Blue);
        player.setPosition(375, 275); // Start at the center

        // Health bar
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

        // Update health bar
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
    int attackDamage;
    int attackCooldown; // Time between attacks

    Enemy(float x, float y) {
        attackDamage = 5; // Damage per hit
        attackCooldown = 0; // Start with no delay

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

    void attack(Ninja &player) {
        if (enemy.getGlobalBounds().intersects(player.player.getGlobalBounds())) {
            if (attackCooldown <= 0) {
                player.takeDamage(attackDamage);
                attackCooldown = 30; // Reset cooldown
                std::cout << "Ninja took damage! HP: " << player.hp << std::endl;
            }
        }
        if (attackCooldown > 0) {
            attackCooldown--;
        }
    }

    void draw(sf::RenderWindow &window) {
        window.draw(enemy);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ninja Battle");
    Ninja player;
    std::vector<Enemy> enemies;

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

        // Player Movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) player.move(0, -2);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) player.move(0, 2);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) player.move(-2, 0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) player.move(2, 0);

        // Weapon Switching
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)) player.hasSniper = false;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)) player.hasSniper = true;

        // Enemy AI
        for (auto &enemy : enemies) {
            enemy.moveTowards(player);
            enemy.attack(player);
        }

        // Check if player is dead
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


