#pragma once

#include <SFML/Graphics.hpp> //SFML 
#include <windows.h>
#include <chrono>
#include <random>
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include <algorithm>

//other


//Window
sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Untitled", sf::Style::None);

using namespace std;
using namespace std::chrono;

int clamp(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

int randint(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return int(dist(gen));
}

int rands() {
    if (randint(0, 1) == 0) {
        return 1;
    }
    else {
        return -1;
    }
}

float distance(sf::Vector2f a, sf::Vector2f b) {
    float dx = b.x - a.x;
    float dy = b.y - a.y;
    return std::sqrt(dx * dx + dy * dy);
}

sf::Vector2f bestMove(sf::Vector2f pos, sf::Vector2f target, int tiles[33][33]) {
    sf::Vector2f candidates[8] = {
        {pos.x + 1, pos.y},
        {pos.x - 1, pos.y},
        {pos.x,     pos.y + 1},
        {pos.x,     pos.y - 1},
        {pos.x + 1, pos.y + 1},
        {pos.x - 1, pos.y + 1},
        {pos.x - 1, pos.y - 1},
        {pos.x + 1, pos.y - 1}
    };

    int best = 0;
    float bestDist = distance(candidates[0], target);

    for (int i = 0; i < 8; i++) {
        float d = distance(candidates[i], target);
        if (d < bestDist && tiles[int(candidates[i].y + 16)][int(candidates[i].x + 16)] == 0) {
            best = i;
            bestDist = d;
        }
    }

    return candidates[best];
}

int countFilesInFolder(const std::string& path) {
    namespace fs = std::filesystem;
    int count = 0;

    for (const auto& entry : fs::directory_iterator(path))
        if (entry.is_regular_file())
            ++count;

    return count;
}

std::vector<std::string> getFileNamesInFolder(const std::string& path) {
    namespace fs = std::filesystem;

    //path = fs::current_path().string() + "\\" + path;

    std::vector<std::string> files;
    cout << "-";
    try {
        for (const auto& entry : fs::directory_iterator(fs::current_path().string() + "\\" + path)) {
            files.push_back(entry.path().filename().string());
        }
    }
    catch (const fs::filesystem_error& e) {
        std::cerr << " (ERROR) " << e.what() << '\n';
    }
    cout << "-";
    return files;
}


class Character {
public:
    int hp, maxHp, moves = 1, maxMoves, damage, heal = 1, biuld = 0;
    sf::Vector2f pos;
    int id, team, texId = 0, state, timer = 0;
    float vision = 2, atkRadius = 0.1, moveRange = 1.5, atkRange = 1.5, biuldRange = 0;
    std::string name = "Character";
    bool inRange = false, trap = 0;

    Character(sf::Vector2f position = sf::Vector2f{ 0,0 }, int ID = 0, int t = 0, int status = 0) {
        pos = position;
        state = status;
        id = ID;
        team = t;

        int humans = 4;
        int monsters = 6;
        int traps = 2;

        //Humans
        if (id == 0) { //Archer
            name = "Archer";
            vision = 6;
            hp = 7;
            moveRange = 2.5;
            atkRange = 5.5;
            moves = 2;
            damage = 4;
        }if (id == 1) { //Mage
            name = "Mage";
            vision = 5;
            hp = 4;
            moveRange = 1.5;
            atkRange = 3.5;
            atkRadius = 1;
            damage = 11;
            moves = 1;
        }if (id == 2) { //Warrior
            name = "Warrior";
            vision = 3.5;
            hp = 13;
            moveRange = 1.5;
            atkRange = 1.5;
            damage = 5;
            moves = 2;
        }if (id == 3) { //Biulder
            name = "Biulder";
            vision = 2.5;
            hp = 20;
            moveRange = 1;
            atkRange = 1.5;
            damage = 3;
            moves = 2;
            biuldRange = 2;
            biuld = 1;
        }

        //Monsters
        if (id == humans) { //Goblin
            name = "Goblin";
            hp = 5;
            moveRange = 1.5;
            atkRange = 1.5;
            damage = 2;
            moves = 2;
            trap = 2;
        }if (id == humans + 1) { //Goblin Theif
            name = "Goblin Theif";
            hp = 7;
            moveRange = 1.5;
            atkRange = 2.5;
            damage = 2;
            moves = 2;
        }if (id == humans + 2) { //Ork Warrior 1
            name = "Orc Soldier";
            hp = 11;
            moveRange = 1;
            atkRange = 1.5;
            damage = 5;
            moves = 1;
        }if (id == humans + 3) { //Ork Archer
            name = "Ork Archer";
            hp = 6;
            moveRange = 1.5;
            atkRange = 5;
            damage = 4;
            moves = 1;
        }if (id == humans + 4) { //Ork Warrior 2
            name = "Orc Tank";
            hp = 19;
            moveRange = 1;
            atkRange = 1.5;
            damage = 3;
            moves = 1;
        }if (id == humans + 5) { //Ork Mage
            name = "Ork Mage";
            hp = 7;
            moveRange = 1.5;
            atkRange = 3;
            damage = 8;
            moves = 1;
        }

        //Objects
        if (id == humans + monsters) { //Torch
            vision = 3.5;
            name = "Torch";
            hp = 3;
            moveRange = 0;
            atkRange = 0;
            damage = 0;
            moves = 0;
        }if (id == humans + monsters + 1) { //Trap
            name = "Trap";
            vision = 1.5;
            hp = 9;
            moveRange = 0;
            atkRange = 1.5;
            damage = 12;
            moves = 1;
            trap = 1;
        }if (id == humans + monsters + 2) { //Wall
            name = "Wall";
            vision = 0;
            hp = 8;
            moveRange = 0;
            atkRange = 0;
            damage = 0;
            moves = 0;
            trap = 1;
        }if (id == humans + monsters + 3) { //Tower
            name = "Tower";
            vision = 4.5;
            hp = 9;
            moveRange = 0;
            atkRange = 3;
            damage = 3;
            moves = 1;
            trap = 0;
        }if (id == humans + monsters + 4) { //Tower
            name = "Tower";
            vision = 4.5;
            hp = 9;
            moveRange = 0;
            atkRange = 3;
            damage = 3;
            moves = 1;
            trap = 0;
        }


        maxMoves = moves;
        maxHp = hp;
    }
};

class Object {
public:
    int hp, maxHp;
    sf::Vector2f pos;
    int id, team = 0, texId;


    Object(sf::Vector2f position = sf::Vector2f{ 0,0 }, int textureId = 0, int ID = 0, int health = 1) {
        pos = position;
        texId = textureId;
        hp = health;
        maxHp = health;
        id = ID;
    }
};

