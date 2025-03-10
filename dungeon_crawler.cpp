#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

using namespace std;

class Monster {
public:
    string name;
    int health;
    int attack;
    int defense;

    Monster(string n, int h, int a, int d) : name(n), health(h), attack(a), defense(d) {}

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    bool isAlive() const { return health > 0; }
};

class Player {
public:
    string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    int potions;
    const int MAX_LEVEL = 20;

    Player(string n) : name(n), health(100), maxHealth(100), attack(10), 
                    defense(5), level(1), experience(0), potions(3) {}

    void takeDamage(int damage) {
        int actualDamage = damage - defense;
        if (actualDamage < 0) actualDamage = 0;
        health -= actualDamage;
        if (health < 0) health = 0;
    }

    void usePotion() {
        if (potions > 0) {
            health += 30;
            if (health > maxHealth) health = maxHealth;
            potions--;
            cout << "Used potion! Health: " << health << "/" << maxHealth << endl;
        } else {
            cout << "No potions left!\n";
        }
    }

    void gainExperience(int exp) {
        experience += exp;
        cout << "Gained " << exp << " XP!\n";
        if (experience >= neededExperience() && level < MAX_LEVEL) {
            levelUp();
        }
    }

    void levelUp() {
        level++;
        maxHealth += 20;
        health = maxHealth;
        attack += 5;
        defense += 2;
        cout << "LEVEL UP! You are now level " << level << "!\n";
        cout << "HP: " << maxHealth << " ATK: " << attack << " DEF: " << defense << endl;
    }

    int neededExperience() const {
        return level * 50;
    }

    bool isAlive() const { return health > 0; }

    void showStats() const {
        cout << "\n--- Player Stats ---\n"
            << "HP: " << health << "/" << maxHealth << "\n"
            << "ATK: " << attack << " DEF: " << defense << "\n"
            << "Level: " << level << " XP: " << experience << "/" << neededExperience() << "\n"
            << "Potions: " << potions << "\n\n";
    }
};

void clearScreen() {
    system("cls || clear");
}

Monster generateMonster(int playerLevel) {
    vector<string> names = {"Goblin", "Orc", "Skeleton", "Zombie", "Demon", "Dragon"};
    string name = names[rand() % names.size()];
    
    int baseHealth = 20 + (playerLevel * 5);
    int baseAttack = 5 + (playerLevel * 2);
    int baseDefense = 2 + (playerLevel * 1);
    
    return Monster(name, 
                baseHealth + (rand() % 10),
                baseAttack + (rand() % 5),
                baseDefense + (rand() % 3));
}

bool combat(Player& player, Monster& monster) {
    cout << "\nA wild " << monster.name << " appears!\n";
    
    while (monster.isAlive() && player.isAlive()) {
        cout << "\n" << player.name << " HP: " << player.health << " | "
            << monster.name << " HP: " << monster.health << "\n";
        cout << "1. Attack\n2. Use Potion\n3. Run\nChoice: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            int damage = player.attack + (rand() % 5);
            monster.takeDamage(damage);
            cout << "You deal " << damage << " damage!\n";
            
            if (monster.isAlive()) {
                int monsterDamage = monster.attack + (rand() % 3);
                player.takeDamage(monsterDamage);
                cout << monster.name << " deals " << monsterDamage << " damage!\n";
            }
        }
        else if (choice == 2) {
            player.usePotion();
        }
        else if (choice == 3) {
            if (rand() % 100 < 50) {
                cout << "You escaped successfully!\n";
                return true;
            }
            cout << "Escape failed!\n";
        }
        else {
            cout << "Invalid choice!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    if (player.isAlive()) {
        int exp = (monster.attack + monster.defense + monster.health) / 3;
        player.gainExperience(exp);
        cout << "You defeated the " << monster.name << "!\n";
        if (rand() % 100 < 30) {
            player.potions++;
            cout << "Found a potion!\n";
        }
        return true;
    }
    return false;
}

void gameLoop(Player& player) {
    srand(time(0));
    
    while (player.isAlive()) {
        cout << "\nWhat will you do?\n"
            << "1. Explore dungeon\n"
            << "2. Check stats\n"
            << "3. Quit game\n"
            << "Choice: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 1) {
            clearScreen();
            if (rand() % 100 < 70) { // 70% chance of combat
                Monster monster = generateMonster(player.level);
                if (!combat(player, monster)) {
                    break; // Player died
                }
            } else {
                if (rand() % 100 < 50) {
                    player.potions++;
                    cout << "Found a potion!\n";
                } else {
                    cout << "Nothing interesting here...\n";
                }
            }
        }
        else if (choice == 2) {
            clearScreen();
            player.showStats();
        }
        else if (choice == 3) {
            cout << "Thanks for playing!\n";
            exit(0);
        }
        else {
            cout << "Invalid choice!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    cout << "\nGAME OVER! You reached level " << player.level << "\n";
}

int main() {
    clearScreen();
    cout << "=== DUNGEON CRAWLER ===" << endl;
    cout << "Enter your name: ";
    string name;
    cin >> name;
    
    Player player(name);
    clearScreen();
    cout << "Welcome, " << player.name << "!\n";
    cout << "Controls:\n- Explore to find monsters and potions\n- Fight to gain XP and level up\n- Manage your potions wisely!\n\n";
    
    gameLoop(player);
    
    return 0;
}