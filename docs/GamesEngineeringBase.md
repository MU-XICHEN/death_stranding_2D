# GamesEngineeringBase.h: API Guide and Minimal Examples

This guide describes the implementation currently in [GamesEngineeringBase.h](../death_stranding_2D/include/GamesEngineeringBase.h). All seven classes belong to the `GamesEngineeringBase` namespace. Examples use `GEB` as a short namespace alias.

## Contents

- [1. Overview](#overview)
- [2. Project setup and conventions](#project-setup)
  - [Coordinates and pixels](#coordinates-and-pixels)
- [3. Window: drawing and input](#window)
  - [Creation](#window-creation)
  - [Public operations](#window-public-operations)
  - [Minimal example: a white pixel](#window-example)
  - [Input and lifetime details](#window-details)
- [4. Timer: elapsed seconds](#timer)
  - [Minimal example: move a pixel with the arrow keys](#timer-example)
- [5. Image: RGB/RGBA data](#image)
  - [Public fields and methods](#image-public-api)
  - [Minimal example: create and draw a red image](#image-example)
  - [Loading a file instead](#image-loading-example)
  - [Loading and transparency details](#image-details)
- [6. SoundManager and Sound: WAV audio](#audio)
  - [Minimal example: play an effect once per Space press](#audio-example)
  - [Low-level Sound API](#sound-api)
  - [Current audio limitations](#audio-limitations)
- [7. Xbox controllers: discovery and button polling](#controllers)
  - [XBoxControllers](#xboxcontrollers)
  - [XBoxController](#xboxcontroller)
  - [Minimal example: show a green pixel while A is held](#controller-example)
- [8. Suggested first steps](#suggested-first-steps)
- [9. Example validation](#example-validation)

<a id="overview"></a>
## 1. Overview

| Class | Purpose | Start here when you need to... |
| --- | --- | --- |
| `Window` | Create a window, draw pixels, and read keyboard/mouse input | Display a game and run its frame loop |
| `Timer` | Measure elapsed time in seconds | Move objects independently of frame rate |
| `Image` | Load or create RGB/RGBA pixel data | Display a sprite or inspect its pixels |
| `SoundManager` | Load and play named WAV effects and music | Add basic audio |
| `Sound` | Store WAV data and manage XAudio2 source voices | Work directly with an existing XAudio2 engine |
| `XBoxController` | Poll one XInput controller and set vibration | Read controller buttons |
| `XBoxControllers` | Discover controllers and return controller objects | Find a connected controller |

The library provides a pixel canvas rather than a scene or sprite system. Movement, collision detection, sprite drawing, animation, and game state are application code.

<a id="project-setup"></a>
## 2. Project setup and conventions

Use this project's existing Visual Studio/MSBuild configuration: Windows, MSVC v145, C++20, and Unicode. Its include directory already contains the header:

```cpp
#include "GamesEngineeringBase.h"
namespace GEB = GamesEngineeringBase;
```

The header uses Win32, Direct3D 11, WIC, XAudio2, and XInput. It requests `D3D11.lib`, `D3DCompiler.lib`, `WindowsCodecs.lib`, and `xinput.lib` through linker pragmas. The audio example additionally requests `xaudio2.lib`. Keep the normal Windows system libraries supplied by the existing Visual Studio project.

Each complete example below is a **separate program**. To try one, use it as the contents of `death_stranding_2D/src/main.cpp`; do not combine multiple `main()` functions. Build and run using the existing Visual Studio solution or the VS Code tasks.

Relative asset paths are resolved from the **process working directory**. In this project, that is the inner `death_stranding_2D` directory. For example, `assets/click.wav` means `death_stranding_2D/assets/click.wav`.

<a id="coordinates-and-pixels"></a>
### Coordinates and pixels

- The origin is the top-left pixel. X increases to the right; Y increases downward.
- Window pixels contain three bytes in **RGB** order, with channel values from 0 to 255.
- The byte offset of a window pixel is `(y * width + x) * 3`.
- Image pixels contain either RGB or RGBA data after a successful load. Their byte offset is `(y * width + x) * channels`.
- Window drawing does **not** check bounds. Keep `0 <= x < width` and `0 <= y < height`.

<a id="window"></a>
## 3. Window: drawing and input

<a id="window-creation"></a>
### Creation

```cpp
void create(unsigned int window_width, unsigned int window_height,
            const std::string window_name, bool window_fullscreen = false,
            int window_x = 0, int window_y = 0);
```

Call `create()` once before using any window operation. Windowed mode uses the requested canvas size. Fullscreen mode uses the current screen dimensions instead. The function returns no success/failure value.

<a id="window-public-operations"></a>
### Public operations

| API | Behavior |
| --- | --- |
| `checkInput()` | Process pending window messages and update input state |
| `clear()` | Fill the CPU back buffer with black |
| `draw(x, y, r, g, b)` | Write one RGB pixel |
| `draw(pixelIndex, r, g, b)` | Write one pixel using a linear **pixel** index, not a byte offset |
| `draw(x, y, unsigned char* pixel)` | Copy the first three bytes of a pixel pointer; alpha is ignored |
| `present()` | Upload the back buffer, display it, and process pending messages |
| `getWidth()`, `getHeight()` | Return the canvas dimensions established by `create()` |
| `backBuffer()`, `getBackBuffer()` | Return the same mutable RGB buffer pointer; do not delete it |
| `keyPressed(int key)` | Return the current held state for a virtual-key code; valid indices are 0–255 |
| `mouseButtonPressed(MouseButton button)` | Return whether the specified button is down |
| `mouseButtonState(MouseButton button)` | Return the internal `MouseButtonState` value |
| `getMouseX()`, `getMouseY()` | Return coordinates stored by the last handled mouse event |
| `getMouseInWindowX()`, `getMouseInWindowY()` | Query the current cursor position in client coordinates |
| `getMouseWheel()` | Return the accumulated raw wheel delta |
| `resetMouseWheelPosition()` | Set the accumulated wheel delta to zero |
| `clipMouseToWindow()` | Restrict the cursor to the client area |

`MouseButton` values are `MouseLeft = 0`, `MouseMiddle = 1`, and `MouseRight = 2`.

`MouseButtonState` values are `MouseUp = 0`, `MouseDown = 1`, and `MousePressed = 2`. A button-down message sets `MouseDown`; a later window message advances it to `MousePressed`. This transition is **message-based**, so `MouseDown` is not a reliable one-frame click event. Track previous/current boolean states yourself when you need a single action per press.

<a id="window-example"></a>
### Minimal example: a white pixel

```cpp
#include "GamesEngineeringBase.h"
namespace GEB = GamesEngineeringBase;

int main()
{
    GEB::Window canvas;
    canvas.create(640, 480, "White pixel");

    while (true)
    {
        canvas.checkInput();
        if (canvas.keyPressed(VK_ESCAPE)) break;

        canvas.clear();
        canvas.draw(320, 240, 255, 255, 255);
        canvas.present();
    }
    return 0;
}
```

Use the frame order **input → update → clear → draw → present**. Although `present()` also processes messages, calling `checkInput()` first makes input available before the current update.

<a id="window-details"></a>
### Input and lifetime details

- `keyPressed('A')`, `keyPressed(VK_LEFT)`, and `keyPressed(VK_ESCAPE)` test held state, not a newly pressed edge. Use uppercase virtual-key codes for letters.
- Cached mouse coordinates and the wheel accumulator are not initialized by `create()`. Prefer the current-position getters and call `resetMouseWheelPosition()` before first reading wheel input. Wheel events also store screen coordinates in the cached mouse fields.
- Cursor coordinates can be outside the canvas. Check bounds before passing them to `draw()`. The current-position getters do not convert a resized client area back to the original canvas dimensions.
- Closing the window calls `exit(0)` inside the message handler. There is no `isOpen()` or `shouldClose()` API, and closing this way bypasses automatic local-object destruction. Breaking the loop on Escape returns through normal C++ scope cleanup.
- Resizing does not rebuild the pixel buffer or viewport. For the simplest programs, keep the original window size.
- Create the window on the main thread and process its input there. `create()` also initializes COM on that thread for image loading.
- Do not copy a `Window`, call `create()` repeatedly on it, or destroy an instance that has never been successfully initialized. It owns raw resources without protective copy/lifetime handling. Its destructor also does not free the CPU pixel buffer or explicitly destroy the native window.
- There is no wrapper to release cursor clipping; if used, release it with Win32 `ClipCursor(nullptr)` when appropriate. Presentation uses `Present(0, 0)`, so the loop has no explicit frame-rate limit.

<a id="timer"></a>
## 4. Timer: elapsed seconds

| API | Behavior |
| --- | --- |
| `Timer()` | Initialize the performance-counter frequency and start timing |
| `reset()` | Start a new measurement interval |
| `float dt()` | Return seconds since the previous reset or `dt()` call, then reset |

Read `dt()` **once per frame** and reuse that value. Calling it separately for different objects gives them different, usually much smaller, intervals.

<a id="timer-example"></a>
### Minimal example: move a pixel with the arrow keys

```cpp
#include "GamesEngineeringBase.h"
namespace GEB = GamesEngineeringBase;

int main()
{
    GEB::Window canvas;
    canvas.create(640, 480, "Timed movement");
    GEB::Timer timer;
    float x = 320.0f;

    while (true)
    {
        canvas.checkInput();
        if (canvas.keyPressed(VK_ESCAPE)) break;
        const float dt = std::min(timer.dt(), 0.05f);

        if (canvas.keyPressed(VK_LEFT)) x -= 150.0f * dt;
        if (canvas.keyPressed(VK_RIGHT)) x += 150.0f * dt;
        x = std::clamp(x, 0.0f, static_cast<float>(canvas.getWidth() - 1));

        canvas.clear();
        canvas.draw(static_cast<int>(x), 240, 255, 255, 255);
        canvas.present();
    }
    return 0;
}
```

The speed is 150 pixels per second. The 0.05-second cap is application logic to limit jumps after a pause; `Timer` itself does not clamp elapsed time or implement a fixed update step.

<a id="image"></a>
## 5. Image: RGB/RGBA data

<a id="image-public-api"></a>
### Public fields and methods

| Member | Meaning |
| --- | --- |
| `unsigned int width`, `height` | Pixel dimensions; initially zero |
| `unsigned int channels` | Normally 3 for RGB or 4 for RGBA after loading; initially zero |
| `unsigned char* data` | Owned pixel array; initially `nullptr` |
| `bool load(std::string filename)` | Decode the first image frame using WIC |
| `unsigned char* at(x, y) const` | Return a pixel pointer, clamping unsigned coordinates to the last column/row |
| `unsigned char at(x, y, index) const` | Read one channel with coordinate clamping; the channel index is unchecked |
| `unsigned char alphaAt(x, y) const` | Read alpha for RGBA, or return 255 for non-RGBA |
| `unsigned char* atUnchecked(x, y) const` | Return a pixel pointer without coordinate checks |
| `unsigned char alphaAtUnchecked(x, y) const` | Read alpha without coordinate checks, or return 255 for non-RGBA |
| `bool hasAlpha() const` | Return whether `channels == 4` |
| `free()` | Delete pixel data and set `data` to null; dimensions/channel count stay unchanged |

The destructor calls `free()`. Copying is disabled; moving transfers pixel ownership and clears the source object's fields. If you allocate procedural image data yourself, use `new unsigned char[...]` so the destructor can release it with `delete[]`.

<a id="image-example"></a>
### Minimal example: create and draw a red image

This example needs no external asset and demonstrates that drawing an image means copying its pixels to the window.

```cpp
#include "GamesEngineeringBase.h"
namespace GEB = GamesEngineeringBase;

int main()
{
    GEB::Window canvas;
    canvas.create(640, 480, "Procedural image");

    GEB::Image image;
    image.width = 32;
    image.height = 32;
    image.channels = 3;
    image.data = new unsigned char[image.width * image.height * image.channels]{};
    for (unsigned int y = 0; y < image.height; ++y)
        for (unsigned int x = 0; x < image.width; ++x)
            image.at(x, y)[0] = 255; // Red; green and blue remain zero.

    while (true)
    {
        canvas.checkInput();
        if (canvas.keyPressed(VK_ESCAPE)) break;
        canvas.clear();
        for (unsigned int y = 0; y < image.height; ++y)
            for (unsigned int x = 0; x < image.width; ++x)
                canvas.draw(100 + x, 100 + y, image.at(x, y));
        canvas.present();
    }
    return 0;
}
```

<a id="image-loading-example"></a>
### Loading a file instead

In the previous example, replace the procedural initialization, from `GEB::Image image;` through the red-pixel initialization loops, with this block. Add `<filesystem>` to the includes. Use a known valid image small enough to fit at `(100, 100)` in the canvas.

```cpp
GEB::Image image;
const std::string path = "assets/sprite.png";
if (!std::filesystem::exists(path) || !image.load(path))
    return 1;
if (image.data == nullptr || image.width == 0 || image.height == 0)
    return 1;
if (image.width > canvas.getWidth() - 100 ||
    image.height > canvas.getHeight() - 100)
    return 1;
```

<a id="image-details"></a>
### Loading and transparency details

- Call `load()` after `Window::create()` on the same thread, or initialize COM yourself before using WIC.
- Accepted decoded formats are 24-bit BGR/RGB and 32-bit BGRA/RGBA. BGR/BGRA is converted to RGB/RGBA. Indexed, grayscale, and other formats are not converted automatically; a `.png` extension alone does not guarantee compatibility.
- The implementation checks only some WIC results. A missing, corrupt, or unsupported file can fail before a clean `false` return. Checking existence helps with a missing file but does not make decoding arbitrary files safe.
- The filename conversion widens individual bytes; it is not UTF-8 decoding. Simple ASCII asset paths avoid this limitation.
- `load()` does not free an earlier pixel allocation. Prefer a fresh `Image` for each load, or call `free()` before reloading.
- Pixel access requires valid nonempty data. Clamping does not make an empty or freed image safe. Coordinates are unsigned; negative values converted to unsigned end up clamped to the far edge.
- `draw(x, y, image.at(...))` copies RGB only. For cutout transparency, skip a pixel when `alphaAt(...) == 0`. Partial alpha requires your own blending with the destination RGB value; there is no image-level draw or alpha-blending function.

<a id="audio"></a>
## 6. SoundManager and Sound: WAV audio

`SoundManager` is the simplest public entry point for audio. Construct it after the window so COM is already initialized. Its methods use the filename string as the sound-effect key.

| `SoundManager` API | Behavior |
| --- | --- |
| `SoundManager()` | Create an XAudio2 engine and mastering voice |
| `load(filename)` | Load an effect unless that exact string is already in the map; returns `void` |
| `play(filename)` | Play a previously loaded effect; do nothing if it is not in the map |
| `loadMusic(filename)` | Allocate and load the current music object; returns `void` |
| `playMusic()` | Start the current music with an infinite loop; requires a successfully loaded track |

<a id="audio-example"></a>
### Minimal example: play an effect once per Space press

Supply a known valid PCM WAV file at `death_stranding_2D/assets/click.wav`. This example checks that the file exists, but the loader still assumes valid WAV content and a working audio device.

```cpp
#include <filesystem>
#include "GamesEngineeringBase.h"
#pragma comment(lib, "xaudio2.lib")
namespace GEB = GamesEngineeringBase;

int main()
{
    GEB::Window canvas;
    canvas.create(640, 480, "Sound example");

    const std::string effect = "assets/click.wav";
    if (!std::filesystem::exists(effect)) return 1;
    GEB::SoundManager audio;
    audio.load(effect);
    bool spaceWasDown = false;

    while (true)
    {
        canvas.checkInput();
        if (canvas.keyPressed(VK_ESCAPE)) break;
        const bool spaceIsDown = canvas.keyPressed(VK_SPACE);
        if (spaceIsDown && !spaceWasDown) audio.play(effect);
        spaceWasDown = spaceIsDown;

        canvas.clear();
        canvas.present();
    }
    return 0;
}
```

For music, call `audio.loadMusic("assets/music.wav")` and then `audio.playMusic()` once during initialization, using a valid file. Do not call `playMusic()` every frame.

<a id="sound-api"></a>
### Low-level Sound API

| `Sound` API | Behavior |
| --- | --- |
| `Sound()` | Default constructor; prefer value initialization (`Sound sound{};`) if using directly |
| `bool loadWAV(IXAudio2* xaudio, std::string filename)` | Load RIFF/WAVE data using a caller-supplied engine and create 128 source voices |
| `play()` | Submit/start audio on the next source voice, wrapping the voice index after 128 |
| `playMusic()` | Set infinite looping and submit/start audio on the current source voice |
| `~Sound()` | Delete the audio allocation and destroy source voices |

`Sound` cannot be copied or moved. Keep the XAudio2 engine alive while a sound uses it. Most application code should use `SoundManager` rather than manage the engine and voices directly.

<a id="audio-limitations"></a>
### Current audio limitations

- File reads and XAudio2 initialization are only partially checked. `loadMusic()` discards the loader's result, and `playMusic()` has no null guard. There is no public success query for manager loading.
- `loadWAV()` already queues one buffer on voice 0; the first `play()` queues it again. Initial playback can therefore repeat unexpectedly.
- There are no public stop, pause, volume, or playback-completion methods. Cycling through voices is not a guarantee of 128 freely available simultaneous sounds.
- The WAV file handle is not closed by the loader. The manager destructor releases its engine but does not delete the stored `Sound` objects or explicitly destroy the mastering voice. Repeated `loadMusic()` also replaces the pointer without cleanup.
- Treat these as basic teaching APIs with limited error recovery and resource management. The example demonstrates their usage; it does not correct their implementation.

<a id="controllers"></a>
## 7. Xbox controllers: discovery and button polling

<a id="xboxcontrollers"></a>
### XBoxControllers

| API | Behavior |
| --- | --- |
| `XBoxControllers()` | Probe the XInput slots immediately |
| `probeControllers()` | Refresh which slots are active; call again to notice connections/disconnections |
| `bool hasController()` | Report whether the last probe found any active slot |
| `XBoxController getPlayerController(int index)` | Return a **copy** of a slot; index must be 0 to `XUSER_MAX_COUNT - 1` |
| `XBoxController getFirstPlayerController()` | Return a copy of the first active slot, or an inactive controller with ID -1 |

<a id="xboxcontroller"></a>
### XBoxController

| API | Behavior |
| --- | --- |
| `XBoxController()` | Start inactive with ID -1 |
| `activate(int ID)`, `deactivate()` | Set the slot ID, or set it to -1; activation does not test connectivity |
| `update()` | Poll XInput and update the local state |
| `int getID()` | Return the assigned slot ID; -1 means inactive, not a live connection test |
| `upPressed()`, `downPressed()`, `leftPressed()`, `rightPressed()` | Read held D-pad buttons |
| `startPressed()`, `backPressed()` | Read held Start/Back buttons |
| `lThumbPressed()`, `rThumbPressed()` | Read held stick-click buttons |
| `lShoulderPressed()`, `rShoulderPressed()` | Read held shoulder buttons |
| `APressed()`, `BPressed()`, `XPressed()`, `YPressed()` | Read held face buttons |
| `vibrate(float l, float r)` | Set motor strengths; supply values within [0, 1] and use `(0, 0)` to stop |

<a id="controller-example"></a>
### Minimal example: show a green pixel while A is held

```cpp
#include "GamesEngineeringBase.h"
namespace GEB = GamesEngineeringBase;

int main()
{
    GEB::Window canvas;
    canvas.create(640, 480, "Controller example");
    GEB::XBoxControllers controllers;

    while (true)
    {
        canvas.checkInput();
        if (canvas.keyPressed(VK_ESCAPE)) break;
        controllers.probeControllers();
        auto controller = controllers.getFirstPlayerController();

        canvas.clear();
        if (controller.getID() >= 0)
        {
            controller.update();
            if (controller.APressed())
                canvas.draw(320, 240, 0, 255, 0);
        }
        canvas.present();
    }
    return 0;
}
```

Call `update()` on the **returned controller object** before reading its buttons. The collection does not update button state for you, and getters return copies rather than references. Button methods report held state; track a previous value for actions that should happen once per press.

The implementation calculates stick/trigger values privately but exposes no getters. It also has unchecked arithmetic in right-stick normalization and does not inspect the result of its polling call. Use the public button APIs as shown; do not assume analog access or robust disconnect handling exists. Vibration inputs are not safely clamped for arbitrary values, so validate them before calling `vibrate()` and explicitly stop vibration when finished.

<a id="suggested-first-steps"></a>
## 8. Suggested first steps

1. Run the `Window` example and confirm the white pixel appears.
2. Try the `Timer` example to learn held input and frame-independent movement.
3. Draw the procedural `Image`, then replace it with a known compatible file.
4. Add sound effects or controller buttons only when your game needs them.

The examples deliberately use the functions available in this header. A sprite class, camera, collision system, and scene manager can be built on top of these primitives as the game grows.

<a id="example-validation"></a>
## 9. Example validation

On 2026-10-15, all five complete examples and the file-loading variation were compiled and linked with the current project's Debug x64 settings, MSVC v145, and C++20. External images/WAV files are not included, and asset decoding, audio playback, and physical controller input were not runtime-tested for this guide.
