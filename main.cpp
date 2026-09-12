//Include
#include "Header.h"

int main() {
    // Create window
    window.setFramerateLimit(125);
    HWND hwnd = window.getSystemHandle();
    //FreeConsole();

    //Camera
    sf::View Camera;
    //Camera.setSize(320, 180);
    Camera.setSize(480, 270);
    Camera.setCenter(0, 0);
    float cameraSpd = 2.5;
    sf::Vector2f cameraPos = { 0,0 };
    sf::Vector2f mousePos = { 0,0 };
    //sf::Vector2f cameraSize;
    float zoom = 1;

    //Time
    auto last = high_resolution_clock::now();
    auto last2 = high_resolution_clock::now();
    auto now = high_resolution_clock::now();
    int r = 0;

    //Input
    bool mouseL = false;
    bool mouseR = false;
    bool keyW = false;
    bool keyA = false;
    bool keyS = false;
    bool keyD = false;
    bool keyQ = false;
    bool keyE = false;
    bool keyEsc = false;
    bool keyEnter = false;
    bool key1 = false;
    bool key2 = false;
    bool key3 = false;


    //Classes
    std::vector <Character> Characters;
    int charNum[3] = { 4,6,3 };
    int trapped = 0;
    std::vector <Object> Objects;
    int visionList[33][33];

    /*
    TILE VISION NUMBER
    0 = empty
    1 = slight fog
    2 = more fog
    3 = black
    4 = move to range
    5 = attack range
    6 = enemy
    7 = self
    8 = biuld range

    */

    int blockedTiles[33][33];

    /*
    TILE OBJECT NUMBER
    0 = empty
    1 = player
    2 = enemy
    3 = obstacle
    4 = chest

    */

    for (int i = 0; i < 33; i++) {
        for (int j = 0; j < 33; j++) {
            visionList[i][j] = 3;
        }
    }for (int i = 0; i < 33; i++) {
        for (int j = 0; j < 33; j++) {
            blockedTiles[i][j] = 0;
        }
    }

    //Selection
    int selectedChar = -1;
    int selectedEnemy = -1;
    bool movingEnemy = false;
    bool foundChar = false;
    int action = 0; //0 = move, 1 = attack, 2 = biuld
    sf::Vector2f selectedTile = { 0,0 };
    bool update = true;
    string infoText = "";

    //Menu
    int gamemode = 2;
    /*
    GAMEMODES
    0 = Main Menu
    1 = Charachter Selection
    2 = Game
    3 = End Screen

    */


    //Wave
    int wave = 0;
    bool nextWave = true;
    int waveCooldown = 0;
    bool pressEnter = false;
    float difficulty = 1;
    int cooldown = 0;
    int resources[10];
    /*
    RECORCES
    0 = Rock
    1 = Wood
    2 = Coal
    3 = Iron
    4 =
    */
    int enemiesLeft = 0;

    for (int i = 0; i < 10; i++) {
        resources[i] = 0;
    }



    //Other
    std::vector <string> files;
    std::vector <string> files2;
    sf::Vector2f position;
    bool b = false;
    float min = 0;
    bool nextTurn = true;
    bool debug = false;
    int texTimer = 0;
    int texTex = 0;


    string rewardTextWords[11] = { "[+ Wood]", "[+ Rock]", "[+ Coal]", "[+ Iron]", "[+ Magic]", "[+ Health]" , "[+ Damage]" , "[+ Biuld Range]" , "[+ Range]" , "[+ Moves] [- Damage]" };
    struct RewardText {
        string text = "";
        int timer = 0;
        sf::Vector2f pos;
        sf::Color color = sf::Color::White;
    };
    std::vector<RewardText> rewardText;

    bool biuldCheck = false;
    int biuldSelect = 0;
    int recepies[10][5] = {
        {2,0,1,0,0} , //Torch
        {0,0,1,2,0} , //Trap
        {1,0,3,0,0} , //Wall
        {1,0,3,1,1} , //Tower
        {0,0,0,1,3} , //Skeleton

    };


    //Font
    sf::Font font;
    font.loadFromFile("5x6.ttf");
    font.setSmooth(false);

    sf::Text text;
    text.setFont(font);           // set font
    text.setString(""); // set string
    text.setCharacterSize(6);    // in pixels
    text.setFillColor(sf::Color::White); // text color
    text.setPosition(0.f, 0.f);

    cout << "Loading assets";

    sf::Texture AssetsTex[100];
    files = getFileNamesInFolder("Assets");
    cout << " (Loaded Assets):" << endl;
    std::sort(files.begin(), files.end());
    for (int i = 0; i < files.size(); i++) {
        AssetsTex[i].loadFromFile("Assets\\" + files[i]);
        cout << " - Loaded " << files[i] << endl;
    }

    sf::Sprite Background;
    Background.setOrigin(264, 264);
    Background.setTexture(AssetsTex[0]);

    sf::Sprite Tile;
    Tile.setOrigin(8, 8);
    Tile.setTexture(AssetsTex[8]);

    cout << "Loading characters:" << endl;

    sf::Texture CharacterTex[30][100];
    files2 = getFileNamesInFolder("Characters");
    std::sort(files2.begin(), files2.end());
    for (int j = 0; j < files2.size(); j++) {
        cout << "+ Loading " << files2[j] << " ";
        files = getFileNamesInFolder("Characters\\" + files2[j]);
        cout << "[" << files.size() << "]" << endl;
        std::sort(files.begin(), files.end());
        for (int i = 0; i < files.size(); i++) {
            CharacterTex[j][i].loadFromFile("Characters\\" + files2[j] + "\\" + files[i]);
            cout << " - Loaded " << files[i] << endl;
        }
    }

    sf::Sprite Entity;
    Entity.setTexture(CharacterTex[0][0]);
    Entity.setOrigin(16, 16);


    sf::RectangleShape Fog;
    Fog.setFillColor(sf::Color(0, 0, 0, 100));
    Fog.setSize(sf::Vector2f(16, 16));
    Fog.setOrigin(8, 8);

    sf::Sprite UI;
    UI.setOrigin(240, 135);
    UI.setTexture(AssetsTex[7]);

    sf::Sprite HealthBar;
    HealthBar.setTexture(AssetsTex[14]);
    HealthBar.setOrigin(6, -5);

    sf::RectangleShape Bar;
    Bar.setOrigin(5, -6);
    Bar.setFillColor(sf::Color(20, 200, 20, 150));

    sf::RectangleShape InfoBar;
    InfoBar.setSize(sf::Vector2f(text.getGlobalBounds().width, text.getGlobalBounds().height));
    InfoBar.setFillColor(sf::Color(240, 228, 209));


    //Startup


    //Characters
    for (int i = 0; i < 4; i++) {
        position = { float(randint(-2,2)), float(randint(-2,2)) };
        while (blockedTiles[int(position.y + 16)][int(position.x + 16)] != 0) {
            position = { float(randint(-2,2)), float(randint(-2,2)) };
        }
        Characters.push_back(Character(position, i, 0));
        blockedTiles[int(position.y + 16)][int(position.x + 16)] = 1;
    }

    for (int j = 0; j < Characters.size() - 1; j++) {
        if (Characters[j].pos.y > Characters[j + 1].pos.y) {
            std::swap(Characters[j], Characters[j + 1]);
        }
    }


    //Enviroment
    for (int i = 0; i < 150; i++) {
        position = { float(randint(-15,15)), float(randint(-15,15)) };
        while (blockedTiles[int(position.y + 16)][int(position.x + 16)] != 0) {
            position = { float(randint(-15,15)), float(randint(-15,15)) };
        }
        Objects.push_back(Object(position, randint(5, 6)));
        if (Objects[i].texId == 6 && randint(0, 1) > 0) {
            Objects[i].texId = 19; //Coal
            if (randint(0, 1) > 0) {
                Objects[i].texId = 20; //Iron
                if (randint(0, 1) > 0) {
                    Objects[i].texId = 21; //Mana Stone
                }
            }
        }
        blockedTiles[int(position.y + 16)][int(position.x + 16)] = 3;

    }

    //Chest
    for (int i = 0; i < 45; i++) {
        position = { float(randint(8,15) * rands()), float(randint(8,15) * rands()) };
        while (blockedTiles[int(position.y + 16)][int(position.x + 16)] != 0) {
            position = { float(randint(8,15) * rands()), float(randint(8,15) * rands()) };
        }
        Objects.push_back(Object(position, randint(17, 18), 1));
        blockedTiles[int(position.y + 16)][int(position.x + 16)] = 4;

    }

    // Main loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.key.code == sf::Mouse::Left) mouseL = true;
                if (event.key.code == sf::Mouse::Right) mouseR = true;
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                //if (event.key.code == sf::Mouse::Left) mouseL = false;
                //if (event.key.code == sf::Mouse::Left) mouseR = false;
            }

            // Key pressed
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W) keyW = true;
                if (event.key.code == sf::Keyboard::S) keyS = true;
                if (event.key.code == sf::Keyboard::A) keyA = true;
                if (event.key.code == sf::Keyboard::D) keyD = true;
                if (event.key.code == sf::Keyboard::E) keyE = true;
                if (event.key.code == sf::Keyboard::Q) keyQ = true;
                if (event.key.code == sf::Keyboard::Escape) keyEsc = true;
                if (event.key.code == sf::Keyboard::Enter) keyEnter = true;
                if (event.key.code == sf::Keyboard::Num1) key1 = true;
                if (event.key.code == sf::Keyboard::Num2) key2 = true;
                if (event.key.code == sf::Keyboard::Num3) key3 = true;
            }

            // Key released
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W) keyW = false;
                if (event.key.code == sf::Keyboard::S) keyS = false;
                if (event.key.code == sf::Keyboard::A) keyA = false;
                if (event.key.code == sf::Keyboard::D) keyD = false;
                if (event.key.code == sf::Keyboard::E) keyE = false;
                if (event.key.code == sf::Keyboard::Q) keyQ = false;
                if (event.key.code == sf::Keyboard::Escape) keyEsc = false;
                if (event.key.code == sf::Keyboard::Enter) keyEnter = false;
                if (event.key.code == sf::Keyboard::Num1) key1 = false;
                if (event.key.code == sf::Keyboard::Num2) key2 = false;
                if (event.key.code == sf::Keyboard::Num3) key3 = false;
            }

            //On exit
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            //Focus
            if (event.type == sf::Event::GainedFocus) {
                SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
            }
            if (event.type == sf::Event::LostFocus) {
                SetPriorityClass(GetCurrentProcess(), BELOW_NORMAL_PRIORITY_CLASS);
            }
        }

        now = high_resolution_clock::now();


        mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        if (now - last >= milliseconds(16)) {

            if (keyW && cameraPos.y > -250) cameraPos += {0, -cameraSpd * zoom};
            if (keyS && cameraPos.y < 250) cameraPos += {0, cameraSpd* zoom};
            if (keyA && cameraPos.x > -250) cameraPos += {-cameraSpd * zoom, 0};
            if (keyD && cameraPos.x < 250) cameraPos += {cameraSpd* zoom, 0};
            if (keyE && zoom > 0.3) zoom -= 0.01;
            if (keyQ && zoom < 1.3) zoom += 0.01;

            Camera.setSize(480 * zoom, 270 * zoom);
            Camera.setCenter(cameraPos);




            if (now - last2 >= milliseconds(100)) {

                if (gamemode == 2) {
                    infoText = "";

                    for (int i = 0; i < 33; i++) {
                        for (int j = 0; j < 33; j++) {
                            if (mousePos.x > (j - 16) * 16 - 8 && mousePos.x < (j - 16) * 16 + 8 && mousePos.y >(i - 16) * 16 - 8 && mousePos.y < (i - 16) * 16 + 8) {
                                selectedTile = sf::Vector2f(j - 16, i - 16);
                            }
                        }
                    }

                    if (visionList[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 6) {
                        if (blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 2) {
                            infoText = "Attack Enemy";
                        }
                        else if (blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 1) {
                            infoText = "Enemy Attack";
                        }
                        else {
                            infoText = "Destroy Obstacle";
                        }
                    }
                    else if (visionList[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 4 && Characters[selectedChar].team == 0) {
                        infoText = "Move Ally";
                    }
                    else if (visionList[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 7) {
                        infoText = "Collect Treasure";
                    }
                    else if (visionList[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 8 && Characters[selectedChar].biuld == 1) {
                        infoText = "Biuld";
                    }

                    //timers
                    for (int i = 0; i < rewardText.size(); i++) {
                        rewardText[i].timer--;
                        if (rewardText[i].timer < 0) { rewardText.erase(rewardText.begin() + i); }

                    }


                    //Update aimations
                    texTimer++;

                    if (texTimer > 5) {
                        texTimer = 0;

                        if (texTex == 0) {
                            texTex = 1;
                        }
                        else {
                            texTex = 0;
                        }

                        for (int i = 0; i < Characters.size(); i++) {
                            //Characters[i].timer++;
                            //texTimer++;
                            if (Characters[i].state == 0) {
                                Characters[i].texId = texTex;
                            }
                        }
                    }

                    //Biuld select
                    if (mouseR) {
                        mouseR = false;
                        biuldSelect++;
                        if (biuldSelect > 2) {
                            biuldSelect = 0;
                        }
                    }

                    biuldCheck = true;
                    for (int i = 0; i < 5; i++) {
                        if (resources[i] < recepies[biuldSelect][i]) {
                            biuldCheck = false;
                        }
                    }
                }

                last2 = now;
            }

            if (gamemode == 2) {
                //INTERACT
                if (mouseL && !movingEnemy) {
                    update = true;
                    b = true;

                    for (int i = 0; i < 33; i++) {
                        for (int j = 0; j < 33; j++) {
                            if (mousePos.x > (j - 16) * 16 - 8 && mousePos.x < (j - 16) * 16 + 8 && mousePos.y >(i - 16) * 16 - 8 && mousePos.y < (i - 16) * 16 + 8) {
                                selectedTile = sf::Vector2f(j - 16, i - 16);
                            }
                        }
                    }

                    foundChar = false;
                    for (int i = 0; i < Characters.size(); i++) {
                        if (selectedTile.x == Characters[i].pos.x && selectedTile.y == Characters[i].pos.y && visionList[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 0) {
                            selectedChar = i;
                            foundChar = true;
                        }
                    }

                    if (selectedChar != -1 && Characters[selectedChar].team == 0 && Characters[selectedChar].moves > 0) {
                        //Action
                        if (action == 0 && !foundChar && distance(selectedTile, Characters[selectedChar].pos) <= Characters[selectedChar].moveRange && (blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 0 || blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 4)) {
                            if (blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 4) {
                                for (int i = 0; i < Objects.size(); i++) {
                                    if (selectedTile.x == Objects[i].pos.x && selectedTile.y == Objects[i].pos.y) {
                                        rewardText.push_back(RewardText{});
                                        if (Objects[i].texId == 17) {
                                            r = randint(0, 4);
                                            resources[r] += randint(0, 4);
                                            rewardText[rewardText.size() - 1].text += rewardTextWords[r];
                                            r = randint(0, 4);
                                            resources[r] += randint(0, 4);
                                            rewardText[rewardText.size() - 1].text += rewardTextWords[r];
                                            rewardText[rewardText.size() - 1].pos = Objects[i].pos;
                                            rewardText[rewardText.size() - 1].timer = 20;
                                            Objects.erase(Objects.begin() + i);
                                        }
                                        else if (Objects[i].texId == 18) {
                                            r = randint(0, 4);
                                            if (r == 0) {
                                                Characters[selectedChar].maxHp += randint(1, 2);
                                            }if (r == 1) {
                                                Characters[selectedChar].damage += randint(1, 2);
                                            }if (r == 2) {
                                                Characters[selectedChar].biuldRange++;
                                            }if (r == 3) {
                                                Characters[selectedChar].atkRange++;
                                                Characters[selectedChar].vision++;
                                            }if (r == 4) {
                                                Characters[selectedChar].maxMoves++;
                                                Characters[selectedChar].damage *= 0.75;
                                            }

                                            Characters[selectedChar].hp += int(Characters[selectedChar].maxHp / 4);
                                            if (Characters[selectedChar].hp > Characters[selectedChar].maxHp) {
                                                Characters[selectedChar].hp = Characters[selectedChar].maxHp;
                                            }
                                            rewardText[rewardText.size() - 1].text += rewardTextWords[r + 5];
                                            rewardText[rewardText.size() - 1].pos = Objects[i].pos;
                                            rewardText[rewardText.size() - 1].timer = 20;
                                            Objects.erase(Objects.begin() + i);
                                        }
                                    }
                                }
                            }

                            blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] = 1;
                            blockedTiles[int(Characters[selectedChar].pos.y + 16)][int(Characters[selectedChar].pos.x + 16)] = 0;
                            Characters[selectedChar].pos = selectedTile;
                            Characters[selectedChar].moves--;


                            for (int j = 0; j < Characters.size() - 1; j++) {
                                if (Characters[j].pos.y > Characters[j + 1].pos.y) {
                                    if (selectedChar == j) selectedChar++;
                                    else if (selectedChar == j + 1) selectedChar--;
                                    std::swap(Characters[j], Characters[j + 1]);
                                }
                            }
                        }if (action == 1 && !foundChar && distance(selectedTile, Characters[selectedChar].pos) <= Characters[selectedChar].atkRange) {
                            for (int i = 0; i < Characters.size(); i++) {
                                if (selectedTile.x == Characters[i].pos.x && selectedTile.y == Characters[i].pos.y) {
                                    Characters[i].hp -= Characters[selectedChar].damage;
                                    Characters[selectedChar].moves--;
                                    if (Characters[selectedChar].trap == 1) {
                                        Characters[selectedChar].hp = 0;

                                        rewardText.push_back(RewardText{});
                                        rewardText[rewardText.size() - 1].text += "[-" + to_string(Characters[selectedChar].maxHp) + "]";
                                        rewardText[rewardText.size() - 1].pos = Characters[selectedChar].pos;
                                        rewardText[rewardText.size() - 1].timer = 20;
                                        rewardText[rewardText.size() - 1].color = sf::Color::Red;
                                    }

                                    rewardText.push_back(RewardText{});
                                    rewardText[rewardText.size() - 1].text += "[-" + to_string(Characters[selectedChar].damage) + "]";
                                    rewardText[rewardText.size() - 1].pos = Characters[i].pos;
                                    rewardText[rewardText.size() - 1].timer = 20;
                                    rewardText[rewardText.size() - 1].color = sf::Color::Red;

                                }
                            }

                            //Remove dead
                            for (int i = 0; i < Characters.size(); i++) {
                                if (Characters[i].hp <= 0) {
                                    blockedTiles[int(Characters[i].pos.y + 16)][int(Characters[i].pos.x + 16)] = 0;
                                    Characters.erase(Characters.begin() + i);
                                    if (Characters[i].team == 2) { enemiesLeft--; }
                                    if (selectedChar == i) selectedChar = -1;
                                    else if (selectedChar > i) selectedChar--;
                                    i--;
                                }
                            }


                        }

                        //Biuld
                        if (action == 2 && !foundChar && blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 0 && distance(selectedTile, Characters[selectedChar].pos) <= Characters[selectedChar].biuldRange && Characters[selectedChar].biuld == 1) {

                            if (biuldCheck) {
                                Characters[selectedChar].moves--;
                                blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] = 1;
                                for (int i = 0; i < 5; i++) {
                                    resources[i] -= recepies[biuldSelect][i];
                                }
                                Characters.push_back(Character(sf::Vector2f(selectedTile.x, selectedTile.y), charNum[0] + charNum[1] + biuldSelect, 0));
                            }
                        }

                        //Destroy
                        else if (action == 2 && !foundChar && blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] == 3 && distance(selectedTile, Characters[selectedChar].pos) <= Characters[selectedChar].biuldRange) {
                            for (int i = 0; i < Objects.size(); i++) {
                                if (selectedTile.x == Objects[i].pos.x && selectedTile.y == Objects[i].pos.y) {
                                    rewardText.push_back(RewardText{});
                                    if (Objects[i].texId == 5) { resources[0] += randint(2, 4); rewardText[rewardText.size() - 1].text += rewardTextWords[0]; }
                                    if (Objects[i].texId == 6) { resources[1] += randint(1, 3); rewardText[rewardText.size() - 1].text += rewardTextWords[1]; if (randint(0, 2) == 0) { r = randint(1, 4); resources[r]++; rewardText[rewardText.size() - 1].text += rewardTextWords[r]; } }
                                    if (Objects[i].texId == 19) { resources[2] += randint(1, 3); resources[1]++; rewardText[rewardText.size() - 1].text += rewardTextWords[2] + rewardTextWords[1]; }
                                    if (Objects[i].texId == 20) { resources[3] += randint(1, 3); resources[1]++; rewardText[rewardText.size() - 1].text += rewardTextWords[3] + rewardTextWords[1]; }
                                    if (Objects[i].texId == 21) { resources[4] += randint(1, 3); resources[1]++; rewardText[rewardText.size() - 1].text += rewardTextWords[4] + rewardTextWords[1]; }


                                    rewardText[rewardText.size() - 1].pos = Objects[i].pos;
                                    rewardText[rewardText.size() - 1].timer = 20;
                                    Objects.erase(Objects.begin() + i);
                                    Characters[selectedChar].moves--;
                                    blockedTiles[int(selectedTile.y + 16)][int(selectedTile.x + 16)] = 0;


                                }
                            }
                        }

                        //CHECK EXLAMATION
                        for (int i = 0; i < Characters.size(); i++) {
                            Characters[i].inRange = false;
                            if (Characters[i].team == 1) {
                                for (int j = 0; j < Characters.size() - 1; j++) {
                                    if (Characters[j].team == 0) {
                                        if (distance(Characters[j].pos, Characters[i].pos) <= Characters[i].atkRange && Characters[i].maxMoves == 1) {
                                            Characters[i].inRange = true;
                                        }if (distance(Characters[j].pos, Characters[i].pos) <= Characters[i].atkRange + 1.5 && Characters[i].maxMoves == 2) {
                                            Characters[i].inRange = true;
                                        }
                                    }
                                }
                            }
                        }


                        //Check Biuld
                        biuldCheck = true;
                        for (int i = 0; i < 5; i++) {
                            if (resources[i] < recepies[biuldSelect][i]) {
                                biuldCheck = false;
                            }
                        }
                    }

                    pressEnter = true;
                    for (int i = 0; i < Characters.size(); i++) {
                        if (Characters[i].moves > 0 && Characters[i].team == 0) {
                            pressEnter = false;
                        }
                    }

                    mouseL = false;

                }

                //Enemy (on enter key)
                else if ((keyEnter && b) || movingEnemy) {
                    if (selectedEnemy == -1) {
                        update = true;
                        movingEnemy = true;
                        waveCooldown = 0;
                        pressEnter = false;
                    }

                    if (selectedEnemy < int(Characters.size() - 1)) {
                        if (cooldown == 0) {

                            selectedEnemy++;
                            trapped = 0;

                            if (Characters[selectedEnemy].team == 1) {
                                if (visionList[int(Characters[selectedEnemy].pos.y + 16)][int(Characters[selectedEnemy].pos.x + 16)] != 3) {
                                    cooldown = 60;
                                }
                                while (Characters[selectedEnemy].moves > 0) {
                                    min = 9999;
                                    selectedChar = -1;


                                    //Find Closest Player
                                    for (int I = 0; I < Characters.size(); I++) {
                                        if (Characters[I].team == 0 && distance(Characters[selectedEnemy].pos, Characters[I].pos) < min && (Characters[I].trap == 0 && Characters[selectedEnemy].trap != 2)) {
                                            min = distance(Characters[selectedEnemy].pos, Characters[I].pos);
                                            selectedChar = I;
                                        }
                                    }


                                    if (selectedChar != -1) {
                                        Characters[selectedEnemy].inRange = false;
                                        if (min <= Characters[selectedEnemy].atkRange) {
                                            Characters[selectedChar].hp -= Characters[selectedEnemy].damage;
                                            for (int i = 0; i < Characters.size(); i++) {
                                                if (Characters[i].hp <= 0) {
                                                    blockedTiles[int(Characters[i].pos.y + 16)][int(Characters[i].pos.x + 16)] = 0;
                                                    Characters.erase(Characters.begin() + i);
                                                    i--;
                                                }

                                            }
                                            Characters[selectedEnemy].inRange = true;
                                        }
                                        else {
                                            blockedTiles[int(Characters[selectedEnemy].pos.y + 16)][int(Characters[selectedEnemy].pos.x + 16)] = 0;
                                            Characters[selectedEnemy].pos = bestMove(Characters[selectedEnemy].pos, Characters[selectedChar].pos, blockedTiles);
                                            blockedTiles[int(Characters[selectedEnemy].pos.y + 16)][int(Characters[selectedEnemy].pos.x + 16)] = 2;
                                        }

                                        //Keep going till ur close
                                        if (min < 10) {
                                            Characters[selectedEnemy].moves--;
                                        }
                                        else {
                                            trapped++;
                                            if (trapped > 50) {
                                                Characters[selectedEnemy].moves--;
                                            }
                                        }

                                        if (distance(Characters[selectedEnemy].pos, Characters[selectedChar].pos) <= Characters[selectedEnemy].atkRange && Characters[selectedEnemy].maxMoves == 1) {
                                            Characters[selectedEnemy].inRange = true;
                                        }if (distance(Characters[selectedEnemy].pos, Characters[selectedChar].pos) <= Characters[selectedEnemy].atkRange + 1.5 && Characters[selectedEnemy].maxMoves == 2) {
                                            Characters[selectedEnemy].inRange = true;
                                        }
                                    }
                                }

                            }
                            if (Characters[selectedEnemy + 1].team == 1 && selectedEnemy < int(Characters.size() - 1)) {
                                if (visionList[int(Characters[selectedEnemy + 1].pos.y + 16)][int(Characters[selectedEnemy + 1].pos.x + 16)] != 3) {
                                    cooldown = 25;
                                }
                            }
                        }
                        else {
                            cooldown--;
                        }

                    }
                    else {
                        b = false;
                        update = true;
                        movingEnemy = false;
                        selectedEnemy = -1;
                        foundChar = false;
                        for (int i = 0; i < Characters.size(); i++) {
                            Characters[i].moves = Characters[i].maxMoves;
                        }

                        //Wave managment
                        if (enemiesLeft < 1) {
                            wave++;
                            waveCooldown = 240;
                            difficulty *= 1.35;
                            for (int j = 0; j < difficulty * 4; j++) {
                                position = { float(randint(13,16) * rands()), float(randint(13,16) * rands()) };
                                while (blockedTiles[int(position.y + 16)][int(position.x + 16)] != 0) {
                                    position = { float(randint(13,16) * rands()), float(randint(13,16) * rands()) };
                                }


                                r = randint(4, 6);
                                if (wave > 2) {
                                    r = randint(4, 7);
                                }if (wave > 4) {
                                    r = randint(4, 8);
                                }if (wave > 6) {
                                    r = randint(4, 9);
                                }


                                Characters.push_back(Character(position, r, 1));
                                blockedTiles[int(position.y + 16)][int(position.x + 16)] = 2;
                                enemiesLeft++;
                            }
                        }

                        for (int j = 0; j < Characters.size() - 1; j++) {
                            if (Characters[j].pos.y > Characters[j + 1].pos.y) {
                                std::swap(Characters[j], Characters[j + 1]);
                            }
                        }

                        for (int i = 0; i < Characters.size(); i++) {
                            if (Characters[i].hp < Characters[i].maxHp) Characters[i].hp++;
                        }
                    }

                    selectedChar = -1;
                    mouseL = false;

                }
                else if (keyEsc) {
                    if (selectedChar != -1) {
                        selectedChar = -1;
                        update = true;
                    }
                }
                else if (key1) {
                    action = 0;
                    update = true;
                }
                else if (key2) {
                    action = 1;
                    update = true;
                }
                else if (key3) {
                    action = 2;
                    update = true;
                }

                //wave text
                if (waveCooldown > 0) {
                    waveCooldown--;
                }



                //Character Select


                //update tiles
                if (update) {
                    for (int i = 0; i < 33; i++) {
                        for (int j = 0; j < 33; j++) {
                            min = 3;
                            for (int I = 0; I < Characters.size(); I++) {
                                if (Characters[I].team == 0) {
                                    if (distance(sf::Vector2f(j - 16, i - 16), Characters[I].pos) <= Characters[I].vision) {
                                        min = 0;
                                    }
                                    else if (distance(sf::Vector2f(j - 16, i - 16), Characters[I].pos) <= Characters[I].vision + 1 && min > 1) {
                                        min = 1;
                                    }
                                    else if (distance(sf::Vector2f(j - 16, i - 16), Characters[I].pos) <= Characters[I].vision + 2 && min > 2) {
                                        min = 2;
                                    }
                                }
                            }

                            if (selectedChar != -1 && min == 0) {
                                if (distance(sf::Vector2f(j - 16, i - 16), Characters[selectedChar].pos) <= Characters[selectedChar].moveRange && Characters[selectedChar].moves > 0 && action == 0) {
                                    if (blockedTiles[i][j] == 0) {
                                        min = 4;
                                    }
                                    else if (blockedTiles[i][j] == 4) {
                                        min = 7;
                                    }
                                }if (distance(sf::Vector2f(j - 16, i - 16), Characters[selectedChar].pos) <= Characters[selectedChar].atkRange && Characters[selectedChar].moves > 0 && action == 1) {
                                    if (blockedTiles[i][j] == 0) {
                                        min = 5;
                                    }
                                    if ((blockedTiles[i][j] == 2 && Characters[selectedChar].team == 0) || (blockedTiles[i][j] == 1 && Characters[selectedChar].team == 1)) {
                                        min = 6;
                                    }
                                }if (distance(sf::Vector2f(j - 16, i - 16), Characters[selectedChar].pos) <= Characters[selectedChar].biuldRange && Characters[selectedChar].moves > 0 && action == 2) {
                                    if (blockedTiles[i][j] == 0) {
                                        min = 8;
                                    }
                                    else if (blockedTiles[i][j] == 3) {
                                        min = 6;
                                    }
                                }
                            }


                            visionList[i][j] = min;
                        }
                    }

                    update = false;
                }
            }
            last = now;
        }
        // Update logic here

        // Render
        window.clear(sf::Color::Black);
        window.setView(Camera);

        //Bacground
        window.draw(Background);

        //Objects
        for (int i = 0; i < Objects.size(); i++) {
            if ((visionList[int(Objects[i].pos.y + 16)][int(Objects[i].pos.x + 16)] != 3 && Objects[i].id == 1) || Objects[i].id == 0) {
                Tile.setPosition(Objects[i].pos.x * 16, Objects[i].pos.y * 16);
                Tile.setTexture(AssetsTex[Objects[i].texId]);
                window.draw(Tile);
            }
        }

        //Characters
        for (int i = 0; i < Characters.size(); i++) {
            if (visionList[int(Characters[i].pos.y + 16)][int(Characters[i].pos.x + 16)] != 3) {
                Entity.setPosition(Characters[i].pos.x * 16, Characters[i].pos.y * 16);
                Entity.setTexture(CharacterTex[Characters[i].id][Characters[i].texId]);
                if (selectedChar == i) {
                    if (Characters[selectedChar].team == 0) {
                        Tile.setTexture(AssetsTex[3]);
                    }
                    else {
                        Tile.setTexture(AssetsTex[2]);
                    }
                    Tile.setPosition(Entity.getPosition());
                    window.draw(Tile);
                }
                else if (action == 1 && selectedChar != -1) {

                }
                window.draw(Entity);

                HealthBar.setPosition(Entity.getPosition());
                window.draw(HealthBar);

                Bar.setPosition(Entity.getPosition());
                Bar.setSize(sf::Vector2f(float(Characters[i].hp) / float(Characters[i].maxHp) * 9.f + 1, 1));
                window.draw(Bar);

            }
        }

        for (int i = 0; i < Characters.size(); i++) {
            if (Characters[i].inRange) {
                Tile.setPosition(Characters[i].pos.x * 16, Characters[i].pos.y * 16 - 10);
                Tile.setTexture(AssetsTex[16]);
                window.draw(Tile);
            }
        }


        //Fog

        for (int i = 0; i < 33; i++) {
            for (int j = 0; j < 33; j++) {
                Fog.setPosition(j * 16 - 16 * 16, i * 16 - 16 * 16);

                if (visionList[i][j] == 3) {
                    Fog.setFillColor(sf::Color(0, 0, 0, 200));
                    window.draw(Fog);
                }
                else if (visionList[i][j] == 2) {
                    Fog.setFillColor(sf::Color(0, 0, 0, 180));
                    window.draw(Fog);
                }
                else if (visionList[i][j] == 1) {
                    Fog.setFillColor(sf::Color(0, 0, 0, 130));
                    window.draw(Fog);
                }
                else if (visionList[i][j] == 4) {
                    Fog.setFillColor(sf::Color(120, 120, 120, 90));
                    window.draw(Fog);
                }
                else if (visionList[i][j] == 5) {
                    Fog.setFillColor(sf::Color(220, 20, 20, 30));
                    window.draw(Fog);
                }
                else if (visionList[i][j] == 6) {
                    //cout << visionList[int(Characters[i].pos.y + 16)][int(Characters[i].pos.x + 16)] << " = " << i << endl;
                    Tile.setPosition(Fog.getPosition());
                    Tile.setTexture(AssetsTex[2]);
                    window.draw(Tile);
                }
                else if (visionList[i][j] == 7) {
                    Tile.setPosition(Fog.getPosition());
                    Tile.setTexture(AssetsTex[4]);
                    window.draw(Tile);
                }
                else if (visionList[i][j] == 8) {
                    Fog.setFillColor(sf::Color(220, 220, 20, 50));
                    window.draw(Fog);
                }

                if (debug) {
                    text.setCharacterSize(6);
                    text.setScale(1, 1);
                    text.setPosition(j * 16 - 16 * 16 - 8, i * 16 - 16 * 16 - 8);
                    text.setString(to_string(visionList[i][j]));
                    text.setFillColor(sf::Color::White);
                    window.draw(text);
                    text.setPosition(j * 16 - 16 * 16 + 0, i * 16 - 16 * 16 + 0);
                    text.setString(to_string(blockedTiles[i][j]));
                    text.setFillColor(sf::Color::Red);
                    window.draw(text);
                }
            }
        }

        if (selectedChar != -1) {

            UI.setPosition(cameraPos);
            UI.setScale(zoom, zoom);
            UI.setTexture(AssetsTex[15]);
            window.draw(UI);

            text.setCharacterSize(6);
            text.setScale(zoom, zoom);
            text.setFillColor(sf::Color::Black);

            text.setString(Characters[selectedChar].name);
            text.setPosition(cameraPos.x + 170 * zoom, cameraPos.y - 120 * zoom);
            window.draw(text);

            text.setString("Moves: " + std::to_string(Characters[selectedChar].moves) + "/" + std::to_string(Characters[selectedChar].maxMoves));
            text.setPosition(cameraPos.x + 167 * zoom, cameraPos.y - 105 * zoom);
            window.draw(text);

            text.setString("Life: " + std::to_string(Characters[selectedChar].hp) + "/" + std::to_string(Characters[selectedChar].maxHp));
            text.setPosition(cameraPos.x + 167 * zoom, cameraPos.y - 95 * zoom);
            window.draw(text);

            text.setString("Damage: " + std::to_string(Characters[selectedChar].damage));
            text.setPosition(cameraPos.x + 167 * zoom, cameraPos.y - 85 * zoom);
            window.draw(text);

            text.setString("Vision: " + std::to_string(int(Characters[selectedChar].vision)));
            text.setPosition(cameraPos.x + 167 * zoom, cameraPos.y - 75 * zoom);
            window.draw(text);

            if (action == 2) {
                UI.setPosition(cameraPos);
                UI.setScale(zoom, zoom);
                UI.setTexture(AssetsTex[22]);
                window.draw(UI);

                text.setCharacterSize(12);
                text.setScale(zoom, zoom);
                text.setFillColor(sf::Color::Black);

                for (int i = 0; i < 5; i++) {
                    text.setString(std::to_string(resources[i]));
                    text.setPosition(cameraPos.x - 209 * zoom, cameraPos.y - (116 - i * 20) * zoom);
                    window.draw(text);
                }

                if (Characters[selectedChar].biuld == 1) {
                    UI.setTexture(AssetsTex[23]);
                    window.draw(UI);

                    UI.move(sf::Vector2f(0, 20 * biuldSelect));
                    UI.setTexture(AssetsTex[24 + int(!biuldCheck)]);
                    window.draw(UI);

                }

            }
        }

        if (pressEnter) {
            text.setString("Press enter for next turn");
            text.setCharacterSize(12);
            text.setScale(zoom, zoom);
            text.setPosition(cameraPos.x - text.getGlobalBounds().width / 2, cameraPos.y - text.getGlobalBounds().height / 2);

            text.setFillColor(sf::Color::White);
            window.draw(text);
        }

        if (waveCooldown > 0) {
            text.setString("Wave " + to_string(wave));
            text.setCharacterSize(30);
            text.setScale(zoom, zoom);
            text.setPosition(cameraPos.x - text.getGlobalBounds().width / 2, cameraPos.y - text.getGlobalBounds().height / 2);

            text.setFillColor(sf::Color::Red);
            window.draw(text);
        }

        if (movingEnemy && enemiesLeft > 0) {
            text.setString("Enemy moving... ");
            text.setCharacterSize(18);
            text.setScale(zoom, zoom);
            text.setPosition(cameraPos.x - text.getGlobalBounds().width / 2, cameraPos.y - text.getGlobalBounds().height / 2);

            text.setFillColor(sf::Color::Red);
            window.draw(text);
        }

        for (int i = 0; i < 3; i++) {
            UI.setPosition(cameraPos.x + (i - 1) * 50 * zoom, cameraPos.y + 10 * zoom);
            UI.setScale(zoom, zoom);
            UI.setTexture(AssetsTex[10 + i]);
            if (action == i) UI.setPosition(cameraPos.x + (i - 1) * 50 * zoom, cameraPos.y);
            window.draw(UI);
        } 

        for (int i = 0; i < rewardText.size(); i++) {
            text.setString(rewardText[i].text);
            text.setCharacterSize(6);
            text.setScale(zoom, zoom);
            text.setPosition(rewardText[i].pos.x * 16 - (text.getGlobalBounds().width / 2), rewardText[i].pos.y * 16 - (21 - rewardText[i].timer));

            text.setFillColor(rewardText[i].color);
            window.draw(text);
        }

        if (infoText != "") {
            text.setString(infoText);
            text.setCharacterSize(6);
            text.setScale(zoom * 0.8, zoom * 0.8);
            text.setPosition(mousePos.x + 3 * zoom, mousePos.y + 3 * zoom);

            text.setFillColor(sf::Color::Black);
            window.draw(text);
        }



        //text.setPosition(mousePos);
        //text.setScale(zoom, zoom);
        //window.draw(text);

        window.display();
    }

    return 0;
}
