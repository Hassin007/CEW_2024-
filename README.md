# Weather Application

## Overview
This Weather Application is a command-line program written in C that fetches real-time weather data for a user-specified city using the OpenWeather API. The application stores weather data in both raw (JSON) and processed (formatted text) formats, provides alerts for extreme temperatures, and calculates average temperatures at defined intervals. It also supports automation via a shell script and cron jobs.

---

## Features
- **Real-Time Weather Data:**
  - Fetches weather data for a specified city using the OpenWeather API.
  
- **Data Storage:**
  - Raw data stored in JSON format (`raw_data.json`).
  - Processed data saved in a human-readable text file (`processed_data_history.txt`).

- **Historical Data Management:**
  - Maintains cumulative records of raw data (`raw_data_history.json`) and processed data.

- **Temperature Alerts:**
  - Alerts for high temperature (>30°C) or low temperature (<10°C) using Zenity pop-ups.

- **Average Temperature Calculation:**
  - Computes and displays the average temperature after every 24 or a multiple of 24 readings.
  - Saves averages to `average_temperatures.txt`.

- **Dynamic City Input:**
  - Accepts city name dynamically via a shell script.

- **Automation:**
  - Shell script (`automation.sh`) for periodic execution.
  - Cron job compatibility for scheduled runs.

---

## File Structure

/weather-app
├── main.c                # Main program logic
├── functions.c           # Functions for weather data handling
├── functions.h           # Function prototypes and data structure
├── Makefile              # Makefile for compilation
├── automation.sh         # Shell script for periodic execution
├── raw_data.json         # Raw JSON response from the API
├── raw_data_history.json # History of raw data responses
├── processed_data_history.txt  # History of processed data
├── average_temperatures.txt     # Average temperature log
