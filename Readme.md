ORBS
----

Orbs is a game created for the 8BitCADE handheld Arduino-based gaming device. It has a 128 x 64 bit black and white display screen operated by an Atmel 8-bit processor.

Orbs is based on the concept of the PC game Osmos but taken in a different direction.

The user owns an orb that is to capture the other orbs in its universe. The users orb is distinguished by sparkles blinking inside of the orb. To win, all other Orbs are to be captured. When the game starts, the users orb is in the center of the display and a number of different size orbs are around its universe. Each of those orbs will move at a different speed and in different directions. 

Game Play
---------

If any two orbs collide, the larger orb will absorb the smaller orb. The large one will become larger (up to a maximum size) and its speed will slow down in using its energy to capture the smaller orb.

If an orb collides with the users orb, the user orb will absorb the smaller orb and become larger. If the users orb is smaller than the orb in the collision, it will get smaller. If the orb gets too small, it is destroyed and the owner loses. You can quickly tell if an orb is larger and could absorb the users orb because those larger orbs have a white center while those at the same size as the users orb or smaller, are black in their interior. 

Whenever two orbs collide, the green LED will flash.

When the game first starts, the users orb is invincible. This is noted by the orb having a hard thick shell. The other orbs pass through the invincible orb and no collision occurs. If the users orb collides with a larger orb and gets smaller, it becomes invincible again for a short period. 

The UP, DOWN, LEFT, and RIGHT buttons are uses to move the users orb to get out of the way of larger orbs and go after smaller orbs to absorb.

If the user orb is destroyed, the display will flash circles around the last location of the user orb where it collided. It will count this as a loss and restart the game with a different initial universe. If the user orb destroys all the other orbs, it will flash larger and smaller to show it has won and the game will restart.

Orbs bounce off the edges of the universe and continue in the opposite direction. But if they are traveling in a north, east, south, or west direction, they can change to an entirely different direction randomly when they hit the edge.


Missiles
--------

The “B” button puts the game into missile mode. The users orb become invincible when in this mode. It will continuously fire a missile In the direction given by UP, DOWN, LEFT, and RIGHT buttons. The Blue LED flashed while in this mode.

If a missile hits an orb, it will split it in half. Each half will increase in speed imparted by the energy of the missile. Each half could go off in a different direction.

Watching the game can get mesmerizing
-------------------------------------

It is often fascinating to just to leave the game in missile mode and watch the splitting and merging of the orbs.

It is also interesting to just leave the game untouched and let it run on its own game after game.

The number of orbs to be destroyed is always shown on the score line of the display along with the number of wins and losses. 



