#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

// --- Terminal Color Codes for Graphics ---
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"

// ==========================================
// CLASS 1: The Machine Learning Engine
// ==========================================
class LogisticRegressionAI {
private:
    // These are the "Learned Weights". 
    // In your final 1000-line project, you will write a function to calculate these from the Kaggle CSV.
    // For now, these are pre-calculated weights so the program works immediately.
    double w_temp = 0.15;
    double w_vib = 0.45;
    double w_rpm = 0.02;
    double bias = -25.5; 

public:
    // The Mathematical Model
    double predictFailure(double temp, double vibration, double rpm) {
        // 1. Linear Function: z = w1x1 + w2x2 + w3x3 + b
        double z = (w_temp * temp) + (w_vib * vibration) + (w_rpm * rpm) + bias;
        
        // 2. Sigmoid Function: P = 1 / (1 + e^-z)
        double probability = 1.0 / (1.0 + exp(-z));
        
        return probability;
    }
};

// ==========================================
// CLASS 2: The Terminal Dashboard & Graphics
// ==========================================
class FlightDashboard {
public:
    void clearScreen() {
        // Clears the console for a smooth animation effect
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void drawHeader() {
        cout << CYAN;
        cout << "======================================================\n";
        cout << "      [X] SKY-GUARDIAN TELEMETRY AI [X]               \n";
        cout << "======================================================\n";
        cout << "       \\_______/              [PREDICTIVE MAINTENANCE]\n";
        cout << "   `---o-O---o---'                                    \n";
        cout << "       /___\\                                          \n";
        cout << "======================================================\n";
        cout << RESET;
    }

    void displayStatus(double temp, double vib, double rpm, double riskProb) {
        drawHeader();
        
        cout << "\n--- LIVE SENSOR FEED ---\n";
        cout << "Motor Temp:    " << temp << " C\n";
        // Formatting to make it look like a real dashboard
        cout << "Vibration:     " << vib << " Hz\n";
        cout << "Rotor Speed:   " << rpm << " RPM\n";
        cout << "------------------------\n\n";

        cout << "--- AI PREDICTION ---\n";
        cout << "Calculated Failure Risk: " << fixed << setprecision(2) << (riskProb * 100) << "%\n";

        // Graphics / Color logic based on AI output
        if (riskProb > 0.75) {
            cout << RED << "\n[!!!] CRITICAL WARNING [!!!]\n";
            cout << "IMMINENT HARDWARE FAILURE DETECTED.\nINITIATING EMERGENCY LANDING PROTOCOL.\n" << RESET;
        } 
        else if (riskProb > 0.40) {
            cout << YELLOW << "\n[!] CAUTION [!]\n";
            cout << "Anomalous readings detected. Advise returning to base.\n" << RESET;
        } 
        else {
            cout << GREEN << "\n[OK] SYSTEM STABLE.\n";
            cout << "All telemetry operating within normal parameters.\n" << RESET;
        }
    }
};

// ==========================================
// MAIN FUNCTION (The Loop)
// ==========================================
int main() {
    LogisticRegressionAI aiModel;
    FlightDashboard ui;
    
    double currentTemp, currentVib, currentRPM;

    while (true) {
        // 1. Read the live data from a text file
        ifstream sensorFile("sensors.txt");
        
        if (sensorFile.is_open()) {
            sensorFile >> currentTemp >> currentVib >> currentRPM;
            sensorFile.close();

            // 2. Send data to the AI Model
            double risk = aiModel.predictFailure(currentTemp, currentVib, currentRPM);

            // 3. Update the Graphics
            ui.clearScreen();
            ui.displayStatus(currentTemp, currentVib, currentRPM, risk);
        } else {
            ui.clearScreen();
            cout << RED << "ERROR: Cannot connect to sensors.txt. System offline." << RESET << endl;
        }

        // Wait 2 seconds before scanning the file again
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    return 0;
}