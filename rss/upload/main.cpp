#include "FragTrap.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(void)
{
	srand(time(NULL));
	std::cout << "Tom & Jerry, bravefull mouse and сunning cat have " << std::endl;
	std::cout << "long lived together hating each other!" << std::endl;
	std::cout << "They can't kill each other (this program is not yet another "
		<< std::endl;
	std::cout << "stupid episode of Death Battle), but they can play Borderlands!"
		<< std::endl;
	std::cout << "Let's finish this debate, who is better, once and for all!" << std::endl
		<< std::endl;
	FragTrap tom = FragTrap("Tom");
	FragTrap jerry = FragTrap("Jerry");

	tom.meleeAttack("Jerry");
	jerry.takeDamage(30);
	jerry.meleeAttack("Tom");
	tom.takeDamage(30);
	tom.rangedAttack("Jerry");
	jerry.takeDamage(20);
	jerry.rangedAttack("Tom");
	tom.takeDamage(20);
	tom.vaulthunter_dot_exe("Jerry");
	jerry.takeDamage(40);
	jerry.vaulthunter_dot_exe("Tom");
	tom.takeDamage(40);
	tom.beRepaired(30);
	jerry.beRepaired(30);
	tom.vaulthunter_dot_exe("Jerry");
	jerry.takeDamage(40);
	tom.vaulthunter_dot_exe("Jerry");
	jerry.takeDamage(40);
	tom.beRepaired(50);
	tom.beRepaired(20);
	tom.meleeAttack("Jerry");
	jerry.takeDamage(30);
	std::cout << std::endl << "Well ... maybe Jerry is brave, but " << std::endl;
	std::cout << "Tom has been playing \"Naruto Shippuden: Ultimate Ninja Storm 3\""
		<< std::endl;
	std::cout << "for long years, which is certanly was helpfull and also turns"
		<< std::endl;
	std::cout << "Tom into Otaku. Kono Tom Da! Game Over..." << std::endl << std::endl;	
	return (0);
}
