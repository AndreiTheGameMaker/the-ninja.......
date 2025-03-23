#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <fstream>
#include <string>

class Player {
public:
    sf::RectangleShape shape;
    float speed = 200.f;
    int hp = 100;
    int xp = 0;
    std::string name;

    Player(float x, float y, const std::string& playerName) {
        shape.setSize(sf::Vector2f(50.f, 50.f));  // Mărimea personajului
        shape.setFillColor(sf::Color::Blue);
        position = sf::Vector2f(x, y);
        shape.setPosition(position);
        name = playerName;
    }

    sf::Vector2f position;

    void move(const sf::Time& deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) position.x -= speed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) position.x += speed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) position.y -= speed * deltaTime.asSeconds();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) position.y += speed * deltaTime.asSeconds();
        shape.setPosition(position);
    }

    void takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    void gainXP(int amount) {
        xp += amount;
    }

    bool isAlive() const {
        return hp > 0;
    }
};

// Funcție pentru a salva datele jucătorului (HP, XP, Nume)
void savePlayerData(const Player& player) {
    std::ofstream file("player_data.txt");
    if (file.is_open()) {
        file << player.name << std::endl;
        file << player.hp << std::endl;
        file << player.xp << std::endl;
        file.close();
    }
}

// Funcție pentru a încărca datele jucătorului (HP, XP, Nume)
bool loadPlayerData(Player& player) {
    std::ifstream file("player_data.txt");
    if (file.is_open()) {
        std::getline(file, player.name);
        file >> player.hp;
        file >> player.xp;
        file.close();
        return true;
    }
    return false;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ninja Brawl Third-Person");
    window.setFramerateLimit(60);

    std::string playerName;
    std::cout << "Enter your player name: ";
    std::cin >> playerName;

    Player player(375.f, 275.f, playerName);

    // Încărcarea datelor anterioare ale jucătorului
    if (loadPlayerData(player)) {
        std::cout << "Player loaded: " << player.name << std::endl;
        std::cout << "HP: " << player.hp << ", XP: " << player.xp << std::endl;
    } else {
        std::cout << "No previous player data found." << std::endl;
    }

    sf::View view = window.getDefaultView();
    view.setSize(800.f, 600.f);
    view.setCenter(player.shape.getPosition().x + 25.f, player.shape.getPosition().y + 25.f); 

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        sf::Time deltaTime = clock.restart();

        // Mișcarea jucătorului
        player.move(deltaTime);

        // Camera urmează jucătorul
        view.setCenter(player.shape.getPosition().x + 25.f, player.shape.getPosition().y + 25.f);
        window.setView(view);

        // Simularea unei interacțiuni cu alți inamici
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            player.takeDamage(10);
            player.gainXP(5);
        }

        // Salvarea progresului periodic (pentru testare)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            savePlayerData(player);
            std::cout << "Data saved!" << std::endl;
        }

        // Curățarea ferestrei și desenarea obiectelor
        window.clear();
        window.draw(player.shape);
        window.display();
    }

    return 0;
}
