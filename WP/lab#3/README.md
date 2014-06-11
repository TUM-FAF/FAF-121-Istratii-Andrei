# WP Laboratory Work #3

## Title

Basics of Working with Mouse. GDI Primitives. Bezier Curve.

## Contents

  - The Mouse
  - Device context
  - GDI Primitives
    - Line
    - Curve
    - Plane
    - Bitmap image
  - Bezier Curve


## Executed Tasks:
  - **Mandatory Tasks:**
    - Draw a Bezier Curve.
    - Draw ellipses, rectangles.

  - **Tasks With Points:**
    - Add a "push like" radio button group for switching tools (2 pt)
    - Draw custom bitmap (use as icons for tool switching buttons) (1 pt)
    - Zoom in/out using keyboard (2 pt)

  - **Additional Tasks:**
    - Use the mouse for all manipulations.
    - Support zooming/panning using mouse.
    - Perserve image quality to some extent while zooming.
    - Zoom into the point where the mouse cursor is located at the moment.
    - Add the possibility to draw perfect circles and squares using the SHIFT key when drawing.
    - Add the possibility to chose the color using a colorpicker or make transparent the border and the fill of currently drawn object.
    - Resolve flickering by implementing a king of double-buffering.
    - Imlement the system in an object-oriented manner.

## Screenshot

  ![Screenshot](https://raw.githubusercontent.com/TUM-FAF/FAF-121-Istratii-Andrei/master/WP/lab%233/screenshots/1.png)



## WinAPI and OOP

The Windows API was designed for the C language with a procedural interface and has nothing to do with OOP. However, Windows apps tend to get bigger and bigger and it's quite hard to handle and maintain a large pile of functions. It is much more comfortable to work with the modular aspect of an object-oriented program. Things like the MFC library wrap the WinAPI in C++ classes and make it possible to operate with windows in OOP fashion. On the other hand it is also interesting to invent the bicycle and wrap the API in classes yourself.

The main problem with doing this was creating object-specific window procedures. When the window class is registered it needs a pointer to a static function which is the window procedure, while what we need is a member function that takes an implicit "this" pointer as a parameter. The way I solved this problem is taken from the book _Windows Graphics Programming Win32 GDI and DirectDraw_ by _Feng Yuan_. The principle consists of having a static class function that is registered in a window class and afterwards redirects any request from the system to a object-specific window procedure. Here comes a question: how the window knows which instance of the class should handle the messages? One way to do this is to pass the pointer the instance in the additional LPARAM parameter in the CreateWindowEx() function when the window is create, which is what I did for this laboratory work. The pointer is later used for message redirection to the necessary window procedure / member function.

While working with this system I descovered an interesting post-effect of an object-oriented architecture. I found out that a lot of code repetition can be avoided when working with multiple window classes using inheritance. It is a fact that the majority of window procedures have a common set of messages that they handle leading to literary copied code in several different window procedures. With inheritance, one can define a base window class which handles these common messages by using callbacks, while derived classes can focus on their specific functionality and redirect the rest of the messages to the parent class imlementation of the window procedure, but if they need a different implementation for a common message they just can override the callback which is a virtual function. Using this kind redirection can save the programmer from writing redundant code.


## Canvas, Backbuffer and friends

In a drawing application, an important set of decisions should be made, mainly how the drawable objects are created, stored, updated and ultimately show to the user.

I will begin with the storage options that I had. There were two main possibilities that I considered: store the objects as bitmaps that cannot be edited anymore but only on pixel basis, versus keeping the data that describes a specific object and draw it on the fly. This dilemma basicaly made me choose between raster and vector graphics. I went for the vector stuff because it appeared more flexible in the things that I wanted to implement. So every drawable object stores some data that describe the way it must be displayed, for instance a rectangle is stored as a pair of POINT structures that represent its the top-left and right-bottom corners.

An other question was how to keep all the objects in memory and treat them uniformly while rendering. I created a class "Drawable" that had a virtual method "Render", classes that needed to be drawn could inherit from Drawable and implement the method. All the objects are stored in a linked list in the order they were created, this way, every time the image has to be updated, the program iterates through this list of objects and draws them according to underlying implementation of the derived classes.

All this functionality was encapsulated in a "Canvas" class, which basicaly represents a large bitmap that is used as the context for drawing the objects from the list (which is also kept as a member of the canvas class). Moreover, this class also has a pointer to a temporary drawable object for the purpose of storing there the thing that the user creates at the given moment and after the procedure of drawing is finished the object is appended to the list discussed above.

The fact that the canvas is zoomable and panable has lead to the creation of a Backbuffer class which is a memory bitmap of the size of the viewport that the user sees. The backbuffer acts as an intermediate context for drawing. After the objects are drawn into canvas, its large bitmap is stretched/shrunk according to the zoom/pan values and is BitBlt-ed into the backbuffer contect. When the WM_PAINT message arrives, the buffer is just presented on the screen. The Backbuffer class has also solved the flickering problems in the process of drawing objects or moving the canvas.


## The Dark Side of the Force

Designing well a program that in complexity goes beyond a simple console applications is hard enough and a programmer often fails to find an ideal layout for his app from the first try. This laboratory work is no exception, even though I tried to perserve modularity of the whole system, a lot of problems have appeared. The most obvious is the Canvas class that grew into large "God object" which encapsulated too much responsibility that is normaly divided among a couple of smaller classes. Another difficulty was implementing the communication mechanism between different objects, as the system got larger, it was harder to get some data from the one subsystem to another.

One lesson I extracted from all this stuff is that it takes a lot of thought in order to create a maintainable codebase and there are many interesting things to learn about software design. Another lesson I learnd is that even is the programmer cannot predict all the things he must strive to predict as much as possible of the different situations that may appear in the process of development of the program.