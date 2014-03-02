#Game-Of-Life [wikipedia](http://en.wikipedia.org/wiki/Conway's_Game_of_Life)

Conway's Game of Life made in C. Has some interesting features.

###Command Line Options

####Rules

-b

	Change the rule for how many neighbors a dead cell must have to be born (default: 2). 
	Passing -1 makes it so that a cell is never born.

-s
	
	Change the rule for how many neighbors a live cell must have to stay alive (default: 23). 
	Passing -1 makes it so that a cell never stays alive.

####Misc

-c

        Change the display character for cells (default: '#').
        
-d

        Change the initial density of cells (default: 0.3).
        
-g

        Allow spontaneous generation with a certain probability (default: 0).
        
-i

        Set the amount of iterations per second (default: 5).
        
-w

        Allow wrapping around the edges (off by default).

###Examples

./life -cO -d0 -g.01 -i50

        Watch as life appears from nothing.
        
./life -d.9 -w

        Observe the effects of overcrowding.

./life -d0 -w -b36 -s23

       This is "HighLife". Try inputting the [replicator](http://en.wikipedia.org/wiki/HighLife).
