Laboratory Work Nr.1
====================

The purpose of this laboratory work was to obtain the basic control creating and message handling skills. The application window is separated in two parts: the left one has 3 controls with basic setup and default font, the right one features a custom background and the controlls have some additional styles besides a more appealing font.

The main window is resizable within certain limits:

|     | Width | Height |
|-----|------:|-------:|
| Min |   400 |    200 |
| Max |   800 |    400 |

Moreover, the controls and the background adjust themselves so that the window remains separated in two equal parts.

### The functionality

* The background can be toggled between two colors using the button on the right:

![App Screenshot](https://raw.github.com/Andreis13/WP-Labs/master/lab%231/screenshot.png)

* To quit the application one must type 'exit' in the edit control on the right, a hint to do so is provided when the close button is pressed.

* When in focus, the background of the edit control on the right is changed to a color similar to the app's background.

* The button on the left takes the text from the edit below and outputs a big and ugly version of it on the whole desktop. The screen is refreshed after one second and the madness disappears.

* The text of the static control on the left is changed to the contents of the edit control whenever the user starts to edit that text. When the edit control looses keyboard focus, the static text returns to the original value.
