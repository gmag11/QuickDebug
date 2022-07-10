/**
  * @brief Auxiliary functions for debugging over Serial
  *
  * Format used on debug functions is the same as `printf()`. Check source code for usage examples
  * Debug calls will be enabled or disabled automatically before compiling according defined `DEBUG_LEVEL`.
  *
  * If `DEBUG_ESP_PORT` is not defined library will give no debug output at all
  *
  * @file EnigmaIOTdebug.h
  * @version 0.9.8
  * @date 15/07/2021
  * @author German Martin
  */

#ifndef _DEBUG_h
#define _DEBUG_h

#define NO_DEBUG	0 ///< @brief Debug level that will give no debug output
#define ERROR	1 ///< @brief Debug level that will give error messages
#define WARN	2 ///< @brief Debug level that will give error and warning messages
#define INFO	3 ///< @brief Debug level that will give error, warning and info messages
#define DBG	    4 ///< @brief Debug level that will give error, warning,info AND dbg messages
#define VERBOSE	5 ///< @brief Debug level that will give all defined messages

#include <Arduino.h>
#ifdef ESP32
#include <esp_log.h>
#define DEBUG_LEVEL CORE_DEBUG_LEVEL 
#endif
#include <string>
#include <list>

typedef struct {
    std::string tagName;
    int level;
} LogTagDebugLevel_t;

class DebugTagManager {

#if CORE_DEBUG_LEVEL || DEBUG_LEVEL

protected:
    std::list<LogTagDebugLevel_t> tagLevels;

public:
    void setTagLevel (const std::string& tagName, int level);
    void setTagToDefaultLevel (const std::string& tagName);
    int getTagLevel (const std::string& tagName);
    std::string getTagLevelStr (const std::string& tagName);

#else
public:
    void setTagLevel (...) {}
    void setTagToDefaultLevel (...) {}
    int getTagLevel (...) { return NO_DEBUG; }
    std::string getTagLevelStr (...) { return "NONE"; }
#endif // CODE_DEBUG_LEVEL > 0

};

extern DebugTagManager debugTagManager;

#define setTagDebugLevel(tagName,level) debugTagManager.setTagLevel(tagName, level)
#define setTagToDefaultDebugLevel(tagName) debugTagManager.setTagToDefaultLevel(tagName)
#define getTagDebugLevel(tagName) debugTagManager.getTagLevel(tagName)
#define getTagDebugLevelStr(tagName) debugTagManager.getTagLevelStr(tagName)

#ifdef ESP8266
#ifndef CONFIG_ARDUHAL_LOG_COLORS
#define CONFIG_ARDUHAL_LOG_COLORS 0
#endif

#if CONFIG_ARDUHAL_LOG_COLORS
#define ARDUHAL_LOG_COLOR_BLACK   "30"
#define ARDUHAL_LOG_COLOR_RED     "31" //ERROR
#define ARDUHAL_LOG_COLOR_GREEN   "32" //INFO
#define ARDUHAL_LOG_COLOR_YELLOW  "33" //WARNING
#define ARDUHAL_LOG_COLOR_BLUE    "34"
#define ARDUHAL_LOG_COLOR_MAGENTA "35"
#define ARDUHAL_LOG_COLOR_CYAN    "36" //DEBUG
#define ARDUHAL_LOG_COLOR_GRAY    "37" //VERBOSE
#define ARDUHAL_LOG_COLOR_WHITE   "38"

#define ARDUHAL_LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define ARDUHAL_LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define ARDUHAL_LOG_RESET_COLOR   "\033[0m"

#define ARDUHAL_LOG_COLOR_E       ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_RED)
#define ARDUHAL_LOG_COLOR_W       ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_YELLOW)
#define ARDUHAL_LOG_COLOR_I       ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_GREEN)
#define ARDUHAL_LOG_COLOR_D       ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_CYAN)
#define ARDUHAL_LOG_COLOR_V       ARDUHAL_LOG_COLOR(ARDUHAL_LOG_COLOR_GRAY)
#define ARDUHAL_LOG_COLOR_PRINT(letter) log_printf(ARDUHAL_LOG_COLOR_ ## letter)
#define ARDUHAL_LOG_COLOR_PRINT_END log_printf(ARDUHAL_LOG_RESET_COLOR)
#else
#define ARDUHAL_LOG_COLOR_E
#define ARDUHAL_LOG_COLOR_W
#define ARDUHAL_LOG_COLOR_I
#define ARDUHAL_LOG_COLOR_D
#define ARDUHAL_LOG_COLOR_V
#define ARDUHAL_LOG_RESET_COLOR
#define ARDUHAL_LOG_COLOR_PRINT(letter)
#define ARDUHAL_LOG_COLOR_PRINT_END
#endif
#endif //ESP8266

#ifdef ESP8266

#define DEBUG_ESP_PORT Serial

#ifdef DEBUG_ESP_PORT

const char* extractFileName (const char* path);
#define DEBUG_LINE_PREFIX(TAG) DEBUG_ESP_PORT.printf(PSTR("[%6lu][H:%5lu][%s:%d] %s(): ["),millis(),(unsigned long)ESP.getFreeHeap(),extractFileName(__FILE__),__LINE__,__FUNCTION__); DEBUG_ESP_PORT.print(TAG); DEBUG_ESP_PORT.print("] ");

#if DEBUG_LEVEL >= VERBOSE
#define DEBUG_VERBOSE(TAG,text,...) if(debugTagManager.getTagLevel(TAG) >= VERBOSE){DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_V "V ");DEBUG_LINE_PREFIX(TAG);DEBUG_ESP_PORT.printf(PSTR(text),##__VA_ARGS__);DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);}
#else
#define DEBUG_VERBOSE(...)
#endif

#if DEBUG_LEVEL >= DBG
#define DEBUG_DBG(TAG,text,...) if(debugTagManager.getTagLevel(TAG) >= DBG){DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_D "D ");DEBUG_LINE_PREFIX(TAG); DEBUG_ESP_PORT.printf(PSTR(text),##__VA_ARGS__);DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);}
#else
#define DEBUG_DBG(...)
#endif

#if DEBUG_LEVEL >= INFO
#define DEBUG_INFO(TAG,text,...) if(debugTagManager.getTagLevel(TAG) >= INFO){DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_I "I ");DEBUG_LINE_PREFIX(TAG);DEBUG_ESP_PORT.printf(PSTR(text),##__VA_ARGS__);DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);}
#else
#define DEBUG_INFO(...)
#endif

#if DEBUG_LEVEL >= WARN
#define DEBUG_WARN(TAG,text,...) if(debugTagManager.getTagLevel(TAG) >= WARN){DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_W "W ");DEBUG_LINE_PREFIX(TAG);DEBUG_ESP_PORT.printf(PSTR(text),##__VA_ARGS__);DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);}
#else
#define DEBUG_WARN(...)
#endif

#if DEBUG_LEVEL >= ERROR
#define DEBUG_ERROR(TAG,text,...) if(debugTagManager.getTagLevel(TAG) >= ERROR){DEBUG_ESP_PORT.print(ARDUHAL_LOG_COLOR_E "E ");DEBUG_LINE_PREFIX(TAG);DEBUG_ESP_PORT.printf(PSTR(text),##__VA_ARGS__);DEBUG_ESP_PORT.println(ARDUHAL_LOG_RESET_COLOR);}
#else
#define DEBUG_ERROR(...)
#endif

#else
#define enableDebugOutput(...)
#define DEBUG_VERBOSE(...)
#define DEBUG_DBG(...)
#define DEBUG_INFO(...)
#define DEBUG_WARN(...)
#define DEBUG_ERROR(...)
#endif


#elif defined ESP32

#define VERBOSE 5
#define DBG 4
#define INFO 3
#define WARN 2
#define ERROR 1
#define NONE 0

#ifdef GET_TASK_STACK_INFO
#define DEBUG_VERBOSE(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= VERBOSE){ESP_LOGV (TAG,"[H:%6d][S:%d T:%s] " format, ESP.getFreeHeap(), uxTaskGetStackHighWaterMark(NULL), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_DBG(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= DBG){ESP_LOGD (TAG,"[H:%6d][S:%d T:%s] " format, ESP.getFreeHeap(), uxTaskGetStackHighWaterMark(NULL), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_INFO(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= INFO){ESP_LOGI (TAG,"[H:%6d][S:%d T:%s] " format, ESP.getFreeHeap(), uxTaskGetStackHighWaterMark(NULL), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_WARN(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= WARN){ESP_LOGW (TAG,"[H:%6d][S:%d T:%s] " format, ESP.getFreeHeap(), uxTaskGetStackHighWaterMark(NULL), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_ERROR(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= ERROR){ESP_LOGE (TAG,"[H:%6d][S:%d T:%s] " format, ESP.getFreeHeap(), uxTaskGetStackHighWaterMark(NULL), pcTaskGetName(NULL), ##__VA_ARGS__);}
#else
#define DEBUG_VERBOSE(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= VERBOSE){ESP_LOGV (TAG,"[H:%6d][T:%s] " format, ESP.getFreeHeap(), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_DBG(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= DBG){ESP_LOGD (TAG,"[H:%6d][T:%s] " format, ESP.getFreeHeap(), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_INFO(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= INFO){ESP_LOGI (TAG,"[H:%6d][T:%s] " format, ESP.getFreeHeap(), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_WARN(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= WARN){ESP_LOGW (TAG,"[H:%6d][T:%s] " format, ESP.getFreeHeap(), pcTaskGetName(NULL), ##__VA_ARGS__);}
#define DEBUG_ERROR(TAG,format,...) if(debugTagManager.getTagLevel(TAG) >= ERROR){ESP_LOGE (TAG,"[H:%6d][T:%s] " format, ESP.getFreeHeap(), pcTaskGetName(NULL), ##__VA_ARGS__);}
#endif // GET_TASK_STACK_INFO
#endif //ESP32

#define LOG_ERROR_IF_NON_ZERO(TAG,ERROR_CODE,format,...) if(debugTagManager.getTagLevel(TAG) >= ERROR){if(ERROR_CODE != 0){DEBUG_ERROR (TAG, "Code: %d. " format, ERROR_CODE, ##__VA_ARGS__);}}
#define LOG_ERROR_IF_ZERO(TAG,ERROR_CODE,format,...) if(debugTagManager.getTagLevel(TAG) >= ERROR){if(ERROR_CODE == 0){DEBUG_ERROR (TAG, "Code: 0. " format, ##__VA_ARGS__);}}

#define LOG_IF_CODE(ERROR_LEVEL,TAG,ERROR_CODE,CODE,format,...) if (ERROR_CODE == CODE){DEBUG_ ##ERROR_LEVEL (TAG, "Code: %d. " format, ERROR_CODE, ##__VA_ARGS__);}

#endif

