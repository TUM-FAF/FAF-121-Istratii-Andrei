# WP Laboratory Work #4

## Title

Windows Timer. Animation.

## Contents

  - Windows timer

## Executed Tasks:
  - **Mandatory Tasks:**
    - Create an animation based on Windows timer which involves at least 5 different drawn objects

  - **Tasks With Points:**
    - Add animated objects which interact with each other (2-6 pt)
      - Create balls by clicking inside the client area.
      - The balls collide with each other and with the borders of the window.
      - The balls are influenced by gravity, and eventually settle down in the bottom of the window.


## Screenshots

  ![Screenshot](https://raw.githubusercontent.com/TUM-FAF/FAF-121-Istratii-Andrei/master/WP/lab%234/screenshots/1.png)

  ![Screenshot](https://raw.githubusercontent.com/TUM-FAF/FAF-121-Istratii-Andrei/master/WP/lab%234/screenshots/2.png)


## Where's the timer?

This application doesn't actually handle the time through WM_TIMER messages, there is no call to SetTimer() function. Instead, Implemented a custom class that uses internaly the high resolution timer that Windows offers through the functions QueryPerformanceFrequency() and QueryPerformanceCounter(). Moreover all the code is not stuffed in the window procedure but is separated in different sections of the program. The handling of the time specificaly occurs in the enhanced event loop similar to those used in real games. The window procedure now handles only windows stuff like resizing and moving the window, while input is intercepted in the loop by sending the arrived message in a special function after it gets into DispatchMessage(). The rendering and updating routine is also abstracted out of the winproc, this allows to focus more on the logic then on the Windows programming.


## Flickering

As in the previous laboratory work the flickering problem was solved using a backbuffer which is essentialy a memory bitmap and all the drawing occurs in its context. After all the stuff was drawn on the backbuffer, the window is invalidated and the contents of the backbuffer are presented on the screen when the app receives the WM_PAINT message.


## Wiggle, wiggle, wiggle

Usually physics engines are among the most complex systems in modern games. Now I know why. To imlement a simple collision algorithm it took a couple hundred lines of code. First of all it was necessary to implement the basic vector math, after that the objects had to be abstracted in a way that would permit easy computations on them. Two dimension collisions are not so complex as 3D, but also require some math to do.

The first problem that appeard with the algorithm was the fact that fast moving objects got overlapped and the collision could not be resolved in one iteration which led the objects to staying together and jumping around unnaturaly. This one was solved by introducing an adjustment step when after all the velocities were calculated, the circles were moved into positions so that they wouldn't generate another collision.

This solution gave the possibility to create multiple objects and even accumulate them in the bottom of the screen however it seems that such approach is not stable for static collisions because the balls were always wiggling like crazy even though I implemented a damping coeficient that was meant to slow the things down.


## Who owns the thread?

An interesting thing I've noticed while running the app in Wine environment on an Ubuntu machine was that the objects kept moving if I moved or resized the window. On the other hand in Windows any motion of this kind led to the full blockage of the process. After some investigation it turned out that when the user drags a window the execution thread enters a loop which is inaccessible to the programmer that is own by the system and exits only when the user releases the window.
