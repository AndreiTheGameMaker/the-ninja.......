#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <iostream>

class Ninja {
public:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f velocity;
    
    Ninja(const std::string& texturePath, sf::Vector2f position) {
        if (!texture.loadFromFile(texturePath)) {
            std::cerr << "Failed to load texture!" << std::endl;
        }
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void move(float dx, float dy) {
        sprite.move(dx, dy);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ninja Sniper Game");
    window.setFramerateLimit(60);

    Ninja player("ninja.png", sf::Vector2f(100, 300));
    std::vector<Ninja> enemies = {
        Ninja("enemy_ninja.png", sf::Vector2f(600, 300)),
        Ninja("enemy_ninja.png", sf::Vector2f(500, 200))
    };

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.move(-5, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.move(5, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.move(0, -5);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.move(0, 5);
        }

        window.clear();
        window.draw(player.sprite);
        for (auto& enemy : enemies) {
            window.draw(enemy.sprite);
        }
        window.display();
    }

    return 0;
}
