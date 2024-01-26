#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <windows.h> // Para mudar as cores dos textos no terminal
#define UNDERLINE "\033[4m"
#define CLOSEUNDERLINE "\033[0m"

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // Para mudar as cores dos textos

class player {
private:
    int life = 100;
    int energy = 20;
    bool isDefending = false;
    double defenseMultiplier = 1.0; // Multiplicador para reduzir o dano caso esteja defendendo

public:
    int randomNumber() {
        int random = rand() % 35; // O dano vai ser entre 0 e 35
        return random;
    }

    int getLife() const{ // Apenas verifica a variável sem mudar seu valor, por isso é const
        return life;
    }

    int getEnergy() const{ 
        return energy;
    }

    void takeDamage(int damage) {
        life -= damage;
    }

    void lessEnergy() {
        energy -= 5;
    }

    double getDefenseMultiplier() const {
        return defenseMultiplier;
    }

    void resetDefenseMultiplier() {
        defenseMultiplier = 1.0;
    }

    bool isDefendingNow() const {
        return isDefending;
    }

    void startDefending() {
        isDefending = true;
    }

    void stopDefending() {
        isDefending = false;
    }

    void defend() {
        int sorte = rand() % 4; // 25% de chance de dar erro

        switch (sorte) {
        case 0:
            std::cout << UNDERLINE << "Defense failed" << CLOSEUNDERLINE << std::endl;
            stopDefending(); // Se a defesa falhar, desativa o estado de defesa
            lessEnergy();
            break;
        case 1:
        case 2:
        case 3:
            std::cout << UNDERLINE << "Successful defense, reducing the next damage" << CLOSEUNDERLINE << std::endl;
            // Atualiza o multiplicador de defesa para reduzir o dano do próximo ataque
            defenseMultiplier = 0.5; // Reduz o dano pela metade durante a defesa
            startDefending(); // Ativa o estado de defesa
            lessEnergy();
            break;
        }
    }

    void rest() {
        energy += 5;
    }

    void heal() {
        life += 15;
        lessEnergy();
    }

    std::string randomOption() {
        int option = rand() % 4;

        switch (option) {
        case 0:
            return "attack";
            break;
        case 1:
            return "defend";
            break;
        case 2:
            return "rest";
            break;
        case 3:
            return "heal";
            break;
        }
    }
};

// Função para agir de acordo com a escolha do jogador
void playerTurn(player& newPlayer, player& newEnemy) {
    int damageP = newPlayer.randomNumber();  
    int damageDealt = damageP * newEnemy.getDefenseMultiplier();
    std::string action;

    do {
        std::cout << "> Your turn. Choose an action: attack, defend, rest, heal" << std::endl << "> ";
        std::cin >> action;

        if (action == "attack") {
            if (newEnemy.isDefendingNow() && damageDealt > 5) {
                std::cout << UNDERLINE << "You attacked, but enemy's defense reduced the damage!" << CLOSEUNDERLINE << std::endl;
            }

            if (damageDealt > 28) {
                std::cout << UNDERLINE << "You attacked and caused " << damageDealt << " of damage (CRITICAL)" << CLOSEUNDERLINE << std::endl;
            } else if (damageDealt <= 5) { // Caso o número aleatório seja até 5 vai ser considerado um erro no ataque
                damageDealt = 0;
                std::cout << UNDERLINE << "You missed the attack" << CLOSEUNDERLINE << std::endl;
            } else {
                std::cout << UNDERLINE << "You attacked and caused " << damageDealt << " of damage" << CLOSEUNDERLINE << std::endl;
            }
            newEnemy.takeDamage(damageDealt);
            newPlayer.lessEnergy();
        } else if (action == "defend") {
            newPlayer.startDefending(); // Ativa a defesa
            newPlayer.defend(); // Chama o método de defesa
        } else if (action == "rest") {
            newPlayer.rest();
            std::cout << UNDERLINE << "Gained 5 points of stamina after taking a short break" << CLOSEUNDERLINE << std::endl;
        } else if (action == "heal") {
            newPlayer.heal();
            std::cout << UNDERLINE << "Healed 15 points of life" << CLOSEUNDERLINE << std::endl;
        } else {
            std::cout << "Invalid option. Please choose a valid action." << std::endl;
        }
    } while (action != "attack" && action != "defend" && action != "rest" && action != "heal");
}

// Função para agir de acordo com a escolha do inimigo
void enemyTurn(player& newPlayer, player& newEnemy) {
    int damageE = newEnemy.randomNumber();
    std::string enemyOption = newEnemy.randomOption();
    std::cout << "> Enemy's turn. They chose: " << enemyOption << std::endl;

    if (enemyOption == "defend") { // NÃO FUNCIONAVA SE NÃO FOSSE ASSIM
        newEnemy.defend();
    } else if (enemyOption == "attack") {
        int damageTaken = damageE * newPlayer.getDefenseMultiplier();
        if (newPlayer.isDefendingNow() && damageTaken > 5) {
            std::cout << UNDERLINE << "Enemy attacked, but your defense reduced the damage!" << CLOSEUNDERLINE << std::endl;
        }

        if (damageTaken > 28) {
            std::cout << UNDERLINE << "Enemy attacked and caused " << damageTaken << " of damage (CRITICAL)" << CLOSEUNDERLINE << std::endl;
        } else if (damageTaken <= 5) { // Caso o número aleatório seja até 5 vai ser considerado um erro no ataque
            damageTaken = 0;
            std::cout << UNDERLINE << "Enemy missed the attack" << CLOSEUNDERLINE << std::endl;
        } else {
            std::cout << UNDERLINE << "Enemy attacked and caused " << damageTaken << " of damage" << CLOSEUNDERLINE << std::endl;
        }
        newPlayer.takeDamage(damageTaken);
        newEnemy.lessEnergy();

        // Redefine o multiplicador de defesa para o próximo turno do jogador
        newPlayer.stopDefending();
        newPlayer.resetDefenseMultiplier();
    } else if (enemyOption == "rest") {
        newEnemy.rest();
        std::cout << UNDERLINE << "Gained 5 points of stamina after taking a short break" << CLOSEUNDERLINE << std::endl;
    } else if (enemyOption == "heal") {
        newEnemy.heal();
        std::cout << UNDERLINE << "Healed 15 points of life" << CLOSEUNDERLINE << std::endl;
    }
}

// Função para exibir o estado atual da batalha
void displayBattleStatus(const player& newPlayer, const player& newEnemy) {
    SetConsoleTextAttribute(h,1);
    std::cout << "[PLAYER]" << std::endl;
    SetConsoleTextAttribute(h,2);
    std::cout << "Health: " << newPlayer.getLife() << std::endl;
    SetConsoleTextAttribute(h,11);
    std::cout << "Stamina: " << newPlayer.getEnergy() << std::endl << std::endl;
    SetConsoleTextAttribute(h,9);
    std::cout << "[ENEMY]" << std::endl;
    SetConsoleTextAttribute(h,2);
    std::cout << "Health: " << newEnemy.getLife() << std::endl;
    SetConsoleTextAttribute(h,11);
    std::cout << "Stamina: " << newEnemy.getEnergy() << std::endl;
    SetConsoleTextAttribute(h,7); // Default
}

// Função para exibir a mensagem de resultado final
void displayBattleResult(const player& newPlayer, const player& newEnemy) {
    SetConsoleTextAttribute(h,12);
    if (newPlayer.getLife() > 0 && newPlayer.getEnergy() > 0) {
        std::cout << "You won the battle with " << newPlayer.getLife() << " health points remaining!" << std::endl;
    } else if (newEnemy.getLife() > 0 && newEnemy.getEnergy() > 0) {
        std::cout << "The enemy won the battle with " << newEnemy.getLife() << " health points remaining!" << std::endl;
    } else {
        std::cout << "It's a Draw!!!" << std::endl; // Caso 1 jogador perca toda a vida mas o outro perca a estamina
    }

    // Adiciona mensagens sobre o motivo da perda
    if (newPlayer.getLife() <= 0) {
        std::cout << "Your life reached zero." << std::endl;
    } else if (newPlayer.getEnergy() <= 0) {
        std::cout << "Your stamina reached zero." << std::endl;
    } else if (newEnemy.getLife() <= 0) {
        std::cout << "The enemy's life reached zero." << std::endl;
    } else if (newEnemy.getEnergy() <= 0) {
        std::cout << "The enemy's stamina reached zero." << std::endl;
    }
    SetConsoleTextAttribute(h,7);
}

int main() {
    srand(time(NULL));
    player newPlayer; // Cria o objeto do jogador
    player newEnemy; // Cria o objeto do inimigo

    std::cout << "[STARTING BATTLE]" << std::endl << std::endl;

    while (newPlayer.getLife() > 0 && newEnemy.getLife() > 0 && newPlayer.getEnergy() > 0 && newEnemy.getEnergy() > 0) {        
        displayBattleStatus(newPlayer, newEnemy);
        
        playerTurn(newPlayer, newEnemy);

        std::cout << std::endl << "==================================" << std::endl << std::endl;

        if (newPlayer.getLife() <= 0 || newEnemy.getLife() <= 0 || newPlayer.getEnergy() <= 0 || newEnemy.getEnergy() <= 0) {
            break;
        }

        displayBattleStatus(newPlayer, newEnemy);

        enemyTurn(newPlayer, newEnemy);

        std::cout << std::endl << "==================================" << std::endl << std::endl;
        }

    displayBattleResult(newPlayer, newEnemy);

    return 0;
}