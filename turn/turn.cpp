#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <limits>
#define NOMINMAX // Para não dar conflito com o <streamsize>::max()
#include <windows.h> // Para mudar as cores dos textos no terminal
#define UNDERLINE "\033[4m" // Coloca underline
#define CLOSEUNDERLINE "\033[0m" // Encerra underline

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // Para mudar as cores dos textos

class player {
private:
    int life = 100;
    int energy = 20;
    bool hasHealed = false;
    bool gaveUp = false;
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

    void lessEnergy(int stamina) {
        energy -= stamina;
    }
    
    bool getHasHealed() const {
        return hasHealed;
    }

    bool hasGivenUp() const {
        return gaveUp;
    }

    void giveUp() {
        gaveUp = true;
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
            lessEnergy(3);
            break;
        case 1:
        case 2:
        case 3:
            std::cout << UNDERLINE << "Successful defense, reducing the next damage" << CLOSEUNDERLINE << std::endl;
            // Atualiza o multiplicador de defesa para reduzir o dano do próximo ataque
            defenseMultiplier = 0.5; // Reduz o dano pela metade durante a defesa
            startDefending(); // Ativa o estado de defesa
            lessEnergy(3);
            break;
        }
    }

    void rest() {
        energy += 5;
        if (energy > 20) {
            energy = 20;
        }
    }

    void heal() {
        life += 30;
        if (life > 100) {
            life = 100;
        }
        lessEnergy(4);
        hasHealed = true;
    }

    // Escolhe uma ação aleatória para o inimigo de acordo com sua vida e estamina
    std::string randomOption() {
        int option = rand() % 4;
        
        if ((life == 100 && energy == 20) || (hasHealed == true && energy == 20)) {
            switch (option) {
                case 0:
                case 1:
                    return "attack";
                    break;
                case 2:
                case 3:
                    return "defend";
                    break;
            }
        } else if ((life == 100 && energy < 20) || (hasHealed == true && energy < 20)) {
            switch (option) {
                case 0:
                    return "rest";
                    break;
                case 1:
                    return "defend";
                    break;
                case 2:
                case 3:
                    return "attack";
                    break;
            }
        } else if (life < 100 && energy == 20 && hasHealed == false) {
            switch (option) {
                case 0:
                    return "heal";
                    break;
                case 1:
                    return "defend";
                    break;
                case 2:
                case 3:
                    return "attack";
                    break;
            }
        } else {
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
    }
};

// Função para agir de acordo com a escolha do jogador
void playerTurn(player& newPlayer, player& newEnemy) {
    int damageP = newPlayer.randomNumber();  
    int damageDealt = damageP * newEnemy.getDefenseMultiplier();
    std::string action;

    do {
        std::cout << "> Your turn. Choose an action: attack, defend, rest, heal, give up" << std::endl << "> ";
        std::getline(std::cin, action);

        if (action == "attack" || action == "ATTACK") { // As ações podem ser com letras minúsculas ou maiúsculas
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
            newPlayer.lessEnergy(5);

            // Redefine o multiplicador de defesa para o próximo turno do inimigo
            newEnemy.stopDefending();
            newEnemy.resetDefenseMultiplier();
        } else if (action == "defend" || action == "DEFEND") {
            newPlayer.defend(); // Chama o método de defesa
        } else if (action == "rest" || action == "REST") {
            if (newPlayer.getEnergy() == 20) { // Se estiver com estamina máxima não é possível descansar
                std::cout << UNDERLINE << "Maximum stamina" << CLOSEUNDERLINE << std::endl;
                action = "";
            } else {
                newPlayer.rest();
                std::cout << UNDERLINE << "Gained 5 points of stamina after taking a short break" << CLOSEUNDERLINE << std::endl;
            }
        } else if (action == "heal" || action == "HEAL") { // Se estiver com vida máxima não é possível se curar
            if (newPlayer.getHasHealed() == true) {
                std::cout << UNDERLINE << "You can only heal one time per battle!" << CLOSEUNDERLINE << std::endl;
                action = "";
            } else if (newPlayer.getLife() == 100) {
                std::cout << UNDERLINE << "Maximum health" << CLOSEUNDERLINE << std::endl;
                action = "";
            } else {
                newPlayer.heal();
                std::cout << UNDERLINE << "Healed 30 points of life" << CLOSEUNDERLINE << std::endl;
            }
        } else if (action == "give up" || action == "GIVE UP" || action == "giveup" || action == "GIVEUP") {
            std::cout << UNDERLINE << "You chose to give up. The battle is over" << CLOSEUNDERLINE << std::endl;
            newPlayer.giveUp();
            break;
        } else {
            std::cout << "Invalid option. Please choose a valid action" << std::endl;
        }
    } while (action != "attack" && action != "ATTACK" && action != "defend" && action != "DEFEND" &&
             action != "rest" && action != "REST" && action != "heal" && action != "HEAL");
}

// Função para agir de acordo com a escolha do inimigo
void enemyTurn(player& newPlayer, player& newEnemy) {
    int damageE = newEnemy.randomNumber();
    std::string enemyOption = newEnemy.randomOption();
    std::cout << "> Enemy's turn. They chose: " << enemyOption << std::endl;

    if (enemyOption == "defend") { // NÃO FUNCIONAVA SE NÃO FOSSE ASSIM
        newEnemy.defend(); // Chama o método de defesa
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
        newEnemy.lessEnergy(5);

        // Redefine o multiplicador de defesa para o próximo turno do jogador
        newPlayer.stopDefending();
        newPlayer.resetDefenseMultiplier();
    } else if (enemyOption == "rest") {
        newEnemy.rest();
        std::cout << UNDERLINE << "Gained 5 points of stamina after taking a short break" << CLOSEUNDERLINE << std::endl;
    } else if (enemyOption == "heal") {
        newEnemy.heal();
        std::cout << UNDERLINE << "Healed 30 points of life" << CLOSEUNDERLINE << std::endl;
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
    if (newEnemy.getLife() <= 0 || newEnemy.getEnergy() <= 0) {
        std::cout << "You won the battle with " << newPlayer.getLife() << " health points remaining!" << std::endl;
    } else if (newPlayer.hasGivenUp()) { // Caso o jogador desista da partida
        std::cout << "You gave up, the enemy won with " << newEnemy.getLife() << " health points remaining!" << std::endl;
    } else if (newPlayer.getLife() <= 0 || newPlayer.getEnergy() <= 0) {
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
    bool loop;
    do {
        player newPlayer; // Cria o objeto do jogador
        player newEnemy; // Cria o objeto do inimigo

        std::cout << "[STARTING BATTLE]" << std::endl << std::endl;

        while (newPlayer.getLife() > 0 && newEnemy.getLife() > 0 && newPlayer.getEnergy() > 0 && newEnemy.getEnergy() > 0) {
            displayBattleStatus(newPlayer, newEnemy);

            playerTurn(newPlayer, newEnemy);

            std::cout << std::endl << "==================================" << std::endl << std::endl;

            if (newPlayer.getLife() <= 0 || newEnemy.getLife() <= 0 || newPlayer.getEnergy() <= 0 || newEnemy.getEnergy() <= 0) {
                break;
            } else if (newPlayer.hasGivenUp()) {
                break;
            }

            displayBattleStatus(newPlayer, newEnemy);

            enemyTurn(newPlayer, newEnemy);
            std::cout << std::endl << "==================================" << std::endl << std::endl;
        }

        displayBattleResult(newPlayer, newEnemy);
        std::string newBattle;

        do {
            std::cout << "> Do you want to start a new battle? (yes/no): "; 
            std::cin >> newBattle;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer de entrada
            std::cout << std::endl;
        
            if (newBattle == "no" || newBattle == "NO" || newBattle == "n" || newBattle == "N") {
                loop = false;
                break;  // Sai do loop principal se o jogador não quiser iniciar uma nova batalha
            } else if (newBattle == "yes" || newBattle == "YES" || newBattle == "y" || newBattle == "Y") {
                loop = true;
                break; // Continua o loop se o jogador desejar iniciar uma nova partida
            } else {
                std::cout << "Invalid option. Please enter 'yes' or 'no'." << std::endl;
            }
        } while (true);

       std::cout << std::endl;
    } while (loop);
    SetConsoleTextAttribute(h, 3);
    std::cout << "Thanks for playing!";
    SetConsoleTextAttribute(h, 7);
    return 0;
}