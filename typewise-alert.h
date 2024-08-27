#pragma once

// Cooling types for different battery configurations
typedef enum {
  PASSIVE_COOLING,
  HI_ACTIVE_COOLING,
  MED_ACTIVE_COOLING
} CoolingType;

// Breach types indicating temperature conditions
typedef enum {
  NORMAL,
  TOO_LOW,
  TOO_HIGH
} BreachType;

// Function to infer breach type based on temperature limits
BreachType inferBreach(double value, double lowerLimit, double upperLimit);

// Function to classify temperature breach based on the cooling type
BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC);

// Alert target types
typedef enum {
  TO_CONTROLLER,
  TO_EMAIL
} AlertTarget;

// Battery character structure to hold cooling type and brand information
typedef struct {
  CoolingType coolingType;
  char brand[48];
} BatteryCharacter;

// Main function to check temperature and alert accordingly
void checkAndAlert(
  AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC);

// Function to send alert to controller
void sendToController(BreachType breachType);

// Function to send alert via email
void sendToEmail(BreachType breachType);

// Internal helper function to get the temperature limits for a given cooling type
static void getTemperatureLimits(CoolingType coolingType, double* lowerLimit, double* upperLimit);

// Internal helper function to process the alert based on the breach type and alert target
static void processAlert(AlertTarget alertTarget, BreachType breachType);

