//
// Author: Saloni Mhatre 
// NetID : smhatr4
// DIVVY Data Project 

#include <iostream>
#include <fstream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;

// Represents a single station in the bike-sharing network.
struct Station {
    string id;
    string name;
    int capacity;
    double latitude;
    double longitude;
    int recordedTrips = 0;
};

// Represents a single bike trip.
struct Trip {
    string tripID;
    string bikeID;
    string startID;
    string endID;
    int durationInSeconds;
    string startTime;
};

// Load station information from a file into a vector.
vector<Station> loadStations(const string& filename) {
    vector<Station> stations;
    ifstream file(filename);

    if (!file) {
        cout << "Error: File '" << filename << "' cannot be opened." << endl;
        return stations; // Return an empty vector to indicate failure.
    }

    Station tempStation;
    while (file >> tempStation.id >> tempStation.capacity >> tempStation.latitude >> tempStation.longitude) {
        getline(file, tempStation.name); // Assumes name is the last in the station's file line.
        stations.push_back(tempStation);
    }

    return stations;
}

// Load trip data from a file into a vector.
vector<Trip> loadTrips(const string& filename) {
    vector<Trip> trips;
    ifstream file(filename);

    if (!file) {
        cout << "Error: File '" << filename << "' cannot be opened." << endl;
        return trips; // Return an empty vector to indicate failure.
    }

    Trip tempTrip;
    while (file >> tempTrip.tripID >> tempTrip.bikeID >> tempTrip.startID >> tempTrip.endID >> tempTrip.durationInSeconds >> tempTrip.startTime) {
        trips.push_back(tempTrip);
    }

    return trips;
}

// Display basic statistics about the loaded data.
void displayStatistics(const vector<Station>& stations, const vector<Trip>& trips) {
    int totalCapacity = 0;
    for (const auto& station : stations) {
        totalCapacity += station.capacity;
    }

    cout << "Total Stations: " << stations.size() << endl;
    cout << "Total Trips: " << trips.size() << endl;
    cout << "Total Capacity: " << totalCapacity << endl;
}

// Further functions to analyze and report on the data can follow here.

int main() {
    string stationsFilename, tripsFilename;
    
    cout << "Enter filename for station data: ";
    cin >> stationsFilename;
    vector<Station> stations = loadStations(stationsFilename);
    if (stations.empty()) return -1; // Exit if loading failed.

    cout << "Enter filename for trip data: ";
    cin >> tripsFilename;
    vector<Trip> trips = loadTrips(tripsFilename);
    if (trips.empty()) return -1; // Exit if loading failed.

    // Example usage of loaded data.
    displayStatistics(stations, trips);

    // Add interactive commands or further processing as needed.

    return 0;
}
