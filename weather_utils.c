#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "weather_utils.h"

void format_city_name(char *city) {
    if (city[0] >= 'a' && city[0] <= 'z') {
        city[0] = city[0] - 'a' + 'A'; // Convert first letter to uppercase
    }
    for (int i = 1; city[i] != '\0'; i++) {
        if (city[i] >= 'A' && city[i] <= 'Z') {
            city[i] = city[i] - 'A' + 'a'; // Convert the rest to lowercase
        }
    }
}

// Function to read the execution count from the file
int read_execution_count(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // If the file does not exist, initialize with 0
        return 0;
    }
    int count;
    fscanf(file, "%d", &count);
    fclose(file);
    return count;
}

// Function to save the updated execution count to the file
void save_execution_count(const char *filename, int count) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }
    fprintf(file, "%d", count);
    fclose(file);
}

// Function to save the temperature reading to the file
void save_temperature_reading(const char *filename, const WeatherData *weather) {
    FILE *file = fopen(filename, "a"); // Append mode
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }
    fprintf(file, "City: %s\n", weather->city);
    fprintf(file, "Temperature: %.2f°C\n", weather->temperature);
    fprintf(file, "Min Temp: %.2f°C\n", weather->temp_min);
    fprintf(file, "Max Temp: %.2f°C\n", weather->temp_max);
    fprintf(file, "Humidity: %.2f%%\n", weather->humidity);
    fprintf(file, "Pressure: %.2f hPa\n", weather->pressure);
    fprintf(file, "Wind Speed: %.2f m/s\n", weather->wind_speed);
    fprintf(file, "Description: %s\n", weather->description);
    fprintf(file, "---------------------------------\n");
    fclose(file);
}

// Function to read the last 5 weather readings from the file
int read_last_five_readings(const char *filename, WeatherData *readings) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s for reading\n", filename);
        return 0;
    }

    char line[512];
    int count = 0;
    WeatherData temp;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "---------------------------------", 33) == 0) {
            if (count > 0) {
                readings[(count - 1) % MAX_READINGS] = temp;
            }
            count++;
        } else if (strstr(line, "City:") != NULL) {
            sscanf(line, "City: %49s", temp.city);  // Added size limit
        } else if (strstr(line, "Temperature:") != NULL) {
            sscanf(line, "Temperature: %f", &temp.temperature);
        } else if (strstr(line, "Min Temperature:") != NULL) {
            sscanf(line, "Min Temperature: %f", &temp.temp_min);
        } else if (strstr(line, "Max Temperature:") != NULL) {
            sscanf(line, "Max Temperature: %f", &temp.temp_max);
        } else if (strstr(line, "Humidity:") != NULL) {
            sscanf(line, "Humidity: %f", &temp.humidity);
        } else if (strstr(line, "Pressure:") != NULL) {
            sscanf(line, "Pressure: %f", &temp.pressure);
        } else if (strstr(line, "Wind Speed:") != NULL) {
            sscanf(line, "Wind Speed: %f", &temp.wind_speed);
        } else if (strstr(line, "Description:") != NULL) {
            sscanf(line, "Description: %[^\n]", temp.description);
        }
    }

    fclose(file);
    return count < MAX_READINGS ? count : MAX_READINGS;  // Ensure it doesn't return more than MAX_READINGS
}

// Function to display the last 5 readings
void display_last_five_readings(WeatherData *readings, int count) {
    printf("\nLast %d Weather Readings:\n", count);
    for (int i = 0; i < count; i++) {
        printf("Reading %d:\n", i + 1);
        printf("City: %s\n", readings[i].city);
        printf("Temperature: %.2f°C\n", readings[i].temperature);
        printf("Min Temp: %.2f°C\n", readings[i].temp_min);
        printf("Max Temp: %.2f°C\n", readings[i].temp_max);
        printf("Humidity: %.2f%%\n", readings[i].humidity);
        printf("Pressure: %.2f hPa\n", readings[i].pressure);
        printf("Wind Speed: %.2f m/s\n", readings[i].wind_speed);
        printf("Description: %s\n", readings[i].description);
        printf("---------------------------------\n");
    }
}

// Function to calculate average temperature
float calculate_average_temperature(WeatherData *readings, int count) {
    float sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += readings[i].temperature;
    }
    return count > 0 ? sum / count : 0.0;
}

void save_raw_data(const char *filename, const char *data) {
    FILE *file = fopen(filename, "a"); // Use "a" for append mode
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }
    fprintf(file, "%s\n", data); // Add a newline for separation
    fclose(file);
}

void save_processed_data(const char *filename, const WeatherData *weather) {
    FILE *file = fopen(filename, "a"); // Use "a" for append mode
    if (file == NULL) {
        fprintf(stderr, "Failed to open file %s for writing\n", filename);
        return;
    }
    fprintf(file, "City: %s\n", weather->city);
    fprintf(file, "Temperature: %.2f °C\n", weather->temperature);
    fprintf(file, "Min Temperature: %.2f °C\n", weather->temp_min);
    fprintf(file, "Max Temperature: %.2f °C\n", weather->temp_max);
    fprintf(file, "Humidity: %.2f %%\n", weather->humidity);
    fprintf(file, "Pressure: %.2f hPa\n", weather->pressure);
    fprintf(file, "Wind Speed: %.2f m/s\n", weather->wind_speed);
    fprintf(file, "Description: %s\n", weather->description);
    fprintf(file, "---------------------------------\n"); // Add a separator for readability
    fclose(file);

    // Print processed data on screen
    printf("City: %s\n", weather->city);
    printf("Temperature: %.2f °C\n", weather->temperature);
    printf("Description: %s\n", weather->description);
}
