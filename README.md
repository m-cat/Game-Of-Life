Game-Of-Life
============

Conway's Game of Life made in C. Has some interesting features.

##Command Line Options

-c

        Change the display character for cells (default: '#').
        
-d

        Change the initial density of cells (default: 0.3).
        
-g

        Allow spontaneous generation with a certain probability (default: 0).
        
-i

        Set the amount of iterations per second (default: 5).

##Examples

./life -c O -d 0 -g .01 -i 50

        Watch as life appears from nothing.
        
./life -d .9

        Observe the effects of overcrowding.
