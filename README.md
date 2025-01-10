Snake Game

Overview

Snake Game is a modern version of the classic snake game that many of us played on Nokia phones. The original Snake game (Finnish: Matopeli) was created by Taneli Armanto in 1998 for the Nokia 6110 phone. This version recreates the nostalgic gameplay using C++ and the SFML (Simple and Fast Multimedia Library) framework. In this game, you control a snake that grows longer as it eats apples. The goal is to keep the snake from colliding with the walls, itself, or going out of bounds while achieving the highest score possible.
Snake Game is a modern implementation of the classic snake game using the SFML (Simple and Fast Multimedia Library) framework. In this game, you control a snake that grows longer as it eats apples. The goal is to keep the snake from colliding with the walls, itself, or going out of bounds while achieving the highest score possible.


Controls

Arrow Keys: Move the snake in the desired direction.


Prerequisites

SFML library installed on your system.

A C++ compiler that supports C++11 or higher.

A font file (ArialBold.ttf) and textures (apple2.png, headUp.png, headLeft.png, headRight.png, headDown.png, headUpRight.png, headUpLeft.png, headDownRight.png, headDownLeft.png, bodyUp.png, bodyHoriz.png, tailUp.png, tailLeft.png, tailRight.png, tailDown.png) in a Resources folder inside your project directory.



Setting Up the Project

Working Directory Configuration in Xcode

To ensure the textures and font files load correctly, you need to set the working directory to the project folder (where the Resources folder is) in Xcode.

Open Your Project in Xcode:

Launch Xcode and open the project for which you want to set the working directory.

Access the Scheme Settings:

In the top menu, click on Product > Scheme > Edit Scheme.


Select the Options Tab:

In the "Edit Scheme" dialog, you’ll see a sidebar on the left with different sections like Build, Run, etc.

Click on the Run section, and then go to the Options tab on the right.

Set the Working Directory:

In the Options tab, you’ll see a checkbox for Use Custom Working Directory.

Check this box, and a file picker will appear.

Navigate to your project folder where the Resources folder is located and select it.

Apply Changes:

Click Close to save your changes and exit the dialog.

This setup ensures the game can load the PNG textures and font files correctly. It also enables keyboard interactions for gameplay.


Building and Running

Clone the repository or download the source code.

Place the Resources folder in the project directory. Ensure it contains all required texture and font files.

Open the project in Xcode or your preferred IDE.

Build the project.

Run the game.



Acknowledgments

Developed using the SFML library.

Inspired by the classic Snake game.

Old school :)
