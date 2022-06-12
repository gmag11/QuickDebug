# Quick Debug

## Introduction
ESP32 SDK provides great debugging tools that are missing in ESP8266 SDK.

This library tries to mimit ESP32 log output on ESP8266 and adds some useful data to every log message.
## Benefits
QuickDebug library adds extra information to every line:

- Current clock: It lets you know when the line was printed.
- Heap usage: It shows how much memory is used by the program. It allows tracking memory leaks easily.
- Source file name, function name and line number: It shows exactly where every log message comes from.
- Colours: It allows you to see the log messages in different colours. By default it uses colours from ESP32 SDK.
- Tag: It allows you to filter log messages by tag. This is included in native ESP32 log fatures but not in ESP8266.

For code simplicity, ESP32 and ESP8266 output format are slightly different.

ESP8266 output example:
```
E [  5065][H:52392][debug_example.cpp:8] showDebug(): [TAG] Error message
W [  5066][H:52392][debug_example.cpp:9] showDebug(): [TAG] Warning message
I [  5070][H:52392][debug_example.cpp:10] showDebug(): [TAG] Info message
D [  5077][H:52392][debug_example.cpp:11] showDebug(): [TAG] Debug message
V [  5085][H:52392][debug_example.cpp:12] showDebug(): [TAG] Verbose message
```

ESP32 output example:
```
[  5026][E][debug_example.cpp:6] showDebug(): [TAG] [H:345304] Error message
[  5027][W][debug_example.cpp:7] showDebug(): [TAG] [H:345304] Warning message
[  5031][I][debug_example.cpp:8] showDebug(): [TAG] [H:345304] Info message
[  5039][D][debug_example.cpp:9] showDebug(): [TAG] [H:345304] Debug message
[  5047][V][debug_example.cpp:10] showDebug(): [TAG] [H:345304] Verbose message
```

## Usage

Debug level must be defined globally.

You must use `CORE_DEBUG_LEVEL` (for ESP32) and `DEBUG_LEVEL` (for ESP8266) macros to define debug level. Possible levels are:

- 0: No debug messages
- 1: Only errors
- 2: Errors and warnings
- 3: Errors, warnings and info
- 4: Errors, warnings, info and debug
- 5: Errors, warnings, info, debug and verbose

It is recommended to set both macros to the same value.

If you want to enable colours you have to define `CONFIG_ARDUHAL_LOG_COLORS` macro to value 1.

If you use PlatformIO, you can define debug level in `platformio.ini` file.

```ini
[debug]
esp32_none = -DCORE_DEBUG_LEVEL=0
none = -DDEBUG_LEVEL=NONE
esp32_error = -DCORE_DEBUG_LEVEL=1
error = -DDEBUG_LEVEL=ERROR
esp32_warn = -DCORE_DEBUG_LEVEL=2
warn = -DDEBUG_LEVEL=WARN
esp32_info = -DCORE_DEBUG_LEVEL=3
info = -DDEBUG_LEVEL=INFO
esp32_debug = -DCORE_DEBUG_LEVEL=4
debug = -DDEBUG_LEVEL=DBG
esp32_verbose = -DCORE_DEBUG_LEVEL=5
verbose = -DDEBUG_LEVEL=VERBOSE

default_level = ${debug.verbose} ; set debug level for esp8266 here
default_esp32_level = ${debug.esp32_verbose} ; set debug level for esp32 here

[esp32_common]
platform = espressif32
board = esp32dev
framework = arduino
build_flags =
    ${debug.default_level} 
    ${debug.default_esp32_level}
    -DCONFIG_ARDUHAL_LOG_COLORS=1
monitor_filters = time

[esp8266_common]
platform = espressif8266
board = esp12e
framework = arduino
build_flags =
    ${debug.default_level}
    -DCONFIG_ARDUHAL_LOG_COLORS=1
monitor_filters = time

[env:esp32_debug]
extends = esp32_common

[env:esp8266_debug]
extends = esp8266_common
```

