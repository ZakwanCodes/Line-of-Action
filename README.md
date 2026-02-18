##Line of Action – C++ Implementation with AI

#Display Recommendation
⚠️ For best experience, use a larger terminal window.

#Overview
This project is a complete console-based implementation of the board game Line of Action, written in C++.

**The game supports:**
- Human vs Bot gameplay
- Full rule validation
- AI move decision logic
- Win detection using graph traversal
- Tournament-style multi-round play

The bot uses heuristic-based decision making to evaluate and select optimal moves.

**Game Rules (Line of Action)**
- Line of Action is a two-player strategy board game played on an 8×8 board.
- Each player starts with pieces arranged along opposite edges.
- A piece moves horizontally, vertically, or diagonally.
- The distance a piece moves must equal the total number of pieces (both colors) on that line.
- Players may jump over their own pieces but not over opponent pieces.
- The objective is to connect all of your pieces into a single connected group.
- A player wins when all their pieces form one connected component.

#Features

**Full Rule Enforcement**
- Validates horizontal, vertical, and diagonal moves.
- Ensures movement distance equals the number of pieces in that direction.
- Prevents illegal jumps over opponent pieces.
- Prevents landing on own pieces.

**Graph-Based Win Detection**
- Converts the board into an adjacency list representation.
- Uses Depth-First Search (DFS) to detect connected components.
- Declares victory when all player pieces are connected.

**AI Opponent**
- The bot uses a two-stage heuristic:
- Maximize Largest Connected Component
- Simulates all legal moves.
- Selects the move that produces the largest connected chain of pieces.
- Minimize Piece Distance (Fallback Strategy)
- If no move increases chain size,
- Selects the move that minimizes total Manhattan distance between pieces.
- The bot evaluates moves by temporarily simulating them and rolling back the board state safely.

**Additional Features**
- Fresh board generation
- Mid-game board loading from file (midgameboard.txt)
- Tournament mode with score tracking
- Multiple rounds of play

#How to Compile:

Using g++:
g++ main.cpp -o lineofaction

Then run:
./lineofaction
