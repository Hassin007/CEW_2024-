#!/bin/bash

if [ -z "$1" ]; then
  echo "Please provide a city name as the first argument."
  exit 1
fi

CITY_NAME=$1

# Pass the city name to the program
for num in {1..5}
do
  make clean
  make
  ./weather_app "$CITY_NAME"  # Pass the city name to the program
  sleep 5
done

make clean
