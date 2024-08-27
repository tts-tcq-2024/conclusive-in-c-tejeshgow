#include <gtest/gtest.h>
#include "typewise-alert.h"

// Test for BreachType inference based on limits
class InferBreachTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(InferBreachTest, ShouldReturnTooLowWhenValueBelowLowerLimit) {
    EXPECT_EQ(inferBreach(-1, 0, 35), TOO_LOW);
}

TEST_F(InferBreachTest, ShouldReturnNormalWhenValueWithinLimits) {
    EXPECT_EQ(inferBreach(20, 0, 35), NORMAL);
}

TEST_F(InferBreachTest, ShouldReturnTooHighWhenValueExceedsUpperLimit) {
    EXPECT_EQ(inferBreach(36, 0, 35), TOO_HIGH);
}

// Test for Temperature Breach Classification based on Cooling Type
class ClassifyTemperatureBreachTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyPassiveCoolingTooLow) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -1), TOO_LOW);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyPassiveCoolingNormal) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 20), NORMAL);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyPassiveCoolingTooHigh) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 36), TOO_HIGH);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyHiActiveCoolingTooLow) {
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1), TOO_LOW);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyHiActiveCoolingNormal) {
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 40), NORMAL);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyHiActiveCoolingTooHigh) {
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyMedActiveCoolingTooLow) {
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1), TOO_LOW);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyMedActiveCoolingNormal) {
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 39), NORMAL);
}

TEST_F(ClassifyTemperatureBreachTest, ShouldClassifyMedActiveCoolingTooHigh) {
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
}

// Test for sending alerts via Email
class SendToEmailTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SendToEmailTest, ShouldSendTooLowEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(TOO_LOW);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");
}

TEST_F(SendToEmailTest, ShouldSendTooHighEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(TOO_HIGH);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

TEST_F(SendToEmailTest, ShouldNotSendEmailForNormalBreach) {
    testing::internal::CaptureStdout();
    sendToEmail(NORMAL);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

// Test for sending alerts to Controller
class SendToControllerTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(SendToControllerTest, ShouldSendToControllerTooLow) {
    testing::internal::CaptureStdout();
    sendToController(TOO_LOW);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 1\n");  // 1 corresponds to TOO_LOW
}

TEST_F(SendToControllerTest, ShouldSendToControllerTooHigh) {
    testing::internal::CaptureStdout();
    sendToController(TOO_HIGH);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");  // 2 corresponds to TOO_HIGH
}

TEST_F(SendToControllerTest, ShouldSendToControllerNormal) {
    testing::internal::CaptureStdout();
    sendToController(NORMAL);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 0\n");  // 0 corresponds to NORMAL
}

// Test for checkAndAlert function
class CheckAndAlertTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(CheckAndAlertTest, ShouldAlertControllerForTooLowBreach) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 1\n");
}

TEST_F(CheckAndAlertTest, ShouldAlertControllerForTooHighBreach) {
    BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, 50);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");
}

TEST_F(CheckAndAlertTest, ShouldNotAlertForNormalBreachWhenToController) {
    BatteryCharacter batteryChar = {MED_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, 20);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 0\n");
}

TEST_F(CheckAndAlertTest, ShouldAlertEmailForTooLowBreach) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, batteryChar, -1);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");
}

TEST_F(CheckAndAlertTest, ShouldAlertEmailForTooHighBreach) {
    BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, batteryChar, 50);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

TEST_F(CheckAndAlertTest, ShouldNotAlertEmailForNormalBreach) {
    BatteryCharacter batteryChar = {MED_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, batteryChar, 20);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}
