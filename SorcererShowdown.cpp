#include <iostream>
#include <random>

using namespace std;

class Character { // class 
protected:
	double health;
	bool is_stunned = false;
public:
	Character(double hp) {
		health = hp;
	}
	void SetStunState(bool s) {
		is_stunned = s;
	}
	double GetCharacterHealth() const {
		return health;
	}
	bool IsCharacterStunned() const {
		return is_stunned;
	}
};


class Sorcerer : public Character{ // class
protected:
	double cursed_energy;

	enum class Technique {
		Usable,
		DomainBoost,
		BurntOut
	};
	enum class Domain {
		Unused,
		Active,
		Clashing
	};
	enum class ReverseCT {
		Disabled,
		Active,
		Overdrive
	};
public:
	Sorcerer(double hp, double ce) : Character(hp) {
		cursed_energy = ce;
	}
	double GetSorcererCE() const {
		return cursed_energy;
	}
};

class Gojo : public Sorcerer { // class
public:
	Gojo() : Sorcerer(400.0, 4000.0) {}
};

class Sukuna : public Sorcerer { // class
protected:
	enum class TechniqueKnowledge {
		None,
		DomainAmplification,
		DomainAmpAndMahoraga
	};
public:
	Sukuna() : Sorcerer(500.0, 12000.0) {}
};

int main() { // main
	Gojo gojo;
	Sukuna sukuna;




	return 0;
}
