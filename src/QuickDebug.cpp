
#include "QuickDebug.h"

DebugTagManager debugTagManager;

#if CORE_DEBUG_LEVEL || DEBUG_LEVEL


#ifdef ESP8266
const char* IRAM_ATTR extractFileName (const char* path) {
    size_t i = 0;
    size_t pos = 0;
    char* p = (char*)path;
    while (*p) {
        i++;
        if (*p == '/' || *p == '\\') {
            pos = i;
        }
        p++;
    }
    return path + pos;
}

#endif // ESP8266

void DebugTagManager::setTagLevel (const std::string& tagName, int level) {
    bool found = false;

    for (std::list<LogTagDebugLevel_t>::iterator tagLevel = tagLevels.begin (); tagLevel != tagLevels.end (); ++tagLevel) {
        if (tagLevel->tagName == tagName) {
            tagLevel->level = level;
            found = true;
            break;
        }
    }

    if (!found) {
        LogTagDebugLevel_t tagLevel;
        tagLevel.tagName = tagName;
        tagLevel.level = level;
        tagLevels.push_back (tagLevel);
    }
}

int DebugTagManager::getTagLevel (const std::string& tagName) {
    for (LogTagDebugLevel_t tagLevel : tagLevels) {
        if (tagLevel.tagName == tagName) {
#ifdef ESP32
            if (tagLevel.level < CORE_DEBUG_LEVEL) {
                return tagLevel.level;
            } else {
                return CORE_DEBUG_LEVEL;
            }
#else
            if (tagLevel.level < DEBUG_LEVEL) {
                return tagLevel.level;
            } else {
                return DEBUG_LEVEL;
            }
#endif // ESP32
        }
    }
#if defined ESP32
    return CORE_DEBUG_LEVEL;
#elif defined ESP8266
    return DEBUG_LEVEL;
#endif
}

void DebugTagManager::setTagToDefaultLevel (const std::string& tagName) {
    for (std::list<LogTagDebugLevel_t>::iterator tagLevel = tagLevels.begin (); tagLevel != tagLevels.end (); ++tagLevel) {
        if ((tagLevel->tagName) == tagName) {
            DEBUG_INFO ("DEBUG", "Tag %s deleted", tagLevel->tagName.c_str ());
#ifdef ESP32
            tagLevels.erase (tagLevel);
#else
            tagLevel->level = VERBOSE;
#endif // ESP32
        }
    }
}

std::string DebugTagManager::getTagLevelStr (const std::string& tagName) {
    std::string tagLevelStr;

    switch (debugTagManager.getTagLevel (tagName)) {
    case ERROR:
        tagLevelStr = "ERROR";
        break;
    case WARN:
        tagLevelStr = "WARN";
        break;
    case INFO:
        tagLevelStr = "INFO";
        break;
    case DBG:
        tagLevelStr = "DBG";
        break;
    case VERBOSE:
        tagLevelStr = "VERBOSE";
        break;
    case NO_DEBUG:
        tagLevelStr = "NONE";
        break;
    default:
        tagLevelStr = "UNKNOWN";
        break;
    }
    return tagLevelStr;
}

#endif