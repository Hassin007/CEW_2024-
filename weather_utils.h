#ifndef WEATHER_UTILS_H
#define WEATHER_UTILS_H
#define MAX_READINGS 5


typedef struct {
    char city[50];
    float temperature;
    float temp_min;
    float temp_max;
    float humidity;
    float pressure;
    float wind_speed;
    char description[100];
} WeatherData;

// Function to format city name (First letter uppercase, rest lowercase)
void format_city_name(char *city);

// Function to save raw JSON data to a file
void save_raw_data(const char *filename, const char *data);

// Function to save processed weather data to a file
void save_processed_data(const char *filename, const WeatherData *weather);

// Function to read the execution count from a file
int read_execution_count(const char *filename);

// Function to save the updated execution count to a file
void save_execution_count(const char *filename, int count);

// Function to read the last five weather readings from a file
int read_last_five_readings(const char *filename, WeatherData *readings);

// Function to calculate the average temperature from the readings
float calculate_average_temperature(WeatherData *readings, int count);

#endif

