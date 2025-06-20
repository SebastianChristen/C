#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Constants
const size_t COMMANDLENGTH = 32;
const size_t MAX_LINE_LEN = 255;


// polymorphism
class Entity {
public:
    std::string type;
    int id;
    std::string name;
    std::string description;
};

class Item : public Entity {
public:
    int position;
};

class Level : public Entity {
public:
    int n, e, s, w;
};

class Game {
private:
    std::vector<Level> levels;
    std::vector<Item> items;
    int currentLevel = 0;

public:
    void readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error opening file\n";
            exit(1);
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string token;
            std::getline(ss, token, '|');

            if (token == "map") {
                Level lvl;
                lvl.type = token;
                std::getline(ss, token, '|'); lvl.id = std::stoi(token);
                std::getline(ss, lvl.name, '|');
                std::getline(ss, lvl.description, '|');
                std::getline(ss, token, '|'); lvl.n = std::stoi(token);
                std::getline(ss, token, '|'); lvl.e = std::stoi(token);
                std::getline(ss, token, '|'); lvl.s = std::stoi(token);
                std::getline(ss, token, '|'); lvl.w = std::stoi(token);
                levels.push_back(lvl);
            } else if (token == "item") {
                Item item;
                item.type = token;
                std::getline(ss, token, '|'); item.id = std::stoi(token);
                std::getline(ss, item.name, '|');
                std::getline(ss, item.description, '|');
                std::getline(ss, token, '|'); item.position = std::stoi(token);
                items.push_back(item);
            }
        }
        file.close();
    }

    void processCommand(const std::string& verb, const std::string& arg = "") {
        if (verb == "look") {
            std::cout << levels[currentLevel].description << std::endl;
        } else if (verb == "quit") {
            exit(0);
        } else if (verb == "n") {
            currentLevel = levels[currentLevel].n;
        } else if (verb == "e") {
            currentLevel = levels[currentLevel].e;
        } else if (verb == "s") {
            currentLevel = levels[currentLevel].s;
        } else if (verb == "w") {
            currentLevel = levels[currentLevel].w;
        } else if (verb == "move") {
            if (arg == "north") {
                currentLevel = levels[currentLevel].n;
            } else if (arg == "east") {
                currentLevel = levels[currentLevel].e;
            } else if (arg == "south") {
                currentLevel = levels[currentLevel].s;
            } else if (arg == "west") {
                currentLevel = levels[currentLevel].w;
            } else {
                std::cout << "Unknown direction.\n";
                return;
            }
            std::cout << "You move " << arg << ".\n";
        } else {
            std::cout << "Unknown command.\n";
        }
    }

    void parseInput(const std::string& input) {
        std::istringstream iss(input);
        std::string verb, arg;
        iss >> verb;
        std::getline(iss, arg);
        if (!arg.empty() && arg[0] == ' ') arg.erase(0, 1);
        processCommand(verb, arg);
    }

    void start() {
        std::cout << "\n\nWelcome to adventure!\n\n";

        while (true) {
            const Level& lvl = levels[currentLevel];
            std::cout << "\n\n" << lvl.description << "\n";
            std::cout << "In the north, you see " << levels[lvl.n].name << ".\n";
            std::cout << "In the east,  you see " << levels[lvl.e].name << ".\n";
            std::cout << "In the south, you see " << levels[lvl.s].name << ".\n";
            std::cout << "In the west,  you see " << levels[lvl.w].name << ".\n";

            for (const auto& item : items) {
                if (item.position == lvl.id) {
                    std::cout << "There is a " << item.name << ".\n";
                }
            }

            std::cout << "\n>? ";
            std::string commandLine;
            std::getline(std::cin, commandLine);
            parseInput(commandLine);
        }
    }
};

int main() {
    Game game;
    game.readFile("world.wad");
    game.start();
    return 0;
}
