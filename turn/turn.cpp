#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

class player {
private:
    int life = 100;
    int energy = 20;
    bool isDefending = false;

public:
    int randomNumber() {
        int random = rand() % 35;
        return random;
    }

    int getLife() {
        return life;
    }

    int getEnergy() {
        return energy;
    }

    void takeDamage(int damage) {
        life -= damage;
    }

    void lessEnergy() {
        energy -= 5;
    }

    bool isDefendingNow() const { // Apenas verifica se está defendendo ou não, por isso é const
        return isDefending;
    }

    void startDefending() {
        isDefending = true;
    }

    void stopDefending() {
        isDefending = false;
    }

    void defend() {
        int sorte = rand() % 2;

        switch (sorte) {
        case 0:
            std::cout << "Defense failed" << std::endl;
            stopDefending(); // // Se a defesa falhar, desativa a defesa
            break;
        case 1:
            std::cout << "Succesfull defense, reducing the damage" << std::endl;
            // FALTA TERMINAR AQUI, NÃO SEI SE FUNCIONA
            if (isDefendingNow()) {
                int reduction = randomNumber();
                int originalDamage = randomNumber();
                int reducedDamage = originalDamage - reduction; // Vai reduzir o dano de acordo com um valor aleatório

                if (reducedDamage < 0) {
                    reducedDamage = 0; // O dano reduzido não vai ser menor do que 0
                }

                lessEnergy();
            }

            stopDefending();
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
            defend();
            return "defend";
            break;
        case 2:
            rest();
            return "rest";
            break;
        case 3:
            heal();
            return "heal";
            break;
        }
    }
};

int main()
{
    srand(time(NULL));
    player newPlayer; // Cria o objeto do jogador
    player newEnemy; // Cria o objeto do inimigo
    std::string action;

    std::cout << "--------INICIANDO A BATALHA--------" << std::endl << std::endl;

    while (newPlayer.getLife() > 0 && newEnemy.getLife() > 0 && newPlayer.getEnergy() > 0 && newEnemy.getEnergy() > 0) {
        int damageP = newPlayer.randomNumber();
        int damageE = newEnemy.randomNumber();

        std::cout << "[PLAYER]" << std::endl 
                  << "Health: " <<  newPlayer.getLife() << std::endl 
                  << "Energy: " << newPlayer.getEnergy() << std::endl << std::endl
                  << "[ENEMY]" << std::endl
                  << "Health: " << newEnemy.getLife() << std::endl
                  << "Energy: " << newEnemy.getEnergy() << std::endl;
        
        std::cout << "Your turn. Choose an action: attack, defend, rest, heal" << std::endl;
        std::cin >> action;

        if (action == "attack") {
            // ACHO QUE DÁ PRA COLOCAR UM IF COM A DEFESA AQUI
            if (damageP > 26) {
                std::cout << "You attacked and caused " << damageP << " of damage (CRITICAL)" << std::endl;
            } else {
                std::cout << "You attacked and caused " << damageP << " of damage" << std::endl;
            }
            newEnemy.takeDamage(damageP);
            newPlayer.lessEnergy();
        } else if (action == "defend") {
            newPlayer.startDefending(); // Ativar a defesa
            newPlayer.defend(); // Chamar o método de defesa
        } else if (action == "rest") {
            newPlayer.rest();
        } else if (action == "heal") {
            newPlayer.heal();
        }
        
        std::cout << std::endl << "==================================" << std::endl << std::endl;
        
        std::cout << "[PLAYER]" << std::endl
                  << "Health: " << newPlayer.getLife() << std::endl
                  << "Energy: " << newPlayer.getEnergy() << std::endl << std::endl
                  << "[ENEMY]" << std::endl
                  << "Health: " << newEnemy.getLife() << std::endl
                  << "Energy: " << newEnemy.getEnergy() << std::endl;

        std::string enemyOption = newEnemy.randomOption();
        std::cout << "Enemy's turn. He chose: " << enemyOption << std::endl;

        if (enemyOption == "attack") {
            if (newPlayer.isDefendingNow()) {
                std::cout << "Enemy attacked, but your defense reduced the damage!" << std::endl;
            } else if (damageE > 26) {
                std::cout << "Enemy attacked and caused " << damageE << " of damage (CRITICAL)" << std::endl;
            }
            else {
                std::cout << "Enemy attacked and caused " << damageE << " of damage" << std::endl;
            }
            newPlayer.takeDamage(damageE);
            newEnemy.lessEnergy();
        }

        std::cout << std::endl << "==================================" << std::endl << std::endl;
        // Preciso descobrir como colocar essa parte em uma função

        // TRADUZIR PARA INGLÊS

        // QUERO COLOCAR CORES NO TERMINAL

        // Faltam algumas coisas, bom revisar tudo antes de terminar
    }

    if (newPlayer.getLife() > 0 && newPlayer.getEnergy() > 0) {
        std::cout << "You won the battle with " << newPlayer.getLife() << " health points remaining!" << std::endl;
    } else if (newEnemy.getLife() > 0 && newEnemy.getEnergy() > 0) {
        std::cout << "The enemy won the battle with " << newEnemy.getLife() << " health points remaining!" << std::endl;
    } else {
        std::cout << "It's a Draw!!!" << std::endl; // Caso 1 jogador perca toda a vida mas o outro perca a energia
    }

    return 0;
}