// Name: Darren Benavides
// Date: 10/08/2024
// Filename: dbenavides.cpp
// Purpose: Project Feature
#include "dbenavides.cpp"

using namespace std;

// Obtained Structure from: 
// https://www.geeksforgeeks.org/print-system-time-c-3-different-ways/
void background_time() {
    time_t my_time = time(0);
    tm * ltime = localtime(&my_time);
    
    // Extract Hours
    int hours = ltime->tm_hour;

    if (5 <= hours < 6) {
        // Dawn
    } else if (6 <= hours < 12) {
        // Morning
    } else if (12 <= hours < 17) {
        // Afternoon
    } else if (17 <= hours < 20) {
        // Evening
    } else if (20 <= hours || hours < 5) {
        // Night
    }
};

