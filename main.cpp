#include "SpaceShooter.h"
#include "Uno.h"
#include <iostream>
using namespace std;

int main() {
    int choice = 3;
    bool invalidCheck = false;

    cout << "Enter 0 For Uno" << endl;
    cout << "Enter 1 For SpaceShooter" << endl;

    do {
        if (invalidCheck) {
            cout << "Wrong Input" << endl;
        }
        cout << "Enter Your Choice: ";
        cin >> choice;
        invalidCheck = true;
    } while ((choice != 0 && choice != 1));

    switch (choice) {
        case 1:
            SpaceShooter();
            break;
        case 0:
            Uno();
            break;
        default:
            return 0;
    }

    return 0;
}
