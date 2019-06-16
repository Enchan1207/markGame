# markGame

## OverView
variable-sized Gomoku game.

## Development Environment
 * Language: C
 * Compiler: clang 4.2.1

## Contents
 * main.c: game loop
 * func.c: functions
 * Betaengine.c: CPU engine(beta)
 * field.h: field size definition

## Usage
### Compile
Run `gcc *.c` or `clang *.c` to compile.
(The order of compilation does not matter)

### Play
 1. Deciding who to put the stone first (randomly).
 2. In CPU turn, the engine decide where to put stone.
 3. In your turn, input position to put stone (like: 0,4 ).
 4. Repeat 2 ~ 3.
 5. When there is no place to put, or when the stones is vertically, horizontally or diagonally aligned, the game finish.

### Change field size
In default definition, the field size is set 5x5.  
To change this, change the `WIDTH` definition in `field.h`.  
(CPU engine is developed as play with 5x5 field, so If you change field size too large, the engine will be poor.)  
In Recent version, you can't input value over 2 digit.  

## License
This software is released under the MIT License, see LICENSE.