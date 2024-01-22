#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

class player {
private:
    int life = 100;
    int energy = 20;

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

    void defend() {
        int sorte = rand() % 2;

        switch (sorte) {
        case 0:
            std::cout << "Defense failed" << std::endl;
            break;
        case 1:
            std::cout << "Succesfull defense, reducing the damage" << std::endl;
            // FALTA TERMINAR AQUI
            break;
        }
    }
};

int main()
{
    srand(time(0));
    player newPlayer; // Cria o objeto do jogador
    player newEnemy; // Cria o objeto do inimigo
    std::string action;
    std::cout << "--------INICIANDO A BATALHA--------" << std::endl << std::endl;

    while (newPlayer.getLife() > 0 && newEnemy.getLife() > 0) {
        int damageP = newPlayer.randomNumber();
        int damageE = newEnemy.randomNumber();

        std::cout << "[JOGADOR]" << std::endl 
                  << "Vida: " <<  newPlayer.getLife() << std::endl 
                  << "Energia: " << newPlayer.getEnergy() << std::endl << std::endl
                  << "[INIMIGO]" << std::endl
                  << "Vida: " << newEnemy.getLife() << std::endl
                  << "Energia: " << newPlayer.getEnergy() << std::endl;
        
        std::cout << "Your turn. Choose an action: attack, defend, rest, heal" << std::endl;
        std::cin >> action;

        if (action == "attack") {
            std::cout << "You attacked and caused " << damageP << " of damage" << std::endl;
            newEnemy.takeDamage(damageP);
        } else if (action == "defend") {
            newPlayer.defend();
        } // CONTINUAR
        
        std::cout << std::endl << "==================================" << std::endl << std::endl;
        // TRADUZIR PARA INGLÊS
    }

    return 0;
}