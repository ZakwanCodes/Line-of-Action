#include <iostream>
#include <vector>
#include <sstream>
#include <stack>
#include <fstream>
//Note by professor on bot automated move:
//For each piece, try all possible moves, find the biggest chain, return the move that retuns the biggest chain
//if we cannot find any move that can create the biggest chain, try all possible moves to reduce the distance from one member of the largest existing chain
using namespace std;

void printBoard(vector<vector<char>> board) {
	int printNums = 8;
	for (int i = 0; i < board.size(); i++) {
		if (i >= 0) {
			cout << printNums << " ";
			printNums--;
		}
		for (int j = 0; j < board.size(); j++) {
			cout << board[i][j] << " ";
		}
		cout << endl;
	}
	cout << "  a b c d e f g h" << endl;


}

bool checkRules(vector<vector<char>>& board, int currX, int currY, int toX, int toY) {
	int n = board.size();
	char piece = board[currX][currY];


	// if current piece is empty or the move from current piece to nextpiece is same return false
	if (piece == '.' || board[toX][toY] == piece) return false;

	// calculate change in x and y from next piece to current piece
	int dx = toX - currX;
	int dy = toY - currY;

	// return false if the change in x and y are not 0 i.e. can't move horizontal or vertical 
	//also return false if absolute value of dx != absolute value of dy i.e. can't move diagonal
	if (!(dx == 0 || dy == 0 || abs(dx) == abs(dy))) return false;

	// Determine step direction
	int sdx = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
	int sdy = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

	//Count pieces in the entire line 
	int countPieces = 0;
	if (dx == 0) { // horizontal
		for (int col = 0; col < n; col++) {
			if (board[currX][col] != '.') countPieces++;
		}
	}
	else if (dy == 0) { // vertical
		for (int row = 0; row < n; row++) {
			if (board[row][currY] != '.') countPieces++;
		}
	}
	else if (abs(dx) == abs(dy)) { // diagonal
		int xStep = (toX > currX) ? 1 : -1;
		int yStep = (toY > currY) ? 1 : -1;

		int x = currX;
		int y = currY;
		while (x >= 0 && x < n && y >= 0 && y < n) {
			if (board[x][y] != '.') countPieces++;
			x += xStep;
			y += yStep;
		}

		x = currX - xStep;
		y = currY - yStep;
		while (x >= 0 && x < n && y >= 0 && y < n) {
			if (board[x][y] != '.') countPieces++;
			x -= xStep;
			y -= yStep;
		}
	}

	int moveDistance = max(abs(dx), abs(dy));
	if (moveDistance != countPieces) return false;

	// Check intermediate squares 
	int x = currX + sdx;
	int y = currY + sdy;
	for (int i = 1; i < moveDistance; i++) {
		if (x == toX && y == toY) break; // stop at destination
		if (board[x][y] != '.' && board[x][y] != piece) return false; // cannot jump over opponent
		x += sdx;
		y += sdy;
	}

	return true; // move is valid
}

vector<vector<int>> boardToAdjList(vector<vector<char>> board, char color) {
	int n = board.size();                  
	int total = n * n;                     
	vector<vector<int>> adj(total);       

	// arrays to check all 8 directions around a piece
	int dxs[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int dys[] = { -1, 0, 1, -1, 1, -1, 0, 1 };


	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			// skip if piece is not player's color or if the piece is empty
			if (board[i][j] != color) continue;

			// calculate adjList key
			int u = i * n + j;

			// check all 8 neighboring directions around the current cell
			for (int k = 0; k < 8; k++) {
				int dx = i + dxs[k];  
				int dy = j + dys[k];  

				// skip if neighbor is out of bounds
				if (dx < 0 || dx >= n || dy < 0 || dy >= n) continue;

				//if the neighbor if found to be same piece, add an edge between them
				if (board[dx][dy] == color) {
					// calculate adjList value
					int v = dx * n + dy;
					adj[u].push_back(v);
				}
			}
		}
	}

	return adj;  // return the complete adjacency list
}

bool playerWon(vector<vector<char>>& board, char playerColor) {

	int n = board.size();

	// Count player's total pieces
	int mainPlayer = 0;
	for (int r = 0; r < n; r++) {
		for (int c = 0; c < n; c++) {
			if (board[r][c] == playerColor) {
				mainPlayer++;
			}
		}
	}

	if (mainPlayer == 0) return false;

	//Find start piece of player
	int start = -1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] == playerColor) {
				start = i * n + j;
				break;
			}
		}	}

	if (start == -1) return false;

	//build adjList
	vector<vector<int>> adj = boardToAdjList(board, playerColor);

	//run dfs on the adjList and keep track of count of verticies 
	vector<bool> visited(n * n, false);
	stack<int> st;

	st.push(start);
	visited[start] = true;

	int connectedCount = 0;

	while (!st.empty()) {
		int u = st.top();
		st.pop();
		connectedCount++;

		for (int v : adj[u]) {
			if (!visited[v]) {
				visited[v] = true;
				st.push(v);
			}
		}
	}
	//if total pieces in dfs search == player's total piece, every piece is connected and player wins
	return connectedCount == mainPlayer;
}

string parsePlayerMoveInput(string move) {
	char colChar = move[0];
	char rowChar = move[1];
	char colNum;
	char rowNum;

	if (move.size() != 2) {
		return "";
	}

	// Convert column
	if (colChar == 'b') colNum = '1';
	else if (colChar == 'c') colNum = '2';
	else if (colChar == 'd') colNum = '3';
	else if (colChar == 'e') colNum = '4';
	else if (colChar == 'f') colNum = '5';
	else if (colChar == 'g') colNum = '6';
	else if (colChar == 'a') colNum = '0';
	else if (colChar == 'h') colNum = '7';
	else {
		return "";
	}

	// Invert row: board[0] is top, row '1' is bottom
	int rowInt = '8' - rowChar; // '1'->7, '8'->0
	rowNum = '0' + rowInt;

	string parsed = "";
	parsed += rowNum;
	parsed += colNum;
	return parsed;
}

void humanTurn(vector<vector<char>>& board, char color) {
	cout << "Human turn: " << endl;
	string move, moveTo;
	int r0, c0, r1, c1;

	//Ask which piece to move
	cout << "What piece would you like to move? ";
	cin >> move;



	string fromParsed = parsePlayerMoveInput(move);
	if (fromParsed == "" ) {
		cout << "Invalid input, try again." << endl;
		humanTurn(board, color);
		return;
	}

	// Extract row and column from parsed string (format: "rowcol", e.g., "52")
	r0 = fromParsed[0] - '0'; // first char = row
	c0 = fromParsed[1] - '0'; // second char = column
	
	if (board[r0][c0] != color) {
		cout << "You must move your own piece!" << endl;
		humanTurn(board, color);
		return;
	}
	// Step 2: ask where to move
	cout << "Where would you like to move to? ";
	cin >> moveTo;
	string toParsed = parsePlayerMoveInput(moveTo);
	if (toParsed == "") {
		cout << "Invalid input, try again." << endl;
		humanTurn(board, color);
		return;
	}

	r1 = toParsed[0] - '0';
	c1 = toParsed[1] - '0';

	// Step 3: check if move is legal
	if (checkRules(board, r0, c0, r1, c1)) {
		board[r1][c1] = board[r0][c0]; // move piece
		board[r0][c0] = '.';           // empty original square
	}
	else {
		cout << "Invalid move, try again." << endl;
		humanTurn(board, color); // retry
		return;
	}

	printBoard(board);
}

int getAllLegalMoves(vector<vector<char>>& board, char color, vector<int>& currX, vector<int>& currY, vector<int>& toX, vector<int>& toY) {
	int n = board.size();
	int count = 0;

	//i,j = coordinates of current piece
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {

			//skip other player's pieces and empty space
			if (board[i][j] != color) continue;

			//k,l = coordinates of piece where the current piece can legally make a move
			for (int k = 0; k < n; k++) {
				for (int l = 0; l < n; l++) {

					//can't land on same spot
					if (i == k && j == l) continue;

					//if move is illegal, simply move onto next space on the board
					if (!checkRules(board, i, j, k, l)) continue;

					//by now, the move on k,l coordinate piece is legal, we've found a possible move so add it to the vectors
					currX.push_back(i);
					currY.push_back(j);
					toX.push_back(k);
					toY.push_back(l);
					count++;
				}
			}
		}
	}
	return count;  // number of legal moves
}

int biggestChain(vector<vector<char>>& board, char color) {
	//run dfs on each piece to find the chain size of that piece and update biggestChainSize as we loop through

	int n = board.size();
	vector<vector<int>> adj = boardToAdjList(board, color);
	vector<bool> visited(n * n, false);
	int biggestChainCount = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] != color) continue;
			int u = i * n + j;
			if (visited[u]) continue;

			stack<int> st;
			st.push(u);
			visited[u] = true;
			int currChainCount = 0;

			while (!st.empty()) {
				int v = st.top();
				st.pop();
				currChainCount++;
				for (int w : adj[v]) {
					if (!visited[w]) {
						visited[w] = true;
						st.push(w);
					}
				}
			}

			if (currChainCount > biggestChainCount) {
				biggestChainCount = currChainCount;
			}
		}
	}

	return biggestChainCount;
}

int playerPieceDistanceSum(vector<vector<char>>& board, char color) {
	int n = board.size();
	vector<int> x, y;
	//add the coordinates of every player piece into two vectors x,y
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (board[i][j] == color) {
				x.push_back(i);
				y.push_back(j);
			}
		}
	}

	int distanceSum = 0;
	int rowsSize = x.size();
	//find the distance between each piece and keeping adding it to the distance sum
	for (int i = 0; i < rowsSize; i++) {
		for (int j = i + 1; j < rowsSize; j++) {
			distanceSum += abs(x[i] - x[j]) + abs(y[i] - y[j]);
		}
	}
	return distanceSum;
}

string parseBotMove(int r, int c) {
	// Convert column number to letter
	char colChar = 'a' + c;

	// Convert row number to board notation (bottom row = 1)
	char rowChar = '8' - r;

	string move = "";
	move += colChar;
	move += rowChar;
	return move;
}

void botTurn(vector<vector<char>>& board, char color) {
	cout << "Player 2 (Bot) turn..." << endl;
	
	int n = board.size();
	vector<int> currX, currY, toX, toY;
	
	//all possible legal moves are stored in curr (x,y) and to (x,y)
	getAllLegalMoves(board, color, currX, currY, toX, toY);

	if (currX.size() == 0) {
		cout << "Bot has no legal moves!" << endl;
		return;
	}

	//now, we need to find the best possible move
	int bestIndex = 0;
	int bestChain = -1;
	int originalLargest = biggestChain(board, color);

	//Find the move that increases the chain the most
	for (int i = 0; i < currX.size(); i++) {
		//temporarily make a move 
		char saved = board[toX[i]][toY[i]];
		board[toX[i]][toY[i]] = board[currX[i]][currY[i]];
		board[currX[i]][currY[i]] = '.';
		
		//store the chain size after the temp move
		int chainSize = biggestChain(board, color);

		//update the index and chain size if a bigger chain has been found
		if (chainSize > bestChain) {
			bestChain = chainSize;
			bestIndex = i;
		}

		//revert the move back to original board and repeat with all other moves
		board[currX[i]][currY[i]] = color;
		board[toX[i]][toY[i]] = saved;
	}

	//If no moves increase the chain size, then we have to pick a move that reduces distance between the unconnected chains / pieces
	if (bestChain <= originalLargest) {

		//temporarily make a move
		char saved = board[toX[0]][toY[0]];
		board[toX[0]][toY[0]] = board[currX[0]][currY[0]];
		board[currX[0]][currY[0]] = '.';

		//initialize the distance with the first move
		int bestDist = playerPieceDistanceSum(board, color);

		//revert back the mpve
		board[currX[0]][currY[0]] = color;
		board[toX[0]][toY[0]] = saved;
		bestIndex = 0;

		//now we can compare the distance size of all moves except the first one which is already initialized 
		for (int i = 1; i < currX.size(); i++) {
			//temporarily make a move again
			saved = board[toX[i]][toY[i]];
			board[toX[i]][toY[i]] = board[currX[i]][currY[i]];
			board[currX[i]][currY[i]] = '.';

			int dist = playerPieceDistanceSum(board, color);

			//if the distance is less, then we found a better move
			if (dist < bestDist) {
				bestDist = dist;
				bestIndex = i;
			}

			//revert back the move
			board[currX[i]][currY[i]] = color;
			board[toX[i]][toY[i]] = saved;
		}
	}

	//now, the best play has been found and we can make the move
	board[toX[bestIndex]][toY[bestIndex]] = board[currX[bestIndex]][currY[bestIndex]];
	board[currX[bestIndex]][currY[bestIndex]] = '.';

	cout << "Bot moves from " << parseBotMove(currX[bestIndex], currY[bestIndex]) << " to " << parseBotMove(toX[bestIndex], toY[bestIndex]) << endl;

	printBoard(board);
}

void generateMidGameBoard(vector<vector<char>>& board) {
	ifstream file("midgameboard.txt");
	if (!file.is_open()) {
		cout << "Error: Could not open mid-game board file!" << endl;
		return;
	}

	string line;
	int row = 0;
	while (getline(file, line) && row < board.size()) {
		int col = 0;
		for (int i = 0; i < line.size() && col < board[row].size(); i++) {
			char piece = line[i];
			if (piece == ' ') continue; // skip spaces
			if (piece == 'x') board[row][col] = '.'; // empty square
			else if (piece == 'b') board[row][col] = 'B';
			else if (piece == 'w') board[row][col] = 'W';
			else board[row][col] = '.'; // default to empty
			col++;
		}
		row++;
	}

	file.close();
}

void generateBoard(vector<vector<char>>& board) {
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.size(); j++) {
			if ((i == 0 || i == board.size() - 1) && (j > 0 && j < board.size() - 1)) {
				board[i][j] = 'B';
			}
			else if ((i > 0 && i < board.size() - 1) && (j == 0 || j == board.size() - 1)) {
				board[i][j] = 'W';
			}
			else {
				board[i][j] = '.';
			}
		}
	}
}

int playGame(vector<vector<char>>& board) {
	int player;
	int playerColor;
	int boardType;
	cout << "Would you like to (1) start a fresh board or (2) mid-game-board? ";
	cin >> boardType;

	while (boardType != 1 && boardType != 2) {
		cout << "Invalid option. Please enter 1 or 2: ";
		cin >> boardType;
	}

	if (boardType == 1) {
		generateBoard(board);
		printBoard(board);
	}
	else { 
		generateMidGameBoard(board);
		printBoard(board);
	}

	do {
		cout << "First player: human (1) or bot (2)? ";
		cin >> player;
		cout << "Color: black (1) or white (2)? ";
		cin >> playerColor;
		if (((player != 1 && player != 2) || (playerColor != 1 && playerColor != 2))) {
			cout << "Invalid option, please enter the correct option" << endl;
		}
	} while ((player != 1 && player != 2) || (playerColor != 1 && playerColor != 2));

	char humanColor, botColor;
	if (player == 1){
		if (playerColor == 1){
			humanColor = 'B'; 
			botColor = 'W'; 
		} else {
			humanColor = 'W'; 
			botColor = 'B'; } 
	}
	else if(player == 2){ 
		if (playerColor == 1){
			botColor = 'B'; 
			humanColor = 'W'; 
		} else {
			botColor = 'W'; humanColor = 'B'; 
		}
	}

	while (true) {
		if (player == 1) {
			humanTurn(board, humanColor);
			if (playerWon(board, humanColor) && playerWon(board, botColor)) {
				cout << "Tie game!" << endl; 
				return 0; //0 = tie game
			}
			else if (playerWon(board, humanColor)) {
				cout << "Human wins!" << endl;
				return 1; //1 = human wins
			}
			player = 2; 
		}
		else if(player == 2){
			botTurn(board, botColor);
			if (playerWon(board, humanColor) && playerWon(board, botColor)) {
				cout << "Tie game!" << endl; //0 = tie game
				return 0;
			}
			else if (playerWon(board, botColor)) {
				cout << "Bot wins!" << endl; //2 = bot wins
				return 2;
			}

			player = 1;
		}
	}
}


int main() {

	vector<vector<char>> board(8, vector<char>(8));

	cout << "          -----Lines of Action Tournament-----" << endl;
	int keepPlaying;
	int humanWins = 0;
	int botWins = 0;

	do {
		int game = playGame(board);
		if (game == 1) {
			humanWins++;
		}
		else if (game == 2) {
			botWins++;
		}
		else if (game == 0) { 
			humanWins++; 
			botWins++; 
		}

		cout << "Would you like to play another round? 1 (yes) or 2 (no) ";
		cin >> keepPlaying;
	} while (keepPlaying == 1);


	if (humanWins == botWins) {
		cout << endl << endl << "Tie tournament!" << endl;
	}
	else if (humanWins > botWins) {
		cout << "Human wins the tournament!" << endl;
	}
	else {
		cout << "Bot wins the tournament!" << endl;
	}

	cout << "final score:" << endl;
	cout << "human: " << humanWins << endl;
	cout << "bot: " << botWins << endl;

	return 0;
}