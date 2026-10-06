#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <sstream>
#include <stdexcept>

using namespace std;

// ============================================================
// 1. MISSION CLASS
// Demonstrates: class, object, encapsulation, constructor
// ============================================================

class Mission {
private:
    string missionId;
    string vehicleId;
    string status;

public:
    Mission(const string& missionId, const string& vehicleId)
        : missionId(missionId),
          vehicleId(vehicleId),
          status("READY") {}

    void startMission() {
        status = "ACTIVE";
        cout << "\nMission started successfully.\n";
    }

    void endMission() {
        status = "COMPLETED";
        cout << "\nMission ended successfully.\n";
    }

    void display() const {
        cout << "\n--- MISSION DETAILS ---\n";
        cout << "Mission ID : " << missionId << '\n';
        cout << "Vehicle ID : " << vehicleId << '\n';
        cout << "Status     : " << status << '\n';
    }

    string getMissionId() const {
        return missionId;
    }

    string getVehicleId() const {
        return vehicleId;
    }

    string getStatus() const {
        return status;
    }
};


// ============================================================
// 2. ABSTRACT SENSOR BASE CLASS
// Demonstrates: abstraction, inheritance, polymorphism
// ============================================================

class Sensor {
protected:
    string sensorId;
    string name;
    string status;

public:
    Sensor(const string& sensorId, const string& name)
        : sensorId(sensorId),
          name(name),
          status("READY") {}

    virtual ~Sensor() = default;

    // Pure virtual functions make Sensor an abstract class.
    virtual void read() = 0;
    virtual void displayStatus() const = 0;

    string getSensorId() const {
        return sensorId;
    }

    string getName() const {
        return name;
    }

    string getStatus() const {
        return status;
    }
};


// ============================================================
// 3. BMP280 SENSOR
// Inherits from Sensor
// ============================================================

class BMP280Sensor : public Sensor {
private:
    double pressure;
    double temperature;

public:
    explicit BMP280Sensor(const string& id)
        : Sensor(id, "BMP280"),
          pressure(0.0),
          temperature(0.0) {}

    void read() override {
        // Simulated sensor values.
        pressure = 92.4;
        temperature = 29.6;
        status = "OK";
    }

    void displayStatus() const override {
        cout << fixed << setprecision(2);
        cout << "[BMP280] Status: " << status
             << " | Pressure: " << pressure << " kPa"
             << " | Temperature: " << temperature << " C\n";
    }
};


// ============================================================
// 4. GPS SENSOR
// Inherits from Sensor
// ============================================================

class GPSSensor : public Sensor {
private:
    double latitude;
    double longitude;

public:
    explicit GPSSensor(const string& id)
        : Sensor(id, "GPS"),
          latitude(0.0),
          longitude(0.0) {}

    void read() override {
        // Simulated GPS coordinates.
        latitude = 19.8762;
        longitude = 75.3433;
        status = "LOCKED";
    }

    void displayStatus() const override {
        cout << fixed << setprecision(6);
        cout << "[GPS] Status: " << status
             << " | Latitude: " << latitude
             << " | Longitude: " << longitude << '\n';
    }
};


// ============================================================
// 5. IMU SENSOR
// Inherits from Sensor
// ============================================================

class IMUSensor : public Sensor {
private:
    double acceleration;
    double gyro;

public:
    explicit IMUSensor(const string& id)
        : Sensor(id, "IMU"),
          acceleration(0.0),
          gyro(0.0) {}

    void read() override {
        // Simulated IMU values.
        acceleration = 9.81;
        gyro = 0.42;
        status = "OK";
    }

    void displayStatus() const override {
        cout << fixed << setprecision(2);
        cout << "[IMU] Status: " << status
             << " | Acceleration: " << acceleration << " m/s^2"
             << " | Gyro: " << gyro << " deg/s\n";
    }
};


// ============================================================
// 6. TELEMETRY PACKET CLASS
// Demonstrates: encapsulation, validation, exception handling
// ============================================================

class TelemetryPacket {
private:
    int packetId;
    double altitude;
    double temperature;
    double pressure;
    double battery;

public:
    TelemetryPacket(int packetId,
                    double altitude,
                    double temperature,
                    double pressure,
                    double battery)
        : packetId(packetId),
          altitude(altitude),
          temperature(temperature),
          pressure(pressure),
          battery(battery) {}

    bool validate() const {
        return altitude >= 0.0 && altitude <= 20000.0 &&
               temperature >= -100.0 && temperature <= 100.0 &&
               pressure >= 0.0 && pressure <= 120.0 &&
               battery >= 0.0 && battery <= 100.0;
    }

    void display() const {
        if (!validate()) {
            throw runtime_error("Invalid telemetry packet.");
        }

        cout << fixed << setprecision(2);
        cout << "\n--- TELEMETRY PACKET ---\n";
        cout << "Packet ID    : " << packetId << '\n';
        cout << "Altitude     : " << altitude << " m\n";
        cout << "Temperature  : " << temperature << " C\n";
        cout << "Pressure     : " << pressure << " kPa\n";
        cout << "Battery      : " << battery << " %\n";
    }

    int getPacketId() const {
        return packetId;
    }

    double getAltitude() const {
        return altitude;
    }

    double getTemperature() const {
        return temperature;
    }

    double getPressure() const {
        return pressure;
    }

    double getBattery() const {
        return battery;
    }
};


// ============================================================
// 7. COMMUNICATION LOG CLASS
// ============================================================

class CommLog {
private:
    int packetId;
    string time;
    string status;

public:
    CommLog(int packetId, const string& status)
        : packetId(packetId),
          status(status) {
        log();
    }

    void log() {
        auto now = chrono::system_clock::now();
        time_t currentTime = chrono::system_clock::to_time_t(now);

        ostringstream output;
        output << put_time(localtime(&currentTime),
                           "%Y-%m-%d %H:%M:%S");

        time = output.str();
    }

    void display() const {
        cout << "Packet ID: " << packetId
             << " | Time: " << time
             << " | Status: " << status << '\n';
    }

    int getPacketId() const {
        return packetId;
    }

    string getTime() const {
        return time;
    }

    string getStatus() const {
        return status;
    }
};


// ============================================================
// 8. DATA LOGGER CLASS
// Demonstrates: file handling + FUNCTION OVERLOADING
// ============================================================

class DataLogger {
private:
    string fileName;

public:
    explicit DataLogger(const string& fileName)
        : fileName(fileName) {}

    // Overloaded function #1
    void logTelemetry(const TelemetryPacket& packet) {
        ofstream file(fileName, ios::app);

        if (!file) {
            throw runtime_error("Could not open data file.");
        }

        file << "TELEMETRY,"
             << packet.getPacketId() << ","
             << packet.getAltitude() << ","
             << packet.getTemperature() << ","
             << packet.getPressure() << ","
             << packet.getBattery() << '\n';
    }

    // Overloaded function #2
    void logTelemetry(double altitude, double temperature) {
        ofstream file(fileName, ios::app);

        if (!file) {
            throw runtime_error("Could not open data file.");
        }

        file << "TELEMETRY_SIMPLE,"
             << altitude << ","
             << temperature << '\n';
    }

    void save(const Mission& mission,
              const vector<TelemetryPacket>& packets,
              const vector<CommLog>& logs) const {

        ofstream file(fileName);

        if (!file) {
            throw runtime_error("Unable to create mission data file.");
        }

        file << "=== CANSAT MISSION DATA ===\n";
        file << "Mission ID: " << mission.getMissionId() << '\n';
        file << "Vehicle ID: " << mission.getVehicleId() << '\n';
        file << "Status: " << mission.getStatus() << "\n\n";

        file << "--- TELEMETRY ---\n";

        for (const auto& packet : packets) {
            file << packet.getPacketId() << ","
                 << packet.getAltitude() << ","
                 << packet.getTemperature() << ","
                 << packet.getPressure() << ","
                 << packet.getBattery() << '\n';
        }

        file << "\n--- COMMUNICATION LOG ---\n";

        for (const auto& log : logs) {
            file << log.getPacketId() << ","
                 << log.getTime() << ","
                 << log.getStatus() << '\n';
        }

        cout << "\nMission data saved to "
             << fileName << ".\n";
    }

    void load() const {
        ifstream file(fileName);

        if (!file) {
            throw runtime_error(
                "No saved mission file found. Save data first."
            );
        }

        cout << "\n========== SAVED DATA ==========\n";

        string line;

        while (getline(file, line)) {
            cout << line << '\n';
        }

        cout << "================================\n";
    }
};


// ============================================================
// 9. INPUT FUNCTIONS
// ============================================================

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt, int minValue, int maxValue) {
    int value;

    while (true) {
        cout << prompt;

        if (cin >> value &&
            value >= minValue &&
            value <= maxValue) {
            return value;
        }

        cout << "Invalid choice. Please try again.\n";
        clearInput();
    }
}

double readDouble(const string& prompt,
                  double minValue,
                  double maxValue) {

    double value;

    while (true) {
        cout << prompt;

        if (cin >> value &&
            value >= minValue &&
            value <= maxValue) {
            return value;
        }

        cout << "Invalid value. Enter a number between "
             << minValue << " and " << maxValue << ".\n";

        clearInput();
    }
}


// ============================================================
// 10. TELEMETRY FUNCTIONS
// ============================================================

void enterTelemetry(vector<TelemetryPacket>& packets,
                    int& nextPacketId) {

    cout << "\n--- ENTER TELEMETRY ---\n";

    double altitude =
        readDouble("Altitude (m) [0-20000]: ",
                   0, 20000);

    double temperature =
        readDouble("Temperature (C) [-100 to 100]: ",
                   -100, 100);

    double pressure =
        readDouble("Pressure (kPa) [0-120]: ",
                   0, 120);

    double battery =
        readDouble("Battery (%) [0-100]: ",
                   0, 100);

    TelemetryPacket packet(
        nextPacketId++,
        altitude,
        temperature,
        pressure,
        battery
    );

    if (!packet.validate()) {
        cout << "Telemetry failed validation.\n";
        return;
    }

    packets.push_back(packet);

    cout << "\nTelemetry packet accepted.\n";

    packet.display();
}

void showLatestTelemetry(
    const vector<TelemetryPacket>& packets) {

    if (packets.empty()) {
        cout << "\nNo telemetry packets available.\n";
        return;
    }

    packets.back().display();
}


// ============================================================
// 11. SENSOR MENU
// Demonstrates RUNTIME POLYMORPHISM
// ============================================================

void sensorMenu(vector<unique_ptr<Sensor>>& sensors) {

    while (true) {

        cout << "\n--- SENSOR STATUS ---\n";
        cout << "1. Read all sensors\n";
        cout << "2. Display sensor status\n";
        cout << "3. Back\n";

        int choice =
            readInt("Select: ", 1, 3);

        if (choice == 3) {
            return;
        }

        if (choice == 1) {

            cout << "\nReading sensors...\n\n";

            for (auto& sensor : sensors) {

                // Runtime polymorphism:
                sensor->read();
                sensor->displayStatus();

                cout << '\n';
            }

        } else {

            for (auto& sensor : sensors) {
                sensor->displayStatus();
            }
        }
    }
}


// ============================================================
// 12. COMMUNICATION MENU
// ============================================================

void communicationMenu(
    vector<CommLog>& logs,
    int& nextPacketId) {

    cout << "\n--- COMMUNICATION LOG ---\n";

    cout << "1. CONNECTED\n";
    cout << "2. TIMEOUT\n";
    cout << "3. ERROR\n";

    int choice =
        readInt("Select status: ", 1, 3);

    string status;

    if (choice == 1)
        status = "CONNECTED";
    else if (choice == 2)
        status = "TIMEOUT";
    else
        status = "ERROR";

    CommLog log(nextPacketId++, status);

    logs.push_back(log);

    cout << "\nCommunication event recorded.\n";

    log.display();
}


// ============================================================
// 13. MISSION REPORT
// ============================================================

void generateReport(
    const Mission& mission,
    const vector<TelemetryPacket>& packets,
    const vector<CommLog>& logs) {

    cout << "\n====================================\n";
    cout << "          MISSION REPORT\n";
    cout << "====================================\n";

    mission.display();

    cout << "\nTelemetry packets       : "
         << packets.size() << '\n';

    cout << "Communication events    : "
         << logs.size() << '\n';

    if (!packets.empty()) {

        double minAltitude =
            packets.front().getAltitude();

        double maxAltitude =
            packets.front().getAltitude();

        double sumAltitude = 0.0;

        for (const auto& packet : packets) {

            minAltitude =
                min(minAltitude,
                    packet.getAltitude());

            maxAltitude =
                max(maxAltitude,
                    packet.getAltitude());

            sumAltitude +=
                packet.getAltitude();
        }

        cout << fixed << setprecision(2);

        cout << "Minimum altitude        : "
             << minAltitude << " m\n";

        cout << "Maximum altitude        : "
             << maxAltitude << " m\n";

        cout << "Average altitude        : "
             << sumAltitude / packets.size()
             << " m\n";
    }

    cout << "====================================\n";
}


// ============================================================
// 14. MAIN PROGRAM
// ============================================================

int main() {

    try {

        // Create mission object.
        Mission mission(
            "MG-CANSAT-01",
            "CSAT-01"
        );

        // Create sensor objects using base-class pointers.
        vector<unique_ptr<Sensor>> sensors;

        sensors.push_back(
            make_unique<BMP280Sensor>("BMP280-01")
        );

        sensors.push_back(
            make_unique<GPSSensor>("GPS-01")
        );

        sensors.push_back(
            make_unique<IMUSensor>("IMU-01")
        );

        // Data containers.
        vector<TelemetryPacket> telemetryPackets;
        vector<CommLog> communicationLogs;

        // Data logger.
        DataLogger logger("mission_data.txt");

        int nextPacketId = 1;

        bool running = true;

        while (running) {

            cout << "\n";
            cout << "====================================\n";
            cout << "       CANSAT GROUND STATION\n";
            cout << "====================================\n";

            cout << "Mission: "
                 << mission.getMissionId();

            cout << "   Status: "
                 << mission.getStatus()
                 << "\n\n";

            cout << "1. Mission Management\n";
            cout << "2. Live Telemetry\n";
            cout << "3. Sensor Status\n";
            cout << "4. Communication Log\n";
            cout << "5. Flight Data / Reports\n";
            cout << "6. Save Data\n";
            cout << "7. Load Data\n";
            cout << "8. Exit\n";

            int choice =
                readInt("Select option: ", 1, 8);

            switch (choice) {

                // ----------------------------------------
                // Mission Management
                // ----------------------------------------

                case 1: {

                    cout << "\n--- MISSION MANAGEMENT ---\n";

                    cout << "1. Start mission\n";
                    cout << "2. End mission\n";
                    cout << "3. View mission\n";

                    int c =
                        readInt("Select: ", 1, 3);

                    if (c == 1) {
                        mission.startMission();
                    }
                    else if (c == 2) {
                        mission.endMission();
                    }
                    else {
                        mission.display();
                    }

                    break;
                }


                // ----------------------------------------
                // Telemetry
                // ----------------------------------------

                case 2: {

                    cout << "\n--- TELEMETRY ---\n";

                    cout << "1. Enter new telemetry\n";
                    cout << "2. View latest telemetry\n";

                    int c =
                        readInt("Select: ", 1, 2);

                    if (c == 1) {

                        enterTelemetry(
                            telemetryPackets,
                            nextPacketId
                        );

                    }
                    else {

                        showLatestTelemetry(
                            telemetryPackets
                        );
                    }

                    break;
                }


                // ----------------------------------------
                // Sensors
                // ----------------------------------------

                case 3:

                    sensorMenu(sensors);

                    break;


                // ----------------------------------------
                // Communication
                // ----------------------------------------

                case 4:

                    communicationMenu(
                        communicationLogs,
                        nextPacketId
                    );

                    break;


                // ----------------------------------------
                // Reports
                // ----------------------------------------

                case 5:

                    generateReport(
                        mission,
                        telemetryPackets,
                        communicationLogs
                    );

                    break;


                // ----------------------------------------
                // Save
                // ----------------------------------------

                case 6:

                    logger.save(
                        mission,
                        telemetryPackets,
                        communicationLogs
                    );

                    break;


                // ----------------------------------------
                // Load
                // ----------------------------------------

                case 7:

                    logger.load();

                    break;


                // ----------------------------------------
                // Exit
                // ----------------------------------------

                case 8:

                    running = false;

                    cout << "\nExiting CanSat Ground Station...\n";

                    break;
            }
        }
    }

    catch (const exception& error) {

        cerr << "\nUnexpected error: "
             << error.what()
             << '\n';

        return 1;
    }

    return 0;
}
