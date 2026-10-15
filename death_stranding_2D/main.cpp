#include <iostream>
#include "GamesEngineeringBase.h" // Include the GamesEngineeringBase header

int main()
{

    // Create a canvas window with dimensions 1024x768 and title “Tiles"
    GamesEngineeringBase::Window canvas;
    canvas.create(1024, 768, "Tiles");
    bool running = true; // Variable to control the main loop's running state.

    while (running)
    {
        // Check for input (key presses or window events)
        if (canvas.keyPressed(VK_ESCAPE))
            break;

        // Clear the window for the next frame rendering
        canvas.clear();
        // Update game logic

        // Draw();
        canvas.draw(512, 384, 255, 255, 255);

        // Display the frame on the screen. This must be called once the frame is finished in order to display the frame.
        canvas.present();
    }
    return 0;
}