#include <iostream>
#include <limits>
using namespace std;


double getDistance() {
    double distance;
    cout << "Enter journey distance in miles: ";
    while (!(cin >> distance) || distance < 0) {
        cout << "Please enter a valid non-negative distance: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return distance;
}

int getFuelType() {
    int fuel;
    cout << "Choose fuel type (1 = Diesel, 2 = Petrol): ";
    while (!(cin >> fuel) || (fuel != 1 && fuel != 2)) {
        cout << "Invalid choice. Enter 1 for Diesel or 2 for Petrol: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return fuel;
}

double calculateCO2(double distance, int fuel) {
    const double litresPer100Miles = 9.66;
    const double dieselCO2PerLitre = 2.68;
    const double petrolCO2PerLitre = 2.31;
    double litresUsed = (distance / 100.0) * litresPer100Miles;
    double emissionRate = (fuel == 1) ? dieselCO2PerLitre : petrolCO2PerLitre;
    return litresUsed * emissionRate;
}

int main() {
    cout << "Car Journey CO2 Calculator\n";
    double distance = getDistance();
    int fuel = getFuelType();
    double emissions = calculateCO2(distance, fuel);

    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "Estimated CO2 emissions: " << emissions << " kg CO2\n";
    return 0;
}
