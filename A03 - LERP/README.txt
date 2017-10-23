NOTE: For some reason I am unable to run the .exe generated via Debug and Release builds on my computer. I will play around with this and try to get it working.
If you open my code in Visual Studio it does work however and you can set any custom amount of orbits as they work as well.

Notes on my code:
I have added code in five key locations.
One: The top of AppClass.h includes certain member variables, an include, and a typedef.
Two: The definition of the CreateRoute function in AppClass.h under the Initialization region.
Three: The bottom of InitVariables function where I call CreateRoute, initialize fTimers, m_routePositions, and m_routeIndex.
Four: The CreateRoute function which is below the InitVariables function (line 50).
Five: Time code below the for loop in the Display function followed by code to determine each white sphere's position in the Display's for loop.

Easy way to test m_uOrbits in VS:
Change the m_uOrbits = 7 on line 26 of AppClass.cpp to m_uOrbits = whichever value you would like to test.