#include <iostream>
#include <random>
#include "Header.h"

using std::endl;

class Mahoraga; // the one that adapts
class Sukuna; // the one that fights
class Gojo; // the honored one
class Air; // whats that in the sky

///// GAME MASTER /////
int GameMaster::turn_amount = 0;
void GameMaster::increment_turn() {
    turn_amount++;
}

int getrandint(int, int);

class Air {
public:

    enum class Purple {
        None,
        Exists,
        Exploded
    };

    enum class Red {
        None,
        InAir
    };

    enum class Blue {
        None,
        InAir
    };

    Blue BCTstate = Blue::None;
    Red RCTstate = Red::None;
    Purple P_state = Purple::None;


    bool BlueInSky() const {
        return BCTstate == Blue::InAir;
    }
    bool RedInSky() const {
        return RCTstate == Red::InAir;
    }

    bool PurpleExists() const {
        return P_state == Purple::Exists;
    }
    bool Purple_Exploded() const {
        return P_state == Purple::Exploded;
    }


    bool FireBlue() {
        if (BCTstate != Blue::InAir) {
            BCTstate = Blue::InAir;
            return true;
        }
        return false;
    }

    bool FireRed() {
        if (RCTstate != Red::InAir) {
            RCTstate = Red::InAir;
            return true;
        }
        return false;
    }

    bool DestroyBlue() {
        if (BCTstate == Blue::InAir) {
            BCTstate = Blue::None;
            return true;
        }
        return false;
    }
    bool DestroyRed() {
        if (RCTstate == Red::InAir) {
            RCTstate = Red::None;
            return true;
        }
        return false;
    }


};





class Mahoraga {
public:
    double health = 200.0;
    const double maxhealth = health;

    
    void take_Damage(double Amount) {
        health -= Amount;
    }



    void maho_health_update() {

        if (health <= 0.0) {
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
        if (health > maxhealth) health = maxhealth;
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

    const double redinforcement[3]{
        1.0,
        0.5,
        0.0
    };


    const double blueinforcement[3]{
        1.0,
        0.5,
        0.0
    };


    InfinityAdaptation infinity_bypass = InfinityAdaptation::None;
    InfinityAdaptation prev_infinity = InfinityAdaptation::None;

    BlueAdaptation blue_nullification = BlueAdaptation::None;
    BlueAdaptation prev_blue = BlueAdaptation::None;

    RedAdaptation red_nullification = RedAdaptation::None;
    RedAdaptation prev_red = RedAdaptation::None;

    bool adapted_to_infinity() const {
        return infinity_bypass == InfinityAdaptation::FourthSpin;
    }
    bool nearly_adapted_to_infinity() const {
        return infinity_bypass == InfinityAdaptation::ThirdSpin;
    }
    bool halfway_adapted_to_infinity() const {
        return infinity_bypass == InfinityAdaptation::SecondSpin;
    }
    bool started_adapting_to_infinity() const {
        return infinity_bypass == InfinityAdaptation::FirstSpin;
    }
    bool not_adapted_to_infinity() const {
        return infinity_bypass == InfinityAdaptation::None;
    }



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

    bool is_wheel() const {
        return state == MahoragaState::WheelActive;
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
        else infinity_bypass = InfinityAdaptation::None;
    }

    void mahoraga_adapting(Gojo& g, double);
    void mahoraga_status();

};

class Sukuna {
public:
    double health = 1000.0;
    const double maxhealth = health;
    double previous_health = health;
    double cursed_energy = 15000.0;
    const double max_cursed_energy = cursed_energy;

    bool stunned = false;
    int stuntimer = 2;

    bool domain_amplification = false;

    int purple_charge = 0;

    enum class ReinforcementLevel {
        Max,
        High,
        MediumHigh,
        Medium,
        MediumLow,
        Low,
        Critical
    };

    const double ReinforcementMult[7] {
        0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1
    };

    ReinforcementLevel reinforcement = ReinforcementLevel::Critical;

    enum class WorldCuttingStatus {
        None,
        BypassInfinity,
        SendSlashes
    };

    void take_CE(double Amount) {
        cursed_energy -= Amount;
    }
    void take_Damage(double Amount) {
        health -= Amount;
    }

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

    void passive_CE_regen() {
        if (cursed_energy >= max_cursed_energy) {
            return;
        }   
        else {
            cursed_energy += 25.0;
        }
    }

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


    enum class TechniqueStatus {
        Active,
        BurntOut
    };

    enum class DomainStatus {
        Inactive,
        Declared,
        Active,
        Clashing
    };

    enum class DomainType {
        Regular,
        Shrinked
    };

    bool hit_by_infinite_void = false;

    enum class ReverseCursedTechnique {
        Inactive,
        Active,
        MaximumOutput
    };

    WorldCuttingStatus wcstatus = WorldCuttingStatus::None;
    SukunaMood mood = SukunaMood::Calm;
    DomainStatus domain_state = DomainStatus::Inactive;
    TechniqueStatus technique_state = TechniqueStatus::Active;
    ReverseCursedTechnique rct_healing = ReverseCursedTechnique::Inactive;
    SukunaFightStatus fight_status = SukunaFightStatus::FightingFar;


    bool declare_domain() { // to activate next turn
        if (domain_state != DomainStatus::Inactive) return false;

        if (technique_state != TechniqueStatus::BurntOut && !hit_by_infinite_void) {
            domain_state = DomainStatus::Declared;
            return true;
        }
        return false;
    }
    bool expand_domain() {
        if (domain_state == DomainStatus::Declared){
            domain_state = DomainStatus::Active;
            return true;
        }
        return false;
    }

    int max_shadow_turn = 3;
    double rct_burnout = 1.0;

    void world_cutting_slash(Mahoraga& m) {
        if (m.halfway_adapted_to_infinity()) {
            wcstatus = WorldCuttingStatus::BypassInfinity;
        }
        else if (m.adapted_to_infinity()) {
            wcstatus = WorldCuttingStatus::SendSlashes;
        }
    }


    void pet_mahoraga(Mahoraga& m);
    void use_domain(Gojo& g);
    void paralyzed();
    void sukuna_turn(Gojo& g, Mahoraga& m);
    void set_sukuna_mental_status();
    void sukuna_status();
    void sukuna_attacked(Mahoraga& m, Gojo& g, double);
    void sukuna_attacked_fists(Mahoraga& m, Gojo& g, double);
};

class Gojo {
public:
    double health = 1000.0;
    const double maxhealth = health;
    double cursed_energy = 8000.0;
    const double max_cursed_energy = cursed_energy;
    double composure = 100.0;
    bool using_infinity = true;
    int domain_turn_timer = 3;
    int domain_use_limit = 5;
    int brain_heal_turn = 10;
    double burnout_mult = 1.0;


    const double purple_ce_limit[5] = { 125.0, 200.0, 300.0, 500.0, 1000.0 };
    const double red_ce_limit[4] = { 15.0, 35.0, 60.0, 90.0 };
    const double blue_ce_limit[4] = { 10.0, 25.0, 50.0, 75.0 };



    enum class CurrentCursedTechnique {
        None,
        Blue,
        Red,
        Purple
    };
    enum class PurpleCharge {
        None,
        NineRopes,
        PolarisedLight,
        CrowandShomyo,
        TheGapBetween, // max charge
    };
    enum class RedCharge {
        None,
        Phase, // both red and blue are phase, impossible to combine without complicating it!!!!
        Paramita,
        PillarsOfLight // max charge
    };
    enum class BlueCharge {
        None,
        Phase,
        Twilight,
        EyesOfWisdom // max charge
    };

    const double blue_multiplier[4]{
        1.0,
        2.0,
        3.0,
        4.0
    };

    const double purple_multiplier[5] {
        1.0,  // None
        2.0,  // NineRopes
        3.5,  // PolarisedLight
        5.0,  // CrowandShomyo
        7.5   // TheGapBetween
    };
    const double red_multiplier[4]{
        1.0,
        2.0,
        3.0,
        4.0
    };
    enum class FightingStyle {
        Aggressive,
        Focused,
        Defensive,
        Disrupted
    };
    enum class TechniqueStatus {
        Active,
        BurntOut
    };
    enum class DomainStatus {
        Inactive,
        Active,
        Clashing
    };
    enum class DomainType {
        Regular,
        Inverted,
        Shrinked
    };
    enum class ReverseCursedTechnique {
        Inactive,
        Active,
        MaximumOutput
    };


    CurrentCursedTechnique current_ct = CurrentCursedTechnique::None;

    DomainStatus domain_status = DomainStatus::Inactive;

    FightingStyle fighting_style = FightingStyle::Focused;

    ReverseCursedTechnique rct_healing = ReverseCursedTechnique::Inactive;
    TechniqueStatus technique_status = TechniqueStatus::Active;

    PurpleCharge purple_status = PurpleCharge::None;
    PurpleCharge prev_p_status = PurpleCharge::None;

    BlueCharge blue_status = BlueCharge::None;
    BlueCharge prev_b_status = BlueCharge::None;

    RedCharge red_status = RedCharge::None;
    RedCharge prev_r_status = RedCharge::None;

    bool broken_guard()const {
        return fighting_style == FightingStyle::Disrupted;
    }
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

    bool stop_using_techniques() {
        if (current_ct != CurrentCursedTechnique::None) {
            current_ct = CurrentCursedTechnique::None;
            return true;
        }
        return false;
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
    bool use_ct_purple() {
        if (current_ct != CurrentCursedTechnique::Purple) {
            current_ct = CurrentCursedTechnique::Purple;
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
        return technique_status == TechniqueStatus::BurntOut;
    }
    bool domain_active() const {
        return domain_status == DomainStatus::Active;
    }
    bool domain_inactive() const {
        return domain_status == DomainStatus::Inactive;
    }

    bool rct_active()const {
        return rct_healing == ReverseCursedTechnique::Active ||
            rct_healing == ReverseCursedTechnique::MaximumOutput;
    }

    bool purple_not_charged()const { // 0;
        return purple_status == PurpleCharge::None;
    }
    bool purple_charge_started()const { // 1;
        return purple_status == PurpleCharge::NineRopes;
    }
    bool purple_charge_partial()const { // 2;
        return purple_status == PurpleCharge::PolarisedLight;
    }
    bool purple_nearly_charged()const { // 3;
        return purple_status == PurpleCharge::CrowandShomyo;
    }
    bool purple_fully_charged()const { // 4;
        return purple_status == PurpleCharge::TheGapBetween;
    }
    bool set_red_0() {
        if (red_status != RedCharge::None) {
            red_status = RedCharge::None;
            return true;
        }
        return false;
    }
    bool set_red_1() {
        if (red_status != RedCharge::Phase) {
            red_status = RedCharge::Phase;
            return true;
        }
        return false;
    }
    bool set_red_2() {
        if (red_status != RedCharge::Paramita) {
            red_status = RedCharge::Paramita;
            return true;
        }
        return false;
    }
    bool set_red_3() {
        if (red_status != RedCharge::PillarsOfLight) {
            red_status = RedCharge::PillarsOfLight;
            return true;
        }
        return false;
    }
    bool set_blue_0() {
        if (blue_status != BlueCharge::None) {
            blue_status = BlueCharge::None;
            return true;
        }
        return false;
    }
    bool set_blue_1() {
        if (blue_status != BlueCharge::Phase) {
            blue_status = BlueCharge::Phase;
            return true;
        }
        return false;
    }
    bool set_blue_2() {
        if (blue_status != BlueCharge::Twilight) {
            blue_status = BlueCharge::Twilight;
            return true;
        }
        return false;
    }
    bool set_blue_3() {
        if (blue_status != BlueCharge::EyesOfWisdom) {
            blue_status = BlueCharge::EyesOfWisdom;
            return true;
        }
        return false;
    }


    bool set_purple_0() {
        if (purple_status != PurpleCharge::None) {
            purple_status = PurpleCharge::None;
            return true;
        }
        return false;
    }
    bool set_purple_1() {
        if (purple_status != PurpleCharge::NineRopes) {
            purple_status = PurpleCharge::NineRopes;
            return true;
        }
        return false;
    }
    bool set_purple_2() {
        if (purple_status != PurpleCharge::PolarisedLight) {
            purple_status = PurpleCharge::PolarisedLight;
            return true;
        }
        return false;
    }
    bool set_purple_3() {
        if (purple_status != PurpleCharge::CrowandShomyo) {
            purple_status = PurpleCharge::CrowandShomyo;
            return true;
        }
        return false;

    }
    bool set_purple_4() {
        if (purple_status != PurpleCharge::TheGapBetween) {
            purple_status = PurpleCharge::TheGapBetween;
            return true;
        }
        return false;
    }

    bool composure_check(double com) {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        double p = com / 100.0;
        if (p < 0.0) p = 0.0;
        if (p > 1.0) p = 1.0;

        std::bernoulli_distribution dist(p);
        return dist(gen);
    }


    int P_chantometer = 2;
    int P_chantimeter = 0;
    
    int B_chantometer = 2;
    int B_chantimeter = 0;

    int R_chantometer = 2;
    int R_chantimeter = 0;

    void decrease_blue_chant() {
        if (B_chantimeter <= 0) return;
        if (prev_b_status == blue_status || B_chantimeter == 3) B_chantometer--;

        if (B_chantometer <= 0) {
            switch (B_chantimeter) {
            case 0:
                set_blue_0();
                std::cout << "-> 'Phase' has worn off on Blue. It wont have the same output unless you redo your Chant!\n";
                break;
            case 1:
                set_blue_1();
                std::cout << "*-> 'Twilight' has worn off on Blue. It wont have the same output unless you redo your Chant!\n";
                break;
            case 2:
                set_blue_2();
                std::cout << "**-> 'Eyes of Wisdom' has worn off on Blue. It wont have the same output unless you redo your Chant!\n";
                break;
            }
            B_chantometer = 2;
        }
        prev_b_status = blue_status;
    }

    void blue_chant() {
        if (B_chantimeter == 3) {
            std::cout << "***=> Your Blue is already fully chanted. You cant increase its output any further\n";
            return;
        }
        if (B_chantimeter < 3) B_chantimeter++;

        switch (B_chantimeter) {
        case 1:
            std::cout << "*=> You chant: 'Phase' to increase the output of your Blue!\n";
            set_blue_1();
            break;
        case 2:
            std::cout << "**=> You chant: 'Twiligh' to increase the output of your Blue!\n";
            set_blue_2();
            break;
        case 3:
            std::cout << "***=> You chant: 'Eyes of wisdom' your Blue reaches it's Maximum Output!\n";
            set_blue_3();
            break;
        }
    }

    void decrease_red_chants() {
        if (R_chantimeter <= 0) return;
        if (prev_r_status == red_status || R_chantimeter == 3) R_chantometer--;
        
        switch (R_chantimeter) {
        case 0:
            set_red_0();
            std::cout << "-> 'Phase' has worn off on Red. Unless you redo your chant, it wont have the same output\n";
            break;
        case 1:
            set_red_1();
            std::cout << "*-> 'Paramita' has worn off on Red. Unless you redo your chant, it wont have the same output\n";
            break;
        case 2:
            set_red_2();
            std::cout << "**-> 'Pillars of Light' has worn off on Red. Unless you redo your chant, it wont have the same output\n";
            break;
        }
        prev_r_status = red_status;
    }

    void red_chant() {
        if (R_chantimeter == 3) {
            std::cout << "***=> Your Red is already fully chanted. You cant increase its output any further\n";
            return;
        }
        if (R_chantimeter < 3) R_chantimeter++;

        switch (R_chantimeter) {
        case 1:
            std::cout << "*=> You chant: 'Phase' to increase the output of your Red!\n";
            set_red_1();
            break;
        case 2:
            std::cout << "**=> You chant: 'Paramita' to increase the output of your Red!\n";
            set_red_2();
            break;
        case 3:
            std::cout << "***=> You chant: 'Pillars of Light' and your Red reaches it's Maximum Output!\n";
            set_red_3();
            break;
        }
    }


    void decrease_purple_chants() {
        if (P_chantimeter <= 0) return;

        if (prev_p_status == purple_status || P_chantimeter == 4) P_chantometer--;
        
        if (P_chantometer <= 0) {
            switch (P_chantimeter) {
            case 1:
                set_purple_0();
                std::cout << "-> Your chants cant keep their strength anymore, you will need to chant: 'Nine Ropes' again.\n";
                std::cout << "-> Your chants have worn off, Purple will be at its weakest\n";
                break;
            case 2:
                set_purple_1();
                std::cout << "*-> Your chants cant keep their strength anymore, you will need to chant: 'Polarised Light' again.\n";
                break;
            case 3:
                set_purple_2();
                std::cout << "**-> Your chants cant keep their strength anymore, you will need to chant: 'Crow and Shomyo' again.\n";
                break;
            case 4:
                set_purple_3();
                std::cout << "***-> Your chants cant keep their strength anymore, you will need to chant: 'The gap between Within and Without' again.\n";
                break;
            }
            P_chantometer = 2;
        }
        else if (P_chantometer < 2 && P_chantometer > 0) {
            std::cout << "*-> Your chants are slowly wearing off, you better use Purple soon!\n";
        }
        prev_p_status = purple_status;
    }

    void purple_chant() {
        if (P_chantimeter == 4) {
            std::cout << "****=> Your Purple is already fully chanted. You cant increase its output any further\n";
            return;
        }
        if (P_chantimeter < 4) P_chantimeter++;
        

        switch (P_chantimeter) {
        case 1:
            set_purple_1();
            std::cout << "*=> You chant: 'Nine Ropes!' Your hollow purple increases in strength!\n";
            break;
        case 2:
            set_purple_2();
            std::cout << "**=> You chant: 'Polarised Light!' Your hollow purple increases in strength!\n";
            break;
        case 3:
            set_purple_3();
            std::cout << "***=> You chant: 'Crow and Shomyo' Your hollow purple increases in strength!\n";
            break;
        case 4:
            set_purple_4();
            std::cout << "****=> You chant: 'The gap between Within and Without!' Your hollow purple increases in strength to its maximum potential!\n";
        }
    }

    bool is_using_cursed_techniques()const { // use for sukuna
        return current_ct == CurrentCursedTechnique::Blue || 
               current_ct == CurrentCursedTechnique::Red  || 
               current_ct == CurrentCursedTechnique::Purple;
    }

    void PurpleUsed() {
        P_chantimeter = 0;
        P_chantometer = 2;
        purple_status = PurpleCharge::None;
    }
    void RedUsed() {
        R_chantimeter = 0;
        R_chantometer = 2;
        red_status = RedCharge::None;
    }
    void BlueUsed() {
        B_chantimeter = 0;
        B_chantometer = 2;
        blue_status = BlueCharge::None;
    }

    void Activate_Domain(Mahoraga& m, Sukuna& s);
    void Damage_to_composure(Gojo& g, int damage);
    void Purple(Mahoraga& m, Air& a, Sukuna& s);
    void Use_hands(Sukuna& s, Mahoraga& m);
    void Use_technique(Mahoraga& m, Sukuna& s);
    void Fire_technique(Air& a, int);



    void Composure() {
        
        if (composure <= 15.0) {
            composure -= 3.5;
        }
        else if (composure <= 40.0) {
            composure -= 2.0;
        }
        else if (composure <= 95.0){
            composure -= 1.0;
        }
        else {
            composure -= 0.5;
        }


        if (composure > 100.0) composure = 100.0;
        if (composure < 0.0) composure = 0.0;
        
        if (composure == 0.0) {
            std::cout << "-> You’re fighting on instinct alone now.\n";// player needs to get their composure up
        }
        else if (composure <= 15.0) {
            std::cout << "-> Keeping control is becoming a conscious effort.\n";
            std::cout << "-> Current composure: " << composure << '\n';
        }
        else if (composure <= 40.0) {
            int chn = getrandint(1, 2);
            if (chn == 1) {
                std::cout << "-> Your focus bleeds away with each exchange.\n";
            }
            else {
                std::cout << "-> Sustaining this pace costs more than it gives.\n";
            }
            std::cout << "-> Current composure: " << composure << '\n';
        }
        else if (composure <= 95.0){
            int ch = getrandint(1, 2);
            if (ch == 1) {
                std::cout << "-> The fight stretches your focus.\n";
            }
            else {
                std::cout << "-> The clash continues, concentration fraying.\n";
            }
            std::cout << "-> Current composure: " << composure << '\n';
        }
        else {
            std::cout << "-> You feel like you can fight non-stop for days on end.\n";
        }
    }
    void take_CE(double Amount) {
        cursed_energy -= Amount;
    }
    void take_Damage(double Amount) {
        health -= Amount;
    }

    void Choose_rct_type(int choice) {
        switch (choice){
        case 1:
            UseRCT();
            break;
        case 2:
            UseMaxOutputRCT();
            break;
        default:
            DeactivateRCT();
        }
    }
    void passive_CE_regen() {
        if (cursed_energy >= max_cursed_energy) {
            return;
        }
        else {
            cursed_energy += 25.0;
        }
    }

    void RCT_Usage() {
        if (cursed_energy < 10.0) {
            std::cout << "-> You dont have enough CT to use RCT anymore.\n";
            DeactivateRCT();
            return;
        }
        else if (cursed_energy < 50.0) {
            std::cout << "-> You dont have enough CT for Max Output anymore but you will still use RCT.\n";
            UseRCT();
        }
        


        if (is_using_rct()) {
            cursed_energy -= 10.0;
            health += 25.0;
        }
        else if (is_using_max_rct()) {
            cursed_energy -= 50.0;
            health += 150.0;
        }
        else {
            std::cout << "-> You arent using RCT currently.\n";
        }

        if (health > maxhealth && rct_active()) {
            std::cout << "-> RCT is repeatedly using Cursed Energy, consider turning it off for a while\n";
            health = maxhealth;
        }
        else if (health < 900.0 && not_using_rct()) {
            std::cout << "-> Despite the current damage you have taken, you havent been using RCT at all.\n";
        }
    }

    void Choose_technique(int choice) {
        switch (choice) {
        case 0:
            current_ct = CurrentCursedTechnique::None;
            break;
        case 1:
            current_ct = CurrentCursedTechnique::Blue;
            break;
        case 2:
            current_ct = CurrentCursedTechnique::Red;
            break;
        case 3:
            current_ct = CurrentCursedTechnique::Purple;
            break;
        default:
            current_ct = CurrentCursedTechnique::None;
        }
    }
};

int main() { //////////////////////////////////////////////////////////////////////// MAIN MAIN MAIN MAIN MAIN
    std::cout << "rest in peace my blue eyed king\n";
    std::cout << "Please refrain from entering letters it will break the game\n\n\n";
    Mahoraga m;
    Sukuna s;
    Gojo g;
    Air a;
    

    std::cout << "==> The person standing in front of you is Sukuna. Challenge him?\n->(1-yes, 2-no): ";
    int start; std::cin >> start;
    if (start == 2) std::cout << "\n-> Did you really the king of curses would ignore your strength?\n\n";

    bool fighting = true;
    while (fighting) {
        std::cout << "\n\n\n";
        std::cout << "================================================\n";
        std::cout << "Health: " << (int)g.health << " | Sukuna's Health: " << (int)s.health << '\n';
        std::cout << "CE: " << (int)g.cursed_energy << " | Sukuna's CE: " << (int)s.cursed_energy << '\n';
        std::cout << "Composure: " << (int)g.composure << '\n';
        std::cout << "RCT type: "; if (g.is_using_rct()) std::cout << "Active\n";
        else if (g.is_using_max_rct()) std::cout << "Max Output\n";
        else std::cout << "Disabled\n";
        if (!g.burnt_out()) std::cout << "Technique Status: Usable " << '\n';
        else std::cout << "Technique Status: Burnt Out " << '\n';
        if (g.using_blue()) std::cout << "Current Technique: Blue\n";
        else if (g.using_red()) std::cout << "Current Technique: Red\n";
        else if (g.using_purple()) std::cout << "Current Technique: Purple\n";
        else std::cout << "Current Technique: None\n";
        std::cout << "------------------------------------------------\n";
        

        std::cout << "== Turn amount: " << GameMaster::turn_amount << " ==";

        std::cout << "\n\n<=========== YOUR TURN ===============>\n";
        std::cout << '\n';

        if (!g.burnt_out()) {
            std::cout << "-> 1-Hand to hand, 2-Technique, 3-RCT, 4-Mahoraga status, 5-Sukuna status, 6-Chants\n";
            std::cout << "=> ";
        }
        else {
            std::cout << "-> 1-Hand to hand (CT BURNT OUT) 3-RCT, 4-Mahoraga status, 5-Sukuna status\n";
            std::cout << "=> ";
        }

        int choice = 0; std::cin >> choice;
        int ct_choice = 0;
        int f_choice = 0;

        switch (choice) {
        case 1:
            g.Use_hands(s, m);
            GameMaster::increment_turn();
            break;
        case 2:
            if (g.burnt_out()) {
                std::cerr << "-> You cant use your technique currently, you are burnt out\n";
                continue;
            }

            std::cout << "==> Choose a cursed technique:\n-> 0-None, 1-Blue, 2-Red, 3-Purple\n"; 
            std::cout << "=> ";
            std::cin >> ct_choice;
            if (ct_choice < 3 && ct_choice > 0) {
                std::cout << "==> Do you want to fire your Technique into the sky to setup an Unlimited Hollow Purple: \n";
                std::cout << "-> 1-yes, 2-no\n";
                std::cout << "=> ";
                std::cin >> f_choice;
            }
            if (f_choice == 1) {
                switch (ct_choice) {
                case 1:
                case 2:
                    g.Fire_technique(a, ct_choice);
                    GameMaster::increment_turn();
                    ct_choice = 0; // keep this here so the else statement doesnt trigger to make you attack automatically
                    break;
                default:
                    g.Fire_technique(a, 0);
                    continue;
                }
            }
            else
            {
                switch (ct_choice) {
                case 1:
                case 2:
                case 3:
                    g.Choose_technique(ct_choice);
                    GameMaster::increment_turn();
                    break;
                case 0:
                    g.Choose_technique(0);
                    std::cout << "-> You stopped using your technique!\n";
                    continue;
                default:
                    std::cout << "-> Invalid Input\n";
                    continue;
                }
                break;
            }
            break;
        case 3:
            std::cout << "==> Choose the type of RCT you'd like to use\n-> (1-regular rct, 2-max output rct, 3-disabled)\n";
            std::cout << "=> ";
            int chiss; std::cin >> chiss;
            switch (chiss) {
            case 1:
            case 2:
                g.Choose_rct_type(chiss);
                break;
            default:
                g.Choose_rct_type(0);
            }
            GameMaster::increment_turn();
            break;
        case 4:
            m.mahoraga_status();
            continue;
        case 5:
            s.sukuna_status();
            continue;
        case 6:
            std::cout << "==> Choose a technique to boost with chants\n-> 1-Blue, 2-Red, 3-Purple\n";
            std::cout << "=> ";
            int chox; std::cin >> chox;
            switch (chox) {
            case 1:
                g.blue_chant();
                GameMaster::increment_turn();
                break;
            case 2:
                g.red_chant();
                GameMaster::increment_turn();
                break;
            case 3:
                g.purple_chant();
                GameMaster::increment_turn();
                break;
            default:
                std::cout << "-> Input Unrecognized\n";
                continue;
            }
            break;
        default:
            std::cout << "-> Input Unrecognized\n";
            continue;
        }
        
        if(g.using_purple()) g.Purple(m, a ,s);
        g.Use_technique(m, s);
        g.RCT_Usage();
        g.Composure();
        g.decrease_purple_chants();
        g.decrease_red_chants();
        g.decrease_blue_chant();
        g.passive_CE_regen();

        std::cout << "\n<=========== SUKUNA'S TURN ============>\n";
        std::cout << '\n';

        s.world_cutting_slash(m);
        s.set_sukuna_mental_status();
        s.sukuna_turn(g, m);
        s.paralyzed();
        s.pet_mahoraga(m);
        s.passive_CE_regen();


        if (g.health <= 0 || s.health <= 0) break;

        std::cout << "\n-> Press Enter...";


        std::cin.ignore(); std::cin.get();

        for (int i = 0; i < 25; ++i)
            std::cout << "\n";
    }


    if (g.health <= 0) {
        std::cout << "===> You have been defeated by the king of curses, he atleast wont ever forget this fight. <===\n";
    }
    else {
        std::cout << "***==> The battle is over. You have defeated The King of Curses! <==***\n";
    }



   
    return 0;
} //////////////////////////////////////////////////////////////////////// MAIN MAIN MAIN MAIN MAIN END END END END END


void Mahoraga::mahoraga_adapting(Gojo& g, double damage) {
    if (can_adapt()) {
        if (g.using_red()) {
            red_adaptation += damage * 0.10;
        }
        else if (g.using_blue()) {
            blue_adaptation += damage * 0.10;
        }
        infinity_adaptation += damage * 0.2;
    }
}

void Gojo::Use_hands(Sukuna& s, Mahoraga& m) {
    if (s.SukunaFarAway()) {
        std::cout << "\n-> Sukuna is not in striking distance, you can use your technique to pull him in\n\n";
        return;
    }
    const double raw_strength = 2.5;
    int chance = getrandint(1, 100);

    std::cout << "\n\n";
    if ((chance == 100 || chance == 1) && composure >= 95.0) {
        s.sukuna_attacked_fists(m, *this, raw_strength * 100);
        std::cout << "***----------!!!!BLACK FLASH!!!!-----------***\n";
    }
    else if (chance >= 85 && composure >= 70.0) { ///////////////////////////////////////////////////////// UNFINISHED
        std::cout << "-> You focus and try to land a hit\n";

    }
    else if (chance >= 50) {
        std::cout << "-> more than or equal to 50\n";
    }
    else if (chance <= 50 && composure <= 40.0) {
        std::cout << "-> less comp, less or equal to 50\n";
    }
    else if (chance <= 50) {
        std::cout << "-> less or equal to 50\n";
    }


}
void Sukuna::sukuna_attacked_fists(Mahoraga& m, Gojo& g, double damage) {
    int missdifier = getrandint(1, 100);
    int rein = static_cast<int>(reinforcement);

    if (SukunaInShadows()) {
        std::cout << "-> Sukuna is in the shadows, you cant go hand to hand with him\n";
        return;
    }
    if (m.is_active()) {
        std::cout << "-> Mahoraga is active, he will take your punch.\n";

        if (damage >= 200.0) { // dont kill mahoraga in 1 hit
            std::cout << "-> Mahoraga took " << damage * 0.3 << " damage!\n";
            m.take_Damage(damage * 0.3);
        }
        else {
            std::cout << "-> Mahoraga took " << damage * 0.7 << " damage!\n";
            m.take_Damage(damage * 0.7);
        }
    }
    else if (stunned) {
        std::cout << "-> You hit Sukuna while he's unconcious!\n";
        take_Damage(damage);
    }
    else if(damage >= 200.0){ 
        std::cout << "-> You hit Sukuna so hard he got knocked out!!!\n";
        stunned = true;
        take_Damage(damage);
    }
    else {
        if (missdifier > 30 && g.composure >= 30.0) {
            std::cout << "-> You hit Sukuna with the full force of your punch!\n";
            std::cout << "-> He took " << damage * ReinforcementMult[rein] << " damage!\n";
            take_Damage(damage * ReinforcementMult[rein]);
        }
        else if (g.composure < 30.0){
            std::cout << "-> You missed your punch! you cant keep your head clear.\n";
        }
        else {
            std::cout << "-> You missed. You were off by a few inches";
        }
    }
}

void Gojo::Fire_technique(Air& air, int ct) {
    switch (ct) {
    case 1:
        if (!air.BlueInSky()) {
            std::cout << "-> You sent off a Blue into the air!\n";
            air.FireBlue();
        }
        else {
            std::cout << "-> You already have a Blue in the air, Focus on attacking sukuna instead.\n";
        }
        break;
    case 2:
        if (!air.RedInSky()) {
            std::cout << "-> You fired a Red into the air!\n";
            air.FireRed();
        }
        else {
            std::cout << "-> You already have a Red in the air, Focus on attacking sukuna instead.\n";
        }
        break;
    }
}

void Gojo::Use_technique(Mahoraga& m, Sukuna& s) { // 1-blue 2-red 3-purple
    const double blue_base = 7.5;
    const double red_base = 10.0;
    const double purple_base = 100.0;

    int chooser;

    switch (current_ct) {
    case CurrentCursedTechnique::Blue: {
        int bluecharge = static_cast<int>(blue_status);
        double Bdamage = blue_base * blue_multiplier[bluecharge];
             
        if (cursed_energy < blue_multiplier[bluecharge]) {
            std::cout << "-> You dont have enough cursed energy to activate Blue.\n";
            std::cout << "-> You need " << blue_ce_limit[bluecharge] - cursed_energy << " more Cursed Energy!\n";
            return;
        }

        if (s.SukunaInShadows()) {
            std::cout << "-> Sukuna is in the shadow's currently, you cant use your technique on him\n";
            std::cout << "-> hold out as long as you can until he gets out of the shadows.\n";
        }
        else if (s.SukunaFarAway()) {
            std::cout << "-> Sukuna is currently far away\n";
            std::cout << "==> Do you want to pull Sukuna in?\n 1-yes, 2-no: ";
            std::cout << "=> ";
            std::cin >> chooser;
            std::cout << '\n';
            if (chooser == 1) {
                if (s.SukunaOffense()) {
                    s.get_close_offensively();
                    std::cout << "-> You pulled Sukuna close, he is on offense, he will try to attack as much as possible.\n";

                }
                else if (s.SukunaDefending()) {
                    s.get_close_defensively();
                    std::cout << "-> You pulled Sukuna close, he is playing defensively, best to knock him off his feet.\n";
                }
            }
            else {
                std::cout << "-> You hit Sukuna with a " << Bdamage << " damage blue!\n";
                s.sukuna_attacked(m, *this, Bdamage);
            }
        }
        else if (s.SukunaCloseBy()) {
            std::cout << "-> Sukuna is close by\n";
            std::cout << "==> Do you want to push him away?\n 1-yes, 2-no: ";
            std::cout << "=> ";
            std::cin >> chooser;
            std::cout << '\n';
            if (chooser == 1) {
                if (s.SukunaDefending()) {
                    s.move_far_defensively();
                    std::cout << "-> You pushed sukuna away with blue, he is in a defensive state.\n";
                }
                else if (s.SukunaOffense()) {
                    s.move_far_offensively();
                    std::cout << "-> You pushed sukuna away with blue, he is focused on offense.\n";
                }
            }
            else {
                std::cout << "-> You hit Sukuna with a " << Bdamage << " damage Blue!\n";
                s.sukuna_attacked(m, *this, Bdamage);
            }
        }
        take_CE(blue_ce_limit[bluecharge]);
        break;
    }
    case CurrentCursedTechnique::Red: {
        int redcharge = static_cast<int>(red_status);
        double Rdamage = red_base * red_multiplier[redcharge];

        if (cursed_energy < red_ce_limit[redcharge]) {
            std::cout << "-> You dont have enough cursed energy left to activate Red!\n";
            std::cout << "-> You need " << red_ce_limit[redcharge] - cursed_energy << " more cursed energy";
            return;
        }

        if (s.SukunaCloseBy()) {

            std::cout << "==> Sukuna is in striking distance\n Blow him away with red?\n";
            std::cout << "-> 1-yes, 2-no\n";
            std::cout << "=> ";
            int push; std::cin >> push;
            if (push == 1) {
                if (s.SukunaOffense()) {
                    s.move_far_offensively();
                    std::cout << "-> You blast Sukuna away with red, Sukuna will be trying to get close and attack frequently.\n";
                }
                else if (s.SukunaDefending()) {
                    s.move_far_defensively();
                    std::cout << "-> You blast Sukuna away with red, he will probably use that to his advantage.\n";
                }
            }
            else {
                std::cout << "-> You hit Sukuna with a " << Rdamage << " damage Red.\n";
                s.sukuna_attacked(m, *this, Rdamage);
            }
        }
        else {
            std::cout << "-> You fire off a Red at Sukuna\n";
            std::cout << "-> You hit Sukuna with a " << Rdamage << " damage Red!\n";
            s.sukuna_attacked(m, *this, Rdamage);
        }
        take_CE(red_ce_limit[redcharge]);
        std::cout << "-> " << red_ce_limit[redcharge] << " amount of cursed energy was used.\n";
        break;
    }
    case CurrentCursedTechnique::Purple: {
        int purplecharge = static_cast<int>(purple_status);
        double Pdamage = purple_base * purple_multiplier[purplecharge];

        if (cursed_energy < purple_ce_limit[purplecharge]) {
            std::cout << "-> You dont have enough cursed energy to pull off Purple\n";
            std::cout << "-> You need " << purple_ce_limit[purplecharge] - cursed_energy << " more Cursed Energy!\n";
            return;
        }

        s.sukuna_attacked(m, *this, Pdamage);
        std::cout << "-> You hit Sukuna with a " << Pdamage << " damage Purple!\n";

        take_CE(purple_ce_limit[purplecharge]);
        std::cout << "-> Consumed CE Amount: " << purple_ce_limit[purplecharge] << '\n';

        PurpleUsed();
        break;
    }
    case CurrentCursedTechnique::None:
        std::cout << "-> You arent using any of the Limitless aside from Infinity currently\n";
        break;
    }
}

void Mahoraga::mahoraga_status() {

    std::cout << '\n';

    if (state == MahoragaState::Destroyed) {
        std::cout << "-> Mahoraga has been destroyed, It's time Sukuna goes down!\n";
        return;
    }

    if (is_active()) {
        if (health < 25.0) {
            std::cout << "-> Mahoraga is close to being defeated for good\n";
        }
        else if (health < 75.0) {
            std::cout << "-> Mahoraga is languishing, keep it up and he will be destroyed\n";
        }
        else if (health < 150.0) {
            std::cout << "-> Mahoraga is standing without a scratch, you'll need a big attack to finish him off\n";
        }
        else {
            std::cout << "-> Mahoraga in all his beauty, you're going to be burned out at the end of this\n";
        }
    }
    else if (state == MahoragaState::WheelActive) {
        std::cout << "-> Sukuna is using Mahoraga's wheel to adapt to your technique's\n";
    }
    else {
        std::cout << "-> Mahoraga isn't currently active or fighting alongside Sukuna\n";
    }

    std::cout << "\n\n";

    switch (blue_nullification) {
    case BlueAdaptation::None:
        std::cout << "-> Mahoraga hasn't adapted to blue.\n";
        break;
    case BlueAdaptation::Partial:
        std::cout << "-> Mahoraga has partially adapted to blue.\n";
        break;
    case BlueAdaptation::Full:
        std::cout << "-> Mahoraga has adapted to blue!!!\n";
        break;
    }

    std::cout << "\n\n";

    switch (red_nullification) {
    case RedAdaptation::None:
        std::cout << "-> Mahoraga hasn't adapted to red.\n";
        break;
    case RedAdaptation::Partial:
        std::cout << "-> Mahoraga has partially adapted to red.\n";
        break;
    case RedAdaptation::Full:
        std::cout << "-> Mahoraga has adapted to red!!!\n";
        break;
    }

    std::cout << "\n\n";

    switch (infinity_bypass) {
    case InfinityAdaptation::None:
        if (is_active()) {
            std::cout << "-> Mahoraga hasn't adapted to infinity.. yet.\n";
            std::cout << "-> The wheel has not spun.\n";
        }
        break;
    case InfinityAdaptation::FirstSpin:
        std::cout << "-> Mahoraga is slowly adapting to infinity,\n";
        std::cout << "-> The wheel has spun once.\n";
        break;
    case InfinityAdaptation::SecondSpin:
        std::cout << "-> Mahoraga is slightly adapted to infinity,\n";
        std::cout << "-> The wheel has spun twice.\n";
        break;
    case InfinityAdaptation::ThirdSpin:
        std::cout << "-> Mahoraga has nearly adapted to infinity,\n";
        std::cout << "-> The wheel has spun thrice.\n";
        break;
    case InfinityAdaptation::FourthSpin:
        std::cout << "-> Mahoraga is fully adapted to infinity.\n";
        std::cout << "-> May god have mercy on your soul.\n";
        break;
    }

    std::cout << "\n\n";
}

void Sukuna::sukuna_status() {
    std::cout << '\n' << '\n';
    switch (mood) {
    case SukunaMood::Amused:
        std::cout << "-> Sukuna has an amused look on his face, he's enjoying the fight a bit too much.\n";
        break;
    case SukunaMood::Smug:
        std::cout << "-> Sukuna looks smug, that move seems to have boosted his confidence.\n";
        break;
    case SukunaMood::Calm:
        std::cout << "-> Sukuna looks on with a calm demeanor, you wonder what he's cooking up for his next move.\n";
        break;
    case SukunaMood::Intrigued:
        std::cout << "-> Sukuna looks intrigued, maybe something has caught his eye.\n";
        break;
    case SukunaMood::Joyous:
        std::cout << "-> Sukuna looks on with a joyous grin, he's enjoying the challenge you're giving him.\n";
        break;
    case SukunaMood::KnockedOut:
        std::cout << "-> Sukuna is out cold, this is your moment to get your hits in.\n";
        break;
    case SukunaMood::Desperate:
        std::cout << "-> Sukuna is looking desperate, he'll try anything to take you down at all cost.\n";
        break;
    case SukunaMood::Defeated:
        std::cout << "-> Sukuna has lost the hope of beating you, he has a defeated look in his face.\n";
        break;
    case SukunaMood::Anxious:
        std::cout << "-> Sukuna feels anxiety as the stake's become high as they can ever hope to become.\n";
        break;
    case SukunaMood::Dead:
        std::cout << "-> Sukuna has stopped breathing. You have killed Sukuna.\n";
        break;
    default:
        std::cout << "-> Sukuna has a look of unreadable malice on his face.\n";
    }
    std::cout << '\n' << '\n';
    switch (fight_status) {
    case SukunaFightStatus::DefendingClose:
        std::cout << "-> Sukuna is in a defensive stance, he is in striking distance.\n";
        break;
    case SukunaFightStatus::DefendingFar:
        std::cout << "-> Sukuna maintains his distance, keeping a watchful eye on your movements.\n";
        break;
    case SukunaFightStatus::FightingClose:
        std::cout << "-> Sukuna is applying pressure from the distance.\n";
        break;
    case SukunaFightStatus::FightingFar:
        std::cout << "-> Sukuna goes on the offensive, right up in your face.\n";
        break;
    }
    std::cout << '\n' << '\n';
}

void Sukuna::sukuna_attacked(Mahoraga& m, Gojo& g, double damage) {
    if (m.is_active()) { // mahoraga active
        std::cout << "-> Mahoraga takes the brunt of the attack!\n";
        m.take_Damage(damage * 0.90);
        m.mahoraga_adapting(g, damage * 0.90);
        std::cout << "-> Damage taken by Mahoraga: " << damage * 0.90 << "\n";
        take_Damage(damage * 0.10);
        std::cout << "-> Damage taken by Sukuna: " << damage * 0.10 << "\n";
        return;
    }
    else { // mahoraga not active
        std::cout << "-> Sukuna took all the brunt of your attack!\n";
        take_Damage(damage);
        std::cout << "-> Damage taken by Sukuna: " << damage << "\n";
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
    // fallback/default
    else mood = SukunaMood::Calm;
}

void Sukuna::sukuna_turn(Gojo& g, Mahoraga& m) {
    if (health <= 0.0) return; 
    if (stunned) {
        std::cout << "-> Sukuna is knocked out, his turn is forfeit\n";
        return;
    }

    int decider = getrandint(1, 20);

    if (health <= 200.0) {

        switch (mood) {
        case SukunaMood::Desperate:
        case SukunaMood::Defeated:
            break;

        }

        return;
    }

    if (health < previous_health) {
        

        switch (mood) {
        case SukunaMood::Amused:
            break;
            
        }

    }
    else {
        

        switch (mood) {
        case SukunaMood::Amused:
            break;

        }


    }
    previous_health = health;
}

void Sukuna::pet_mahoraga(Mahoraga& m) {
    if (m.is_active()) {
        take_CE(100.0);
    }
    else if (m.is_wheel()) {
        take_CE(45.0);
    }
}

void Sukuna::paralyzed() {
    if (stunned && stuntimer > 0) {
        std::cout << "-> Sukuna has been stunned and his turn will be skipped";
        stuntimer -= 1;
    }
    if (stuntimer <= 0) {
        stunned = false;
        stuntimer = 2;
    }
}

void Gojo::Purple(Mahoraga& m, Air& a, Sukuna& s) {
    if (purple_charge_started()) {
        std::cout << "*------------Hollow Purple!-----------*\n";
        std::cout << "-> You send out a hollow purple with one chant\n";
    }
    else if(purple_charge_partial()){
        std::cout << "**------------Hollow Purple!-----------**\n";
        std::cout << "-> You send out a hollow purple with two chants\n";
    }
    else if (purple_nearly_charged()) {
        std::cout << "***------------HOLLOW PURPLE!-----------***\n";
        std::cout << "-> You send out a hollow purple with three chants\n";
    }
    else if (purple_fully_charged()) {
        std::cout << "****------------HOLLOW TECHNIQUE: PURPLE!-----------****\n";
        std::cout << "-> You send out a fully chanted hollow purple\n";
    }
    else {
        std::cout << "------------Hollow Purple!-----------\n";
        std::cout << "-> You send out an unchanted hollow purple\n";
    }
}

int getrandint(int min, int max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

