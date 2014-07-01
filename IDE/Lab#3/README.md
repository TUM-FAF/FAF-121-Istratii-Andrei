# IDE Laboratory Work #3

## Title

Mobile Development.


## Executed Tasks:
  - **Mandatory Tasks:**
    - Study design guidlines for any mobile OS
    - Create a mobile app and launch it on emulator/simulator. Your app should have at least 3 element (ex. button, textarea, statusbar...) and one action (ex. clicking on a button will trigger a popup)

  - **Tasks With Points:**
    - Consume an API and display its data in application. ([link](https://www.kimonolabs.com/worldcup/explorer)) (2pt)
    - Create 3 different views (ex. one with a list of some element, one with some settings and one with application description) (2pt)
    - Debug an application. At some point you'll have bugs (ex. null pointer exception). Set breakpoints and/or watchers. Find out buggy variables. (1pt)
    - Use a hardware device for testing and debugging (1pt)


## Screenshot

  ![Screenshot](https://raw.githubusercontent.com/TUM-FAF/FAF-121-Istratii-Andrei/master/IDE/Lab%233/screenshots/1.png)



## Android attack

Meeting the Android development environment was a bit of a challenge. The user interface of Eclipse IDE provides a wide range of information to the viewer and in the begining it was very easy to get lost in it. 

The application created for this laboratory work represents a simple example of a 'livescore' app. It uses a REST API service to fetch the information about teams and matches of the WorldCup 2014. When the data is obtained, the user sees the last played match and then can see other matches by clicking the 'Next' or 'Prev' button.

All the data is fetched using Asynchronous Tasks.
