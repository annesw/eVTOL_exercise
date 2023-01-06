# eVTOL_exercise

To run the simulation:
from the eVTOL_exercises directory, to the following commands:

  mkdir build
  cd build
  cmake ..
  make
  ./eVTOL

  to run the tests:
  ./eVTOL tests

It has been a very long time since I have used C++ in a shipping product, so I 
am a little rusty. There are some design decisions, that I might have change if
I did this again, but I am happy to discuss why I made the choices I did.

I have left out the normal error checking that a real system would have. For 
example, if an Aircraft releases a charger, check that that Aircraft is the last
Aircraft given access to that charger. 

I would also add more tests, and make a better testing format. Since it has been
so long since I used C++ I made a few rookie mistakes that took a little time to 
understand and that I wouldn't make again :-) , so the exercise took longer than
I expected. 

Another thing I might change, there are a couple global variables that could be 
replaced using static member functions to the classes.

Some of my times may still be off by one second. In general, I would double check
all my numbers.
