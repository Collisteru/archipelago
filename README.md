CSCI 4229 FINAL PROJECT
"Stochastic Archipelago"

By Sean Carter

Demonstration of stochastic computer science functions in an interactive setting.

Utilizes functions and basic rendering code from the CSCIx229.h library by:
Willem A. (Vlakkies) Schreuder

What I'm proud of:
- The Perlin Noise algorithm is a novel implementation designed specifically for computer grpahics
-- It accomadates a variable number of nodes (area of the noise) and a variable density for vectors (granularity of the noise)
-- It also supports multiple noise octaves (weaker noise on top of noise, like a Fourier Transform), allowing for more naturalistic terrain
- The implementation features full first-person perspective navigation
- The implementation includes a variation on the Boids algorithm by Craig Reynolds
-- This variation is modified to include a new centrality factor that makes boids tend towards the center of the scene. This makes it less likely for boids to hit the boundary wall. (If they do hit the boundary wall they simply bounce off, but without this factor they tend to bounce between the walls like pinballs.)
-- Boids exhibit naturalistic behavior and demonstrate another stochastic algorithm


Why I should get an A:
- I implemented the Perlin noise and many other features that contributed to the naturalistic archipelago mentioned in the project proposal, including the skybox and the boids
- While I did not have time to implement the many creatures and objects mentioned in the project proposal, I made up for this by implementing extra control features, the skybox, and Boids governed by a complex algorithm.


HOW TO RUN THE PROGRAM:
- Type 'make' into your terminal 
- Run ./final

HOW TO USE THE PROGRAM:

For movement controls, HOLDING SHIFT WHILE PRESSING THE MOVE BUTTON ALLOWS YOU TO GO FASTER. This makes navigation much easier so please use this.

- Up -- Look up
- Down -- Look down
- Left -- Look left
- Right -- look right
- u/U -- Move up (slow/fast)
- j/J -- Move down
- w/W -- Move forward
- s/S -- Move backward
- a/A -- Move leftward
- d/D -- move rightward
