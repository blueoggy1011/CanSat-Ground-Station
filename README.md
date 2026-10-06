# CanSat Ground Station (C++17)

An object-oriented C++ ground station application designed for CanSat telemetry tracking, sensor management, mission control, and flight data reporting.

## Features

- **Mission Management**: Track mission states (`READY`, `ACTIVE`, `COMPLETED`), start/end mission routines.
- **Sensor Integration (Polymorphism)**: Abstract base `Sensor` class with derived implementations for:
  - **BMP280**: Pressure & Temperature sensor simulation
  - **GPS**: Latitude & Longitude coordinate tracking
  - **IMU**: Accelerometer & Gyroscope telemetry
- **Telemetry Processing**: Encapsulated `TelemetryPacket` validation and real-time monitoring.
- **Communication Logging**: Event timestamps and connection status tracking (`CONNECTED`, `TIMEOUT`, `ERROR`).
- **Flight Data Logger**: Export flight session metrics and load historical logs to/from `mission_data.txt`.
- **Mission Analytics**: Calculates min, max, and average altitude data.

## System Requirements

- C++17 compliant compiler (`g++`, `clang++`, or `MSVC`)

## Building and Running

### Using GCC (MinGW / w64devkit)
```bash
g++ -std=c++17 -Wall -Wextra -pedantic CanSat_Ground_Station_CA4_SINGLE.cpp -o CanSat_Ground_Station.exe
./CanSat_Ground_Station.exe
```

## Repository Structure

```
CanSat-Ground-Station/
├── CanSat_Ground_Station_CA4_SINGLE.cpp  # Main C++ source file
├── README.md                              # Project documentation
└── .gitignore                             # Git ignore rules
```
