#include "typewise-alert.h"
#include <stdio.h>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if (value < lowerLimit) {
    return TOO_LOW;
  }
  if (value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  switch (coolingType) {
    case PASSIVE_COOLING:
      return inferBreach(temperatureInC, 0, 35);
    case HI_ACTIVE_COOLING:
      return inferBreach(temperatureInC, 0, 45);
    case MED_ACTIVE_COOLING:
      return inferBreach(temperatureInC, 0, 40);
    default:
      return NORMAL;
  }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);

  if (alertTarget == TO_CONTROLLER) {
    sendToController(breachType);
  } else if (alertTarget == TO_EMAIL) {
    sendToEmail(breachType);
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendEmail(const char* recipient, const char* message) {
  printf("To: %s\n", recipient);
  printf("Hi, %s\n", message);
}

void sendToEmail(BreachType breachType) {
  const char* recipient = "a.b@c.com";

  switch (breachType) {
    case TOO_LOW:
      sendEmail(recipient, "the temperature is too low");
      break;
    case TOO_HIGH:
      sendEmail(recipient, "the temperature is too high");
      break;
    case NORMAL:
    default:
      break;  // No action needed for NORMAL
  }
}
