#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using std::string;
using std::endl;

class Mahoraga;
class Sukuna;
class Gojo;


class Mahoraga {
public:
    double health = 200.0;

    void maho_health_update() {

        if (health <= 0) {
            health = 0.0;
            state = MahoragaState::Destroyed;
            return;
        }
        else {
            if (infinity_bypass != prev_infinity) {
                health += 45.0;
                prev_infinity = infinity_bypass;
            }
            if (blue_nullification != prev_blue) {
                health += 20.0;
                prev_blue = blue_nullification;
            }
            if (red_nullification != prev_red) {
                health += 35.0;
                prev_red = red_nullification;
            }
        }
        if (health >= 200.0) health = 200.0;
    }

    enum class MahoragaState {
        Inactive,
        WheelActive,
        Active,
        Destroyed
    };

    MahoragaState state = MahoragaState::Inactive;

    enum class InfinityAdaptation {
        None,
        FirstSpin,
        SecondSpin,
        ThirdSpin,
        FourthSpin // fully adapted
    };

    enum class BlueAdaptation {
        None,
        Partial,
        Full
    };
    
    enum class RedAdaptation {
        None,
        Partial,
        Full
    };

    InfinityAdaptation infinity_bypass = InfinityAdaptation::None;
    InfinityAdaptation prev_infinity = InfinityAdaptation::None;

    BlueAdaptation blue_nullification = BlueAdaptation::None;
    BlueAdaptation prev_blue = BlueAdaptation::None;

    RedAdaptation red_nullification = RedAdaptation::None;
    RedAdaptation prev_red = RedAdaptation::None;


    bool red_adapted() const {
        return red_nullification == RedAdaptation::Full;
    }

    bool red_partially_adapted() const {
        return red_nullification == RedAdaptation::Partial;
    }

    bool blue_adapted() const {
        return blue_nullification == BlueAdaptation::Full;
    }

    bool blue_partially_adapted() const {
        return blue_nullification == BlueAdaptation::Partial;
    }

    double infinity_adaptation = 0.0;
    double red_adaptation = 0.0;
    double blue_adaptation = 0.0;
    double mahoraga_health = 200.0;


    bool activate_wheel() { /// inactive to active
        if (state == MahoragaState::Inactive) {
            state = MahoragaState::WheelActive;
            return true;
        }
        return false;
    }

    bool deactivate_wheel() { /// wheel to inactive
        if (state == MahoragaState::WheelActive) {
            state = MahoragaState::Inactive;
            return true;
        }
        return false;
    }


    bool dismiss_to_wheel() { //// active to wheel
        if (state == MahoragaState::Active) {
            state = MahoragaState::WheelActive;
            return true;
        }
        return false;
    }

    bool summon() { /// wheel to active
        if (state == MahoragaState::WheelActive) {
            state = MahoragaState::Active;
            return true;
        }
        return false;
    }

    bool dismiss() { /// active to inactive
        if (state == MahoragaState::Active) {
            state = MahoragaState::Inactive;
            return true;
        }
        return false;
    }



    bool destroy() { /// active to destroyed
        if (state == MahoragaState::Active) {
            state = MahoragaState::Destroyed;
            return true;
        }
        return false;
    }




    bool can_adapt() const {
        return state == MahoragaState::WheelActive ||
            state == MahoragaState::Active;
    }

    bool is_active() const {
        return state == MahoragaState::Active;
    }

    void update_adaptations() {
        if (red_adaptation >= 100.0) red_nullification = RedAdaptation::Full;
        else if (red_adaptation >= 50.0) red_nullification = RedAdaptation::Partial;

        if (blue_adaptation >= 100.0) blue_nullification = BlueAdaptation::Full;
        else if (blue_adaptation >= 50.0) blue_nullification = BlueAdaptation::Partial;

        if (infinity_adaptation >= 100.0) infinity_bypass = InfinityAdaptation::FourthSpin;
        else if (infinity_adaptation >= 75.0) infinity_bypass = InfinityAdaptation::ThirdSpin;
        else if (infinity_adaptation >= 50.0) infinity_bypass = InfinityAdaptation::SecondSpin;
        else if (infinity_adaptation >= 25.0) infinity_bypass = InfinityAdaptation::FirstSpin;
    }

    void mahoraga_adapting(Gojo& g, double);
    void mahoraga_status();

};

class Sukuna {
public:
    double Health = 1000.0;
    double cursed_energy = 15000.0;

    bool domain_amplification = false;

    enum class SukunaMood {
        Calm,
        Intrigued,
        Smug,
        Amused,
        Joyous,
        Anxious,
        Desperate,
        KnockedOut,
        Defeated,
        Dead
    };

    double confidence = 100.0;
    double fear = 0.0;


    enum class SukunaFightStatus {
        FightingFar,
        DefendingFar,
        FightingClose,
        DefendingClose,
        InTheShadows
    };

    bool get_close_defensively() {
        if (fight_status != SukunaFightStatus::DefendingClose) {
            fight_status = SukunaFightStatus::DefendingClose;
            return true;
        }
        return false;
    }
    bool get_close_offensively() {
        if (fight_status != SukunaFightStatus::FightingClose) {
            fight_status = SukunaFightStatus::FightingClose;
            return true;
        }
        return false;
    }
    bool move_far_defensively() {
        if (fight_status != SukunaFightStatus::DefendingFar) {
            fight_status = SukunaFightStatus::DefendingFar;
            return true;
        }
        return false;
    }
    bool move_far_offensively() {
        if (fight_status != SukunaFightStatus::FightingFar) {
            fight_status = SukunaFightStatus::FightingFar;
            return true;
        }
        return false;
    }

    bool go_into_shadows() {
        if (fight_status != SukunaFightStatus::InTheShadows) {
            fight_status = SukunaFightStatus::InTheShadows;
            return true;
        }
        return false;
    }

    bool SukunaFarAway()const {
        return fight_status == SukunaFightStatus::FightingFar ||
            fight_status == SukunaFightStatus::DefendingFar;
    }
    bool SukunaCloseBy()const {
        return fight_status == SukunaFightStatus::DefendingClose ||
            fight_status == SukunaFightStatus::FightingClose;
    }
    bool SukunaDefending()const {
        return fight_status == SukunaFightStatus::DefendingFar ||
            fight_status == SukunaFightStatus::DefendingClose;
    }
    bool SukunaOffense()const {
        return fight_status == SukunaFightStatus::FightingClose ||
            fight_status == SukunaFightStatus::FightingFar;
    }
    bool SukunaInShadows()const {
        return fight_status == SukunaFightStatus::InTheShadows;
    }


    enum class DomainStatus {
        Inactive,
        Active,
        BurntOut
    };

    enum class HitByVoid {
        BrainDamaged,
        Neutral
    };


    enum class ReverseCursedTechnique {
        Inactive,
        Active,
        MaximumOutput
    };

    SukunaMood mood = SukunaMood::Calm;
    HitByVoid brain_state = HitByVoid::Neutral;
    DomainStatus domain_state = DomainStatus::Inactive;
    ReverseCursedTechnique rct_healing = ReverseCursedTechnique::Inactive;
    SukunaFightStatus fight_status = SukunaFightStatus::FightingFar;


    bool expand_domain() {
        if (domain_state != DomainStatus::BurntOut && brain_state != HitByVoid::BrainDamaged) {
            domain_state = DomainStatus::Active;
            return true;
        }
        return false;
    }



    bool in_shadow = false;

    int max_shadow_turn = 3;
    double rct_burnout = 1.0;


    void set_sukuna_mental_status();
    void sukuna_status();
    void sukuna_attacked(Mahoraga& m, Gojo& g, double);
};

class Gojo {
public:
    double Health = 1000.0;
    double cursed_energy = 8000.0;
    bool using_infinity = true;
    int domain_turn_timer = 3;
    int domain_limit = 5;
    int brain_heal_turn = 10;
    double burnout_mult = 1.0;

    enum class CurrentCursedTechnique {
        None,
        Blue,
        Red,
        Purple
    };

    enum class DomainStatus {
        Inactive,
        Active,
        BurntOut
    };

    enum class ReverseCursedTechnique {
        Inactive,
        Active,
        MaximumOutput
    };


    CurrentCursedTechnique current_ct = CurrentCursedTechnique::None;
    DomainStatus domain_status = DomainStatus::Inactive;
    ReverseCursedTechnique rct_healing = ReverseCursedTechnique::Inactive;

    bool deactivate_cursed_technique() {
        if (current_ct != CurrentCursedTechnique::None) {
            current_ct = CurrentCursedTechnique::None;
            return true;
        }
        return false;
    }

    bool use_hollow_purple() {
        if (current_ct != CurrentCursedTechnique::Purple) {
            current_ct = CurrentCursedTechnique::Purple;
            return true;
        }
        return false;
    }


    bool using_blue()const {
        return current_ct == CurrentCursedTechnique::Blue;
    }
    bool using_red()const {
        return current_ct == CurrentCursedTechnique::Red;
    }
    bool using_purple()const {
        return current_ct == CurrentCursedTechnique::Purple;
    }





    bool use_ct_red() {
        if (current_ct != CurrentCursedTechnique::Red) {
            current_ct = CurrentCursedTechnique::Red;
            return true;
        }
        return false;
    }

    bool use_ct_blue() {
        if (current_ct != CurrentCursedTechnique::Blue) {
            current_ct = CurrentCursedTechnique::Blue;
            return true;
        }
        return false;
    }

    bool UseRCT() {
        if (rct_healing != ReverseCursedTechnique::Active) {
            rct_healing = ReverseCursedTechnique::Active;
            return true;
        }
        return false;
    }
    bool UseMaxOutputRCT() {
        if (rct_healing != ReverseCursedTechnique::MaximumOutput) { // use it at all cost
            rct_healing = ReverseCursedTechnique::MaximumOutput;
            return true;
        }
        return false;
    }

    bool DeactivateRCT() {
        if (rct_healing != ReverseCursedTechnique::Inactive) {
            rct_healing = ReverseCursedTechnique::Inactive;
            return true;
        }
        return false;
    }

    bool is_using_rct()const {
        return rct_healing == ReverseCursedTechnique::Active;
    }
    bool is_using_max_rct()const {
        return rct_healing == ReverseCursedTechnique::MaximumOutput;
    }
    bool not_using_rct()const {
        return rct_healing == ReverseCursedTechnique::Inactive;
    }



    bool burnt_out() const {
        return domain_status == DomainStatus::BurntOut;
    }

    bool domain_active() const {
        return domain_status == DomainStatus::Active;
    }

    bool domain_inactive() const {
        return domain_status == DomainStatus::Inactive;
    }


    void Use_hands(Sukuna& s);
    void Use_technique(Mahoraga& m, Sukuna& s);
    void Choose_technique(int choice) {
        if (choice == 1) {
            current_ct = CurrentCursedTechnique::Blue;
        }
        else if (choice == 2) {
            current_ct = CurrentCursedTechnique::Red;
        }
        else if (choice == 3) {
            current_ct = CurrentCursedTechnique::Purple;
        }
        else {
            current_ct = CurrentCursedTechnique::None;
        }
    }
};

int main() {
    std::cout << "rest in peace my blue eyed king\n";
    std::cout << "Please refrain from entering letters it will break the game\n\n\n";

    Sukuna s;
    Gojo g;
    Mahoraga m;
    srand(time(0));
    int turn_counter = 0;

    std::cout << "The person standing in front of you is Sukuna. Challenge him?\n (1-yes, 2-no): ";
    int start; std::cin >> start;
    if (start == 2) std::cout << "\nDid you really the king of curses would ignore your strength?\n\n";

    bool fighting = true;
    while (fighting) {
        std::cout << "\n\n\n";
        std::cout << "================================================\n";
        std::cout << "Health: " << (int)g.Health << " | Sukuna's Health: " << (int)s.Health << "\n";
        std::cout << "CE: " << (int)g.cursed_energy << " | Sukuna's CE: " << (int)s.cursed_energy << "\n";
        std::cout << "------------------------------------------------\n\n";
        
        if (!g.burnt_out()) {
            std::cout << "1-Hand to hand, 2-Technique, 3-RCT, 4-Mahoraga status, 5-Sukuna status\n";
            std::cout << "=> ";
        }
        else {
            std::cout << "1-Hand to hand (CT BURNT OUT) 3-RCT, 4-Mahoraga status, 5-Sukuna status\n";
            std::cout << "=> ";
        }

        int choice; std::cin >> choice;
        int ct_choice;

        switch (choice) {
        case 1:

            turn_counter += 1;
            break;
        case 2:
            if (g.burnt_out()) {
                std::cerr << "You cant use your technique currently, you are burnt out\n";
                continue;
            }

            std::cout << "Choose a cursed technique:\n1-Blue, 2-Red, 3-Purple\n"; 
            std::cout << "=> ";
            std::cin >> ct_choice;

            switch(ct_choice) {
            case 1:
            case 2:
            case 3:
                turn_counter += 1;
                g.Choose_technique(ct_choice);
                break;
            default:
                g.Choose_technique(0);
            }
            break;
        case 3:

            turn_counter += 1;
            break;
        case 4:
            m.mahoraga_status();
            continue;
        case 5:
            s.sukuna_status();
            continue;
        default:
            std::cout << "Input Unrecognized\n";
            continue;
        }


        

        std::cout << "\n<=========== YOUR TURN ===============>\n";

        g.Use_technique(m, s);

        std::cout << "\n<=========== SUKUNA'S TURN ============>\n";

        s.set_sukuna_mental_status();



        if (g.Health <= 0 || s.Health <= 0) fighting = false;

        std::cout << "\nPress Enter...";


        std::cin.ignore(); std::cin.get();
        for (int i = 0; i < 25; ++i)
            std::cout << "\n";
    }
    std::cout << "The battle is over.\n";
    return 0;
}


void Mahoraga::mahoraga_adapting(Gojo& g, double damage) {
    if (can_adapt()) {
        if (g.using_red()) {
            red_adaptation += damage * 0.10;
        }
        else if (g.using_blue()) {
            blue_adaptation += damage * 0.10;
        }

        if (g.using_infinity) {
            infinity_adaptation += 1.5;
        }
    }
}

void Gojo::Use_technique(Mahoraga& m, Sukuna& s) { // 1-blue 2-red 3-purple
    const double blue_base = 7.5;
    const double red_base = 10.0;
    const double purple_base = 100.0;


    switch (this->current_ct) {
    case CurrentCursedTechnique::Blue:
        if (s.SukunaFarAway()) {
            std::cout << "Sukuna is currently far away\n";
            std::cout << "Do you want to pull Sukuna in?\n 1-yes, 2-no: ";
            int pull; std::cin >> pull;
            std::cout << '\n';
            if (pull == 1) {
                if (s.SukunaOffense()) {
                    s.get_close_offensively();
                    std::cout << "You pulled Sukuna close, he is quite aggressive, this might disadvantage you.\n";
                }
                else if (s.SukunaDefending()) {
                    s.get_close_defensively();
                    std::cout << "You pulled Sukuna close, he is playing defensively, this might disadvantage him.\n";
                }
            }
            else {
                int numb = rand() % 10 + 1;
                std::cout << "You hit Sukuna with a " << blue_base * numb << " Damage blue\n";
                s.sukuna_attacked(m, *this, blue_base * numb);
            }
        }
        else {
            std::cout << "Sukuna is currently nearby you\n";
            std::cout << "Do you want to push Sukuna away?\n 1-yes, 2-no: ";
            int push; std::cin >> push;
            std::cout << '\n';
            if (push == 1) {
                if (s.SukunaOffense()) {
                    s.move_far_offensively();
                    std::cout << "You pushed Sukuna away, he is on offense, this might disadvantage him.\n";
                }
                else if (s.SukunaDefending()) {
                    s.move_far_defensively();
                    std::cout << "You pushed Sukuna away, he is playing defensively, this might disadvantage you.\n";
                }
            }
            else {
                int numby = rand() % 10 + 11;
                std::cout << "You hit Sukuna with a " << blue_base * numby << " Damage blue\n";
                s.sukuna_attacked(m, *this, blue_base * numby);
            }
        }
        break;
    case CurrentCursedTechnique::Red:
        break;
    case CurrentCursedTechnique::Purple: // unfinished , finish these
        break;
    default:
        std::cout << "you arent using a technique currently\n";
    }
}

void Mahoraga::mahoraga_status() {

    std::cout << '\n';

    if (state == MahoragaState::Destroyed) {
        std::cout << "Mahoraga has been destroyed, It's time Sukuna goes down!\n";
        return;
    }

    if (is_active()) {
        if (health < 25.0) {
            std::cout << "Mahoraga is close to being defeated for good\n";
        }
        else if (health < 75.0) {
            std::cout << "Mahoraga is languishing, keep it up and he will be destroyed\n";
        }
        else if (health < 150.0) {
            std::cout << "Mahoraga is standing steady, you'll need a big attack to finish him off\n";
        }
        else {
            std::cout << "Mahoraga in all his beauty, you're going to be burned out at the end of this\n";
        }
    }
    else if (state == MahoragaState::WheelActive) {
        std::cout << "Sukuna is using Mahoraga's wheel to adapt to your technique's\n";
    }
    else {
        std::cout << "Mahoraga isn't currently active or fighting alongside Sukuna\n";
    }

    std::cout << "\n\n";

    switch (blue_nullification) {
    case BlueAdaptation::None:
        std::cout << "Mahoraga hasn't adapted to blue.\n";
        break;
    case BlueAdaptation::Partial:
        std::cout << "Mahoraga has partially adapted to blue.\n";
        break;
    case BlueAdaptation::Full:
        std::cout << "Mahoraga has adapted to blue!!!\n";
        break;
    }

    std::cout << "\n\n";

    switch (red_nullification) {
    case RedAdaptation::None:
        std::cout << "Mahoraga hasn't adapted to red.\n";
        break;
    case RedAdaptation::Partial:
        std::cout << "Mahoraga has partially adapted to red.\n";
        break;
    case RedAdaptation::Full:
        std::cout << "Mahoraga has adapted to red!!!\n";
        break;
    }

    std::cout << "\n\n";

    switch (infinity_bypass) {
    case InfinityAdaptation::None:
        if (is_active()) {
            std::cout << "Mahoraga hasn't adapted to infinity.. yet.\n";
            std::cout << "The wheel has not spun.\n";
        }
        break;
    case InfinityAdaptation::FirstSpin:
        std::cout << "Mahoraga is slowly adapting to infinity,\n";
        std::cout << "The wheel has spun once.\n";
        break;
    case InfinityAdaptation::SecondSpin:
        std::cout << "Mahoraga is slightly adapted to infinity,\n";
        std::cout << "The wheel has spun twice.\n";
        break;
    case InfinityAdaptation::ThirdSpin:
        std::cout << "Mahoraga has nearly adapted to infinity,\n";
        std::cout << "The wheel has spun thrice.\n";
        break;
    case InfinityAdaptation::FourthSpin:
        std::cout << "Mahoraga is fully adapted to infinity.\n";
        std::cout << "May god have mercy on your soul.\n";
        break;
    }

    std::cout << "\n\n";
}

void Sukuna::sukuna_status() {
    std::cout << '\n' << '\n';
    switch (mood) {
    case SukunaMood::Amused:
        std::cout << "Sukuna looks amused, almost as if he didn't think you'd get this far.\n";
        break;
    case SukunaMood::Smug:
        std::cout << "Sukuna looks smug, that move seems to have boosted his confidence.\n";
        break;
    case SukunaMood::Calm:
        std::cout << "Sukuna looks on with a calm demeanor, you wonder what he's cooking up for his next move.\n";
        break;
    case SukunaMood::Intrigued:
        std::cout << "Sukuna looks intrigued, maybe something has caught his eye.\n";
        break;
    case SukunaMood::Joyous:
        std::cout << "Sukuna looks on with a joyous grin, he's enjoying the challenge you're giving him.\n";
        break;
    case SukunaMood::KnockedOut:
        std::cout << "Sukuna is out cold, this is your moment to get your hits in.\n";
        break;
    case SukunaMood::Desperate:
        std::cout << "Sukuna is looking desperate, he'll try anything to take you down at all cost.\n";
        break;
    case SukunaMood::Defeated:
        std::cout << "Sukuna has lost the hope of beating you, he has a defeated look in his face.\n";
        break;
    case SukunaMood::Anxious:
        std::cout << "Sukuna feels anxiety as the stake's become high as they can ever hope to become.\n";
        break;
    case SukunaMood::Dead:
        std::cout << "Sukuna has stopped breathing. You have killed Sukuna.\n";
        break;
    default:
        std::cout << "Sukuna has a look of unreadable malice on his face.\n";
    }
    std::cout << '\n' << '\n';
    switch (fight_status) {
    case SukunaFightStatus::DefendingClose:
        std::cout << "Sukuna is in a defensive stance, he is close to you.\n";
        break;
    case SukunaFightStatus::DefendingFar:
        std::cout << "Sukuna is far away, he is in a fully defensive state.\n";
        break;
    case SukunaFightStatus::FightingClose:
        std::cout << "Sukuna is applying pressure from the distance.\n";
        break;
    case SukunaFightStatus::FightingFar:
        std::cout << "Sukuna goes on the offensive, right up in your face.\n";
        break;
    }
    std::cout << '\n' << '\n';
}

void Sukuna::sukuna_attacked(Mahoraga& m, Gojo& g, double damage) {
    if (m.is_active()) {
        std::cout << "Mahoraga takes the brunt of the attack!\n";
        m.health -= damage * 0.90; std::cout << "Damage taken by Mahoraga: " << damage * 0.90 << "\n";
        Health -= damage * 0.10; std::cout << "Damage taken by Sukuna: " << damage * 0.10 << "\n";
        return;
    }
    else {
        if (domain_amplification) {
            std::cout << "Sukuna brushed off your attack with domain amplification!\n";
            Health -= damage * 0.60; std::cout << "Damage taken by Sukuna: " << damage * 0.60 << "\n";
        }
        else {
            std::cout << "Sukuna took all the brunt of your attack!\n";
            Health -= damage; std::cout << "Damage taken by Sukuna: " << damage << "\n";
        }
    }
}

void Sukuna::set_sukuna_mental_status() {
    if (confidence > 100.0) confidence = 100.0;
    if (confidence < 0.0) confidence = 0.0;

    if (fear > 100.0) fear = 100.0;
    if (fear < 0.0) fear = 0.0;

    // fear states
    if (fear >= 85.0 && confidence < 15.0) mood = SukunaMood::Defeated;
    else if (fear >= 70.0 && confidence < 30.0) mood = SukunaMood::Desperate;
    else if (fear >= 50.0) mood = SukunaMood::Anxious;
    // confidence states
    else if (confidence >= 95.0 && fear < 20.0) mood = SukunaMood::Amused;
    else if (confidence >= 80.0 && fear < 35.0) mood = SukunaMood::Smug;
    else mood = SukunaMood::Calm;
}
