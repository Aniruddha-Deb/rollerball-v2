# Starter Code Overview

Rollerball's GUI is implemented in JavaScript, and the core engine is written in C++. A similar interface is used as Assignment2. Note that *for this assignment, we are only accepting solutions in C++*.

## Requirements

1. gcc >= 11
2. python >= 3.7

## Quickstart

```bash
git clone https://github.com/Aniruddha-Deb/rollerball-v2 && cd rollerball-v2
make rollerball
```

If all goes well, you should have an executable called `rollerball` in `bin`. To run the GUI, launch a web server from the `web` directory.

```bash
cd web
python3 -m http.server 8080
```

You can then open [localhost:8080](http://localhost:8080) on your browser to view the GUI. Here you can select one of the three board types. 

To launch the bots (assuming you're in the directory)

```bash
./bin/rollerball -p 8181
```

You can then connect the GUI to the bots. You would also need to start another bot for black on port 8182 to join and start the game.

## Web UI Changes

For this iteration, we have provided the source code for the Web UI as well. Those interested in developing/modifying this may do so. The UI is written in Vue, and contains a small README in the `websrc` directory that will help you in getting started. Note that **The TAs are not responsible for any bugs you may encounter while changing the UI code.** Posts on Piazza regarding questions about any files or modifications in `websrc` will not be answered.

![A screenshot of the UI in action (8_2 board)](figures/ui.png)

- The Web UI (and engine) now support **Starting new games without reloading the UI / restarting the engine(s)**.
- The UI also supports starting/stopping games and disconnecting/reconnecting from bots. 
- The clock in the UI runs at a resolution of 10 ms, and may not be as accurate as the clock we use when evaluating your engines. Please use this clock as a guideline, and not as a benchmark.
- The three board sizes can be changed using the buttons below the board, when a game is not in progress.

## C++ Binding changes

### Board:

- **All the methods in the Board class have been made public** for students to use.
- **Methods that modify Board state are suffixed with an underscore (_).** This means that `do_move` is now `do_move_`. Other methods modifying state have been made public and follow the same convention. You may need to change your code as a result of this. 
- **Copy constructors have been implemented for Board and BoardData**, and the `copy` method has been removed.
- Move generation has been generalized across multiple boards. This may lead to the generation being slightly slower than it was previously. You are advised to make changes taking this into account.
- BoardData and board utility methods have been split off into their own header files to clean up the code

### Engine:

- **engine.hpp can now be edited**, allowing students to store state across multiple invocations of their Engine. Note that this inherits from **AbstractEngine**, so you cannot remove or edit the signature of the `get_best_move` method.
- Due to managing time, the engine class now has a `time_left` field, which stores the time left (in ms). This is synced with the arbiter's clock whenever the arbiter asks the engine to find the best move. Note that a timeout on the arbiter's end implies that you lose, irrespective of the time on the clock you maintain.

### BoardData:

- Additional fields have been added for extra pieces, such as for knights and extra pawns, and the suffixes (`ws`, `bs`) have been renamed to `1`, `2`, `3`, `4`.
- A field called `board_type` indicates the type of the board: the type may be one of `SEVEN_THREE`, `EIGHT_FOUR` or `EIGHT_TWO` corresponding to the boards described above.  
- BoardData now has a corresponding board mask, indicating the squares on the board which are valid. These can be seen in `constants.hpp`, along with macros.
- BoardData also stores the pawn promotion squares in an array `pawn_promo_squares`. This is used for move generation.

### Other Changes:

- The `DEAD` constant has been changed from `pos(7,7)` to `0xff`
- Printing/debugging functions have been moved to `butils.hpp` 
- Move generation has been refactored completely, and should have fewer bugs now
- Documentation for methods can be found in the respective .hpp files

## Implementing the Engine

You would need to implement the `get_best_move` method in `engine.cpp`. This method will be called on an engine object when the server decides to search for a move, and it should do the following:

- Search for the best move given the board type, and store this best move in `best_move`.
- Terminate before the time specified in `time_left` runs out. 
- Not modify the board passed to it (Note that the board is declared `const`). You may make a copy of the board and modify that if needed.

The starter code's engine.cpp randomly picks a move from the moveset and sets the best move to it, as an example. This time, you are free to edit both `engine.cpp` and `engine.hpp`. The Engine class inherits from AbstractEngine class, and as long as you do not change the function signature of `get_best_move`, you are free to make edits in these two files.

## Other details

- You are **not allowed** to use any libraries other than those that come with the language. This means that libraries such as boost are not allowed.
- You are only allowed to modify and submit `engine.cpp` and `engine.hpp`  
- Your code must compile with the Makefile provided **without any modifications**.
- Your implemented algorithm should be **single-threaded**. You are not allowed to make use of multiple threads, asyncio or any other form of parallel execution.
- You are allowed to use any and all techniques subject to these constraints (including but not limited to value functions, tablebases, neural networks, MCTS, distillation, learning via Self-Play etc). Note that the assignment is competitive, and the TA bots are not necessarily the best benchmark to optimize against :)
