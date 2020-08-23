This README is just a copy of the 'README.md' file from the repo. If both are available, it's recommended that you look at the .md file instead.
Repository for Bonus Fruit second year game development engine and game.
The game is named 'Codename: ZERO'.

Due to the COVID-19 pandemic, GameCon for Winter 2020 was not held. It is assumed that some replacement will be held Fall 2020, but that is still TBD.
The game was worked on over the summer to fix various issues it had. It is not perfected, but it is in a much better state than it was before.
If a makeup GameCon does happen, this code may be revisited. But at this time there are no plans to.

TEAM: BONUS FRUIT
- Kennedy Adams	(100632983)
- Jonah Griffin	(100702748)
- Nathan Tuck	(100708651)
- Spencer Tester (100653129)
- Stephane Gagnon (100694227)
- Roderick "R.J." Montague (100701758)
- Ryan Burton (100707511)
	- NOTE: this developer is no longer part of the program.

GAME: CODENAME: ZERO

*****
NOTES
*****
- information regarding audio:
	- the isEventPlaying() function for the Audio Engine does not work for some reason.
	- the audio banks only need to be loaded once, so it's best just to load them all at the start. An event cannot be unloaded.
	- If you tell the audio to play, it will start over. So if the audio doesn't play, it's likely because it keeps getting started over.
	- fmod cannot play the same effect multiple times at once. Any attempt to do so will simply restart said sound effect.
		- It is because of this reason that the sound wasn't working in the original build. This has been fixed for the most part.
		- There are unused sounds that could be added in, and audio balancing that could be done as well.
	- the only way to really do volume control is to change the z-position of the audio.
- Sounds and music do not trigger consistently. We are unsure of why this is, but the sound may not always work.
	- Notably the engine project (BF PJT) does not have any audio enabled.
- the game now builds a scene from scratch whenever it is loaded up, destroying it and remaking it when it is to be used.
	- originally, the game would open and close a scene, which would have some data preserve between scene switches.
	- this would cause artifacts and graphcis not being deleted properly.
	- it takes longer to load this way, but it was the only way to stop the crashes. 
- the ranking system has been added, and is filled in with the ImGui window. This was not completed in the original build. 
	- the ImGui window is supposed to disappear once the player enters their score, but sometimes it doesn't.
	- if the player's score cracks the top 10, the ImGui window comes up for them to enter their name.
	- the ranking system tracks the name, map, and player score.
- the mouse clicks can be a bit finicky. Try looking at the C++ console to see if shaders are being compiled after a mouse click. If so, then the scene is being switched.
	- there are sound prompts, but the confirmation sound will only play once next scene loads completely (if applicable).
- User Interface in the Codename: Zero Gameplay Scene:
	- the top right section shows the amount of lives the player has
	- the top right section shows the player's score
	- the bottom left section shows how much of the time stop effect the player has charged up (this happens automatically).
		- if at its maximum value, the player can activate the time stop. When the effect is activated, the timer counts down.
		- when it reaches 0, the effect stops. It then counts up to when it can be used again.
			- the text turns red when this happens. It becomes white once the time stop has been fully charged.

**********************************
CHERRY ENGINE - GDW_Y2 - BF PJT
**********************************
- CONTROLS (ENGINE) -
-- Title Scene --
Use the mouse to click on the title to enter the game.

-- Gameplay Scene --
(W) - move camera up
(S) - move camera down
(A) - move camera left
(D) - move camera right
(Q) - zoom camera out
(E) - zoom camera in

UP ARROW - rotate camera upwards
DOWN ARROW - rotate camera downwards
LEFT ARROW - rotate camera to the left
RIGHT ARROW - rotate camera to the right
Page Up - rotate camera clockwise
Page Down - rotate camera counter-clockwise

(<) Turn Down Music (Gameplay Scene only)
(>) Turn Up Music (Gameplay Scene only)

(I) - Switch Path
(P) - Pause Path
(L) - look at world origin

(0) - Delete Item
(1) - No Post-Processing
(2) - Scales1 (Invert, Greyscale, Sepia, Gamma Correction)
(3) - Scales2 (Red, Blue, Green)
(4) - Kernel/Visuals (Edge Detect, Sharpen)
(5) - Lighting (Lighting, Shadows, Cel)
(6) - Blur (Box, Gaussian 3X3, Gaussian 5X5, Radial-Circular, Radial-Linear, Radial-Zoom In, Radial - Zoom Out)
(7) - Bloom (Box, Gaussian, Radial)
(8) - Camera (Depth of Field, Motion Blur)
(9) - Look Up Tables (Cool, Warm)

**********************************
CODENAME: ZERO - GDW_Y2 - CNZ
**********************************
- CONTROLS (GAME) -
-- Title Scene --
Use the mouse to click on buttons.
Use (I) to view in-game controls.
The ranking scene, gameover scene, and main menu all require the use of the mouse.

CTRL + SHIFT + D: Enter Debug Scene (Map 0)

-- Gameplay Scene --
W: upward movement
S: downward movement
A: leftward movement
D: rightward movement

SPACE BAR/Left Mouse Click:
	- charge dash (via holding button) - arrow appears when dash is fully charged.
	- dash when charged (let go of button)
Mouse: aim player
F: time stop
	- requires a full charge, which is indicated by the number at the bottom left of the screen.
	- when it runs out (i.e. the bottom left timer counts down),

Up Arrow: move player light up
Down Arrow: move player light down
Left Arrow: move player light left
Right Arrow: move player light right
Page Up: raise player light
Page Down: lower player light
Period (.): reset player light to default position

P: pause game
R: restart level (if game is paused)
B: return to main menu (if game is paused)
Escape Key (Esc):
	- pause game (if not already paused)
	- exit game (if paused)


**********************************
CHERRY LIBRARY - GDW_Y2 - CRY LIB
**********************************
Library project. Originally this was going to be used to avoid copying files between projects, but the team decided to just copy files anyway.
This project still gets updated, but not nearly as frequently.