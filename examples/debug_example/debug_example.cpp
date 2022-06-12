#include <QuickDebug.h>

static const char* TAG = "TAG";

void showDebug () {
    DEBUG_ERROR (TAG, "Error message");
    DEBUG_WARN (TAG, "Warning message");
    DEBUG_INFO (TAG, "Info message");
    DEBUG_DBG (TAG, "Debug message");
    DEBUG_VERBOSE (TAG, ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_MAGENTA) "Verbose message");
}

int32_t giveNumber (int32_t number) {
    return number;
}

void setup () {
    Serial.begin (115200);
    Serial.println ();
    delay (5000);
    showDebug ();
    LOG_ERROR_IF_NON_ZERO (TAG, giveNumber(-1), "Error message");
    LOG_ERROR_IF_ZERO (TAG, giveNumber (0), "Error message");
    LOG_IF_CODE (WARN, TAG, giveNumber (-1), -1, "Warning message");
    delay (1000);
}

void loop () {
    delay (0);
}