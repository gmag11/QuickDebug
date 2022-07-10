# Quick Debug

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/gmag11/library/QuickDebug.svg)](https://registry.platformio.org/libraries/gmag11/QuickDebug)

## Introduction

ESP32 IDF provides great debugging tools that are missing in ESP32 and ESP8266 Arduino cores.

This library tries to mimic ESP32 log output on ESP8266 and adds some useful data to every log message. Besides it allows setting custom log levels to every different debug tag.

## Benefits

QuickDebug library adds extra information to every line:

- Current clock: It lets you know when the line was printed.
- Heap usage: It shows how much memory is used by the program. It allows tracking memory leaks easily.
- Source file name, function name and line number: It shows exactly where every log message comes from.
- Colours: It allows you to see the log messages in different colours. By default it uses colours from ESP32 SDK.
- Tag: It allows you to filter log messages by tag. This is included in native ESP32 log fatures but not in ESP8266.
- Individual tags debug level may be decreased.
- If debug level is set to 0 (NONE) then most of library code is disabled so it is not compiled.

For code simplicity, ESP32 and ESP8266 output format are slightly different.

ESP8266 output example:

```text
E [  5065][H:52392][debug_example.cpp:8] showDebug(): [TAG] Error message
W [  5066][H:52392][debug_example.cpp:9] showDebug(): [TAG] Warning message
I [  5070][H:52392][debug_example.cpp:10] showDebug(): [TAG] Info message
D [  5077][H:52392][debug_example.cpp:11] showDebug(): [TAG] Debug message
V [  5085][H:52392][debug_example.cpp:12] showDebug(): [TAG] Verbose message
```

Format explanation:

| I | [5070] | [H:52392] | [debug_example.cpp:10] | showDebug() | [TAG] | Info message |
|---|---|---|---|---|---|---|
| Message level | Current clock | Free heap | Source file name and line number | Function name | Tag | Message |

ESP32 output example:

```text
[  2061][E][debug_esp32_task.cpp:18] showDebug(): [TAG2] [H:345004][S:6404 T:loopTask] Error message  
[  2062][W][debug_esp32_task.cpp:19] showDebug(): [TAG2] [H:345004][S:6404 T:loopTask] Warning message
[  2071][I][debug_esp32_task.cpp:20] showDebug(): [TAG2] [H:345004][S:6404 T:loopTask] Info message
[  2081][D][debug_esp32_task.cpp:21] showDebug(): [TAG2] [H:345004][S:6324 T:loopTask] Debug message  
[  2090][V][debug_esp32_task.cpp:22] showDebug(): [TAG2] [H:345004][S:6324 T:loopTask] Verbose message
```

Format explanation:

| I | [2071] | [debug_esp32_task.cpp:20] | showDebug() | [TAG2] | [H:345004] | [S:6404 | T:loopTask] | Info message |
|---|---|---|---|---|---|---|---|---|
| Message level | Current clock | Source file name and line number | Function name | Tag | Free heap | Task stack watermark | Task name |  Message |

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
none = 0
error = 1
warn = 2
info = 3
debug = 4
verbose = 5

level = ${debug.verbose} ; set debug level here (none, error, warn, info, debug, verbose)

default_esp32_level = -DCORE_DEBUG_LEVEL=${debug.level}
default_esp8266_level = -DDEBUG_LEVEL=${debug.level}

[esp32_common]
platform = espressif32
board = esp32dev
framework = arduino
build_flags =
    ${debug.default_esp32_level}
    -DCONFIG_ARDUHAL_LOG_COLORS=1 ; enable colours
monitor_filters = time ; add time information to every serial monitor line

[esp8266_common]
platform = espressif8266
board = esp12e
framework = arduino
build_flags =
    ${debug.default_esp8266_level}
    -DCONFIG_ARDUHAL_LOG_COLORS=1 ; enable colours
monitor_filters = time ; add time information to every serial monitor line

[env:esp32_debug]
extends = esp32_common

[env:esp8266_debug]
extends = esp8266_common
```
