#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <cjson/cJSON.h>
#include "weather_utils.h"

// Callback function to write data into a string
size_t WriteCallback(void *contents, size_t size, size_t nmemb, char *userp) {
    size_t total_size = size * nmemb;
    strncat(userp, contents, total_size < 4096 ? total_size : 4096 - strlen(userp) - 1);
    return total_size;
}

int main(int argc, char *argv[]) {
    system("clear");

    if (argc < 2) {
        fprintf(stderr, "Please provide a city name as a command-line argument.\n");
        return 1;
    }

    CURL *curl;
    CURLcode res;

    const char *api_key = "ce18c3b2bf0093492bf3b15733e685ca";
    char city[100];
    char url[256];
    char response[4096] = {0};

    // Get the city name from the command-line argument
    strncpy(city, argv[1], sizeof(city) - 1);
    city[sizeof(city) - 1] = '\0';  // Ensure null termination

    // Format the city name
    format_city_name(city);

    // Define the file where execution count will be stored
    const char *execution_count_file = "execution_count.txt";

    // Get the current execution count
    int execution_count = read_execution_count(execution_count_file);

    // Increment the execution count
    execution_count++;

    // Save the updated execution count
    save_execution_count(execution_count_file, execution_count);

    snprintf(url, sizeof(url), "https://api.openweathermap.org/data/2.5/weather?q=%s&appid=%s&units=metric", city, api_key);

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize cURL\n");
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);

    if (strlen(response) == 0) {
        fprintf(stderr, "No data received from API\n");
        return 1;
    }

    // Save raw data
    save_raw_data("raw_data.json", response);

    // Parse JSON response and populate WeatherData structure
    WeatherData weather;
    cJSON *json = cJSON_Parse(response);
    if (json == NULL) {
        fprintf(stderr, "Failed to parse JSON response\n");
        return 1;
    }

    cJSON *name = cJSON_GetObjectItem(json, "name");
    if (name && cJSON_IsString(name)) {
        strncpy(weather.city, name->valuestring, sizeof(weather.city) - 1);
    }

    cJSON *main = cJSON_GetObjectItem(json, "main");
    if (main) {
        cJSON *temp = cJSON_GetObjectItem(main, "temp");
        cJSON *temp_min = cJSON_GetObjectItem(main, "temp_min");
        cJSON *temp_max = cJSON_GetObjectItem(main, "temp_max");
        cJSON *humidity = cJSON_GetObjectItem(main, "humidity");
        cJSON *pressure = cJSON_GetObjectItem(main, "pressure");

        if (temp && cJSON_IsNumber(temp)) weather.temperature = temp->valuedouble;
        if (temp_min && cJSON_IsNumber(temp_min)) weather.temp_min = temp_min->valuedouble;
        if (temp_max && cJSON_IsNumber(temp_max)) weather.temp_max = temp_max->valuedouble;
        if (humidity && cJSON_IsNumber(humidity)) weather.humidity = humidity->valuedouble;
        if (pressure && cJSON_IsNumber(pressure)) weather.pressure = pressure->valuedouble;
    }

    cJSON *wind = cJSON_GetObjectItem(json, "wind");
    if (wind) {
        cJSON *wind_speed = cJSON_GetObjectItem(wind, "speed");
        if (wind_speed && cJSON_IsNumber(wind_speed)) weather.wind_speed = wind_speed->valuedouble;
    }

    cJSON *weatherArray = cJSON_GetObjectItem(json, "weather");
    if (weatherArray && cJSON_IsArray(weatherArray)) {
        cJSON *weatherObject = cJSON_GetArrayItem(weatherArray, 0);
        cJSON *description = cJSON_GetObjectItem(weatherObject, "description");
        if (description && cJSON_IsString(description)) {
            strncpy(weather.description, description->valuestring, sizeof(weather.description) - 1);
        }
    }

    cJSON_Delete(json);

    // Save processed data
    save_processed_data("processed_data.txt", &weather);

    if (weather.temperature > 30) {
        printf("WARNING! High Temperature: %.2f°C\n", weather.temperature);
        system("echo 'High Temperature Warning! Current temperature exceeds the threshold.'");
    }
    if (weather.temperature > 50) {
        printf("WARNING! Extremely High Temperature: %.2f°C\n", weather.temperature);
        system("echo 'EXTREMELY High Temperature Warning! Current temperature exceeds the threshold.'");
    }
    if (weather.temperature < 15) {
        printf("Alert! Cold Outside: %.2f°C\n", weather.temperature);
        system("echo 'Low Temperature Warning! Current temperature below the threshold.'");
    }

    // After 5 executions, calculate the average temperature
    if (execution_count % 5 == 0) {
        WeatherData readings[MAX_READINGS];
        int count = read_last_five_readings("processed_data.txt", readings);
        if (count > 0) {
            float avg_temp = calculate_average_temperature(readings, count);
            printf("\n-------------------------------------------------------");
            printf("\nAverage Temperature from last %d readings: %.2f°C\n", count, avg_temp);
            printf("\n-------------------------------------------------------\n");
        } else {
            printf("\nNo readings found to calculate average temperature.\n");
        }
    }

    return 0;
}
