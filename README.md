# CS 120b Final Project: Tetris Game
I use a github library from a user on the internet
https://github.com/LittleBuster/avr-nokia5110.

##Usage 

- Block Line - Creates a Tetris struct object with its variables 
    Has Helper Functions and Utilities.
- Grid - Creates a Grid for Nokia 5110 LCD
    Has Constructor/Checking for Pixels
- Nokia 5110 - Nokia 5110 LCD file
    The Components of Nokia 5110 library
- Main - 6 state machines
1. StartON_Tick - Check if user has pressed start
2. GameON_Tick - Initiate Dropping Tetris from the Grid
3. Rotate_Tick - Check if user had wanted to rotate the block
4. Movement_Tick - Check if user wanted to move left or move right
5. CheckLine_Tick - Check if the tetris block grid is full
6. AttachLine- Tick - Use of the drop block

##Contribution
https://github.com/LittleBuster/avr-nokia5110.
