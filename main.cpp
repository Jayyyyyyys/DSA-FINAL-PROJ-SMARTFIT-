#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;

const map<string, double> ACTIVITY_MULTIPLIERS = {
    {"Sedentary", 1.2}, {"Light", 1.375}, {"Moderate", 1.55},
    {"Active", 1.725}, {"Very Active", 1.9}
};

const map<string, int> GOAL_ADJUSTMENTS = {
    {"Lose Weight", -500}, {"Lose Fat", -400},
    {"Gain Muscle", 300}, {"Gain Weight", 500}
};

const map<string, map<string, double>> MACRO_RATIOS = {
    {"Lose Weight", {{"protein", 0.30}, {"carbs", 0.35}, {"fats", 0.35}}},
    {"Lose Fat",    {{"protein", 0.40}, {"carbs", 0.30}, {"fats", 0.30}}},
    {"Gain Muscle", {{"protein", 0.35}, {"carbs", 0.45}, {"fats", 0.20}}},
    {"Gain Weight", {{"protein", 0.25}, {"carbs", 0.50}, {"fats", 0.25}}}
};

// ─── Validation ───────────────────────────────────────────────────
class InputValidator {
public:
    static bool validateHeight(double height) {
        return height > 0 && height < 300;
    }
    static bool validateWeight(double weight) {
        return weight > 0 && weight < 500;
    }
    static bool validateAge(int age) {
        return age > 0 && age < 150;
    }
    static bool validateGender(const string& gender) {
        string g = gender;
        transform(g.begin(), g.end(), g.begin(), ::tolower);
        return g == "male" || g == "female" || g == "m" || g == "f";
    }
    static bool validateActivityChoice(int choice) {
        return choice >= 1 && choice <= 5;
    }
    static bool validateGoalChoice(int choice) {
        return choice >= 1 && choice <= 4;
    }
};

// ─── Calculations ─────────────────────────────────────────────────
double calculate_bmi(double weight, double height_cm) {
    double height_m = height_cm / 100.0;
    return weight / (height_m * height_m);
}

string get_bmi_category(double bmi) {
    if (bmi < 18.5) return "Underweight";
    if (bmi < 25)   return "Normal";
    if (bmi < 30)   return "Overweight";
    return "Obese";
}

double calculate_bmr(double weight, double height, int age, const string& gender) {
    string g = gender;
    transform(g.begin(), g.end(), g.begin(), ::tolower);
    if (g == "male" || g == "m")
        return (10.0 * weight) + (6.25 * height) - (5.0 * age) + 5.0;
    else
        return (10.0 * weight) + (6.25 * height) - (5.0 * age) - 161.0;
}

double calculate_tdee(double bmr, const string& activity) {
    auto it = ACTIVITY_MULTIPLIERS.find(activity);
    return (it != ACTIVITY_MULTIPLIERS.end()) ? bmr * it->second : bmr * 1.2;
}

double calculate_target_calories(double tdee, const string& goal) {
    auto it = GOAL_ADJUSTMENTS.find(goal);
    return (it != GOAL_ADJUSTMENTS.end()) ? tdee + it->second : tdee;
}

void calculate_macros(double calories, const string& goal,
                      double& protein, double& carbs, double& fats) {
    auto ratios = MACRO_RATIOS.at("Lose Weight");
    auto it = MACRO_RATIOS.find(goal);
    if (it != MACRO_RATIOS.end()) ratios = it->second;

    protein = (calories * ratios.at("protein")) / 4.0;
    carbs   = (calories * ratios.at("carbs"))   / 4.0;
    fats    = (calories * ratios.at("fats"))    / 9.0;
}

void print_exercises(const string& goal) {
    cout << "\nRecommended Exercises:\n";

    if (goal == "Lose Weight") {
        cout << "Focus: High-calorie burn\n";
        cout << "  - 30-45 min brisk walking or jogging\n";
        cout << "  - Cycling at moderate intensity (30 min)\n";
        cout << "  - Jump rope intervals (3 sets x 2 min)\n";
        cout << "  - HIIT workouts (20-30 min)\n";
    } else if (goal == "Lose Fat") {
        cout << "Focus: Cardio + resistance\n";
        cout << "  - Strength training 3x per week\n";
        cout << "  - Barbell squats, deadlifts, bench press\n";
        cout << "  - 20-30 min cardio after weights\n";
        cout << "  - Circuit training with minimal rest\n";
    } else if (goal == "Gain Muscle") {
        cout << "Focus: Progressive overload\n";
        cout << "  - Heavy compound lifts: Squats, Deadlifts, Bench\n";
        cout << "  - 4-5 sets of 6-12 reps per exercise\n";
        cout << "  - Progressive overload: increase weight weekly\n";
        cout << "  - Rest 48 hours between muscle groups\n";
    } else if (goal == "Gain Weight") {
        cout << "Focus: Strength with minimal cardio\n";
        cout << "  - Compound movements with heavy weights\n";
        cout << "  - Squats, Deadlifts, Overhead Press\n";
        cout << "  - Limited cardio: 10-15 min warm-up only\n";
        cout << "  - 3-4 sets of 8-12 reps with adequate rest\n";
    }
}

// ─── Main Menu ────────────────────────────────────────────────────
void showMainMenu() {
    cout << "\n========================================\n";
    cout << "        SMARTFIT - Fitness Calculator   \n";
    cout << "========================================\n";
    cout << "\n1. Calculate Fitness Profile\n";
    cout << "2. Exit\n";
    cout << "\nChoice: ";
}

// ─── Calculator ───────────────────────────────────────────────────
void runCalculator() {
    double height, weight;
    int age;
    string gender, activity, goal;

    cout << "\n========================================\n";
    cout << "Enter Your Information:\n";
    cout << "========================================\n";

    // Height
    do {
        cout << "Height (cm): ";
        cin >> height;
        if (!InputValidator::validateHeight(height))
            cout << "Invalid. Enter a value between 1-300 cm.\n";
    } while (!InputValidator::validateHeight(height));

    // Weight
    do {
        cout << "Weight (kg): ";
        cin >> weight;
        if (!InputValidator::validateWeight(weight))
            cout << "Invalid. Enter a value between 1-500 kg.\n";
    } while (!InputValidator::validateWeight(weight));

    // Age
    do {
        cout << "Age: ";
        cin >> age;
        if (!InputValidator::validateAge(age))
            cout << "Invalid. Enter a value between 1-150.\n";
    } while (!InputValidator::validateAge(age));

    cin.ignore();

    // Gender
    do {
        cout << "Gender (Male/Female): ";
        getline(cin, gender);
        if (!InputValidator::validateGender(gender))
            cout << "Invalid. Enter Male or Female.\n";
    } while (!InputValidator::validateGender(gender));

    // Activity Level
    int act_choice;
    do {
        cout << "\nActivity Level:\n";
        cout << "  1. Sedentary\n  2. Light\n  3. Moderate\n  4. Active\n  5. Very Active\n";
        cout << "Select (1-5): ";
        cin >> act_choice;
        if (!InputValidator::validateActivityChoice(act_choice))
            cout << "Invalid choice.\n";
    } while (!InputValidator::validateActivityChoice(act_choice));

    switch (act_choice) {
        case 1: activity = "Sedentary";   break;
        case 2: activity = "Light";       break;
        case 3: activity = "Moderate";    break;
        case 4: activity = "Active";      break;
        case 5: activity = "Very Active"; break;
    }

    cin.ignore();

    // Goal
    int goal_choice;
    do {
        cout << "\nFitness Goal:\n";
        cout << "  1. Lose Weight\n  2. Lose Fat\n  3. Gain Muscle\n  4. Gain Weight\n";
        cout << "Select (1-4): ";
        cin >> goal_choice;
        if (!InputValidator::validateGoalChoice(goal_choice))
            cout << "Invalid choice.\n";
    } while (!InputValidator::validateGoalChoice(goal_choice));

    switch (goal_choice) {
        case 1: goal = "Lose Weight"; break;
        case 2: goal = "Lose Fat";    break;
        case 3: goal = "Gain Muscle"; break;
        case 4: goal = "Gain Weight"; break;
    }

    cin.ignore();

    // ─── Perform Calculations ─────────────────────────────────────
    double bmi        = calculate_bmi(weight, height);
    string bmi_cat    = get_bmi_category(bmi);
    double bmr        = calculate_bmr(weight, height, age, gender);
    double tdee       = calculate_tdee(bmr, activity);
    double target_cal = calculate_target_calories(tdee, goal);
    double protein, carbs, fats;
    calculate_macros(target_cal, goal, protein, carbs, fats);

    // ─── Display Results ──────────────────────────────────────────
    cout << fixed << setprecision(2);
    cout << "\n========================================\n";
    cout << "             YOUR RESULTS               \n";
    cout << "========================================\n";

    cout << "\nHealth Metrics:\n";
    cout << "  BMI:  " << bmi << " (" << bmi_cat << ")\n";

    cout << "\nMetabolism:\n";
    cout << "  BMR:  " << bmr  << " kcal/day\n";
    cout << "  TDEE: " << tdee << " kcal/day\n";

    cout << "\nDaily Nutrition Targets:\n";
    cout << "  Calories: " << target_cal << " kcal\n";
    cout << "  Protein:  " << protein    << "g\n";
    cout << "  Carbs:    " << carbs      << "g\n";
    cout << "  Fats:     " << fats       << "g\n";

    print_exercises(goal);

    cout << "\n========================================\n";
}

// ─── Entry Point ──────────────────────────────────────────────────
int main() {
    int choice;
    bool running = true;

    while (running) {
        showMainMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                runCalculator();
                break;
            case 2:
                running = false;
                break;
            default:
                cout << "\nInvalid choice. Try again.\n";
        }

        if (running) {
            cout << "\nPress Enter to continue...";
            cin.get();
        }
    }

    cout << "\nGoodbye!\n";
    return 0;
}