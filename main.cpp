
		#include <iostream>
		#include <iomanip>
		#include <cstdlib>
		#include <vector>
		#include <algorithm>
		#include <cassert>
		using namespace std;

		struct MyGameDef { // professional way to define global enumerable values.
			enum Player {
			Human, Progm, Empty
			};
			Player m_player;
			MyGameDef(const Player p) :
					m_player(p) {
			}
			operator Player() const {
				return m_player;
			}
		};

		class Cell {       // representing a cell of a board.
			MyGameDef::Player m_occu; // marked 'Empty', 'Human', and 'Progm'.
		public:
			Cell() :
					m_occu(MyGameDef::Empty) {
			}
			void setTurn(const MyGameDef::Player p) {
				assert(!isOccupied());
				m_occu = p;
			}
			bool isOccupied() const {
				return m_occu != MyGameDef::Empty;
			}
			bool isMe(const MyGameDef::Player p) const {
				return m_occu == p;
			}
			void erase() {
				m_occu = MyGameDef::Empty;
			}
			friend ostream& operator<<(ostream& o, const Cell& c) {
				char mark;
				switch (c.m_occu) {
				case MyGameDef::Empty:
					return o << " . ";
				case MyGameDef::Human:
					return o << " o ";
				case MyGameDef::Progm:
					return o << " x ";
				default:
					abort();
				}
			}
		};

		class Eval {
			int m_conn; // connected to me.
			int m_poss; // possible spaces.
		public:
			Eval(const int c = 0, const int p = 0) :
					m_conn(c), m_poss(p) {
			}
			bool isConnect4() const {
				cout << m_conn << " Connected" << endl;
				cout << m_poss << " Possible" << endl;
				return m_conn >= 3;
			}
			bool isWinning() const {
				return m_conn == 2 && m_poss >= 1;
			}
			int getValue() const {
				return m_conn * 2 + m_poss;
			}
			friend ostream& operator<<(ostream& o, const Eval& e) {
				return o << "(" << e.m_conn << ", " << e.m_poss << ")";
			}
		};

		class Turn {
			char m_turn;
			int m_score;
		public:
			Turn(const char c = '\0', const int s = 0) :
					m_turn(c), m_score(s) {
			}
			char getTurn() const {
				return m_turn;
			}
			int getScore() const {
				return m_score;
			}
			friend bool operator<(const Turn& lhs, const Turn& rhs) {
				return lhs.m_score < rhs.m_score;
			}
			friend ostream& operator<<(ostream& o, const Turn& t) {
				return o << "--> (" << t.m_turn << ") with score " << t.m_score;
			}
		};

		class Board {       // represents Connect Four board.
			int m_row;
			int m_col;
			const int m_max;  // used to detect tie.
			int m_num;        // the number of the marks placed.
			vector<vector<Cell> > m_map;
			bool validCoord(int r, int c) const{
				return c >= 0 && c < m_col && r >= 0 && r < m_row;
			}
			Eval makeEval(const int r, const int c, const MyGameDef::Player p, int dy, int dx) const{
				int count = 0, possible = 0;
				bool connected  = true;
				for(int col = c + dx, row = r + dy, saw = 0; validCoord(row,col) && saw < 3; col += dx, row += dy, saw++){
					if(m_map[row][col].isMe(p)){
						if(++count > 2){
							if(connected)
								break;
						}
						continue;
					}
					connected = false;
					if(m_map[row][col].isOccupied())
						break;
					++possible;
				}
				for(int col = c - dx, row = r - dy, saw = 0; validCoord(row,col) && saw < 3; col -= dx, row -= dy, saw++){
					if(m_map[row][col].isMe(p)){
						if(++count > 2){
							if(connected)
								break;
						}
						continue;

					}
					connected = false;
					if(m_map[row][col].isOccupied())
						break;
					++possible;
				}
				cout << count << " Counted" << endl;
				return Eval(count,possible);
			}
			Eval vertical(const int r, const int c, const MyGameDef::Player p) const {
				return makeEval(r,c,p,1,0);
			}
			Eval horizontal(const int r, const int c, const MyGameDef::Player p) const {
				int count = 0, possible = 0;
				bool connected = true;
				for (int col = c + 1, saw = 0; col < m_col && saw < 3; ++col, ++saw) { // east direction.
					if (m_map[r][col].isMe(p)) {
						cout << count << endl;
						if (++count > 2) {
							if (connected)
								break;
						}
						continue;
					}
					connected = false;
					if (m_map[r][col].isOccupied())
						break;
					++possible;
				}
				for (int col = c - 1, saw = 0; col >= 0 && saw < 3; --col, ++saw) { // west direction.
					if (m_map[r][col].isMe(p)) {
						cout << count << endl;
						if (++count > 2) {
							if (connected)
							break;
						}
						continue;
					}
					connected = false;
					if (m_map[r][col].isOccupied())
						break;
					++possible;
				}
				return Eval(count, possible);
			}
			Eval leftup(const int r, const int c, const MyGameDef::Player p) const {
				return makeEval(r,c,p,-1,-1);
			}
			Eval rightup(const int r, const int c, const MyGameDef::Player p) const {
				return makeEval(r,c,p,-1,1);
			}
			void bestMoves(vector<Turn>& moves, const MyGameDef::Player p, const bool show = false){
				for(int col = 0; col < m_col; col++){
					bool valid = false;
					int topRow;
					for (int row = m_row - 1; row >= 0; --row) {
						if (m_map[row][col].isOccupied())
							continue;
						valid = true;
						topRow = row;
						break;
					}
					string player;
					if(p == MyGameDef::Human){
						player = "you";
					}
					else{
						player = "machine";
					}
					int score;
					if(!valid){
						if(show){
							string name = player;
							name[0] = toupper(name[0]);
							cout << name << " cannot choose (" << (char)('a' + col) << ") - " << endl;
						}
						Turn t((char)('a' + col), -1);
						moves.push_back(t);
						continue;
					}
					Eval V = vertical(topRow, col, p);   // check verticcal line.
					Eval H = horizontal(topRow, col, p); // check horizontal line.
					Eval L = leftup(topRow, col, p);     // check leftup diagnal line.
					Eval R = rightup(topRow, col, p);    // check rightup diagnal line.
					score = V.getValue() + H.getValue() + L.getValue() + R.getValue();
					if(show){
						cout << "If " << player << " chose (" << (char)('a' + col) << ") - ";
						cout << "H " << H << ", V " << V << ", L " << L << ", " << ", R " << R;
						cout << ", total score " << score << endl;
					}
					Turn t((char)('a' + col), score);
					moves.push_back(t);
				}
				sort(moves.begin(),moves.end());
			}

		public:
			Board(const int r = 6, const int c = 7) :
					m_row(r), m_col(c), m_max(r * c), m_num(0) {
				vector<Cell> arow(m_col, Cell());
				for (int i = 0; i < m_row; ++i)
					m_map.push_back(arow);
			}
			bool isTie() const {
				return m_num >= m_max;
			}
			bool isValid(const char c, const MyGameDef::Player p) {
				int col = c - 'a';
				if (col < 0 || col >= m_col)
					return false;
				bool placed = false;
				for (int row = m_row - 1; row >= 0; --row) {
					if (m_map[row][col].isOccupied())
						continue;

					placed = true;
					++m_num;
					m_map[row][col].setTurn(p);
					break;
				}
				return placed;
			}
			bool isWon(const char c, const MyGameDef::Player p) const {
				int col = c - 'a', row = 0;
				for (; row < m_row; ++row) {
					if (!m_map[row][col].isOccupied())
						continue;

					assert(m_map[row][col].isMe(p));
					break;
				}
				Eval V = vertical(row, col, p);   // check vertical line.
				Eval H = horizontal(row, col, p); // check horizontal line.
				Eval L = leftup(row, col, p);     // check leftup diagnal line.
				Eval R = rightup(row, col, p);    // check rightup diagnal line.
				return V.isConnect4() || H.isConnect4() || L.isConnect4()
						|| R.isConnect4();
			}
			char findBestMove(const MyGameDef::Player p, const bool show = false) {
				vector<Turn> moves;
				bestMoves(moves,p,show);
				cout << "After sorted..." << endl;
				for(int i = 0; i < moves.size(); i++){
					cout << moves[i] << endl;
				}
				MyGameDef::Player opp;
				string playerposs;
				string opponposs;
				if(p == MyGameDef::Human){
					opp = MyGameDef::Progm;
					playerposs = "your";
					opponposs = "machine's";
				}
				else{
					opp = MyGameDef::Human;
					playerposs = "machine's";
					opponposs = "your";
				}
				// Countermoves
				vector<Turn> countermoves;
				bestMoves(countermoves, opp,show);
				cout << "after sorted..." << endl;
				for(int i = 0; i < moves.size(); i++){
					cout << moves[i] << endl;
				}
				int lastmove = moves.size() - 1;
				int lastcm = countermoves.size() - 1;
				cout << ">>> " << playerposs << " best score is " << moves[lastmove].getScore();
				cout << " by taking a turn (" << moves[lastmove].getTurn() << "), ";
				cout << "while " << opponposs << " best score is " << countermoves[lastcm].getScore();
				cout << " by taking a turn (" << countermoves[lastcm].getTurn() << ")..." << endl;
				if(moves[lastmove].getScore() >= 110){
					return moves[lastmove].getTurn();
				}
				for(int i = lastcm; i >= 0; i--){
					if(countermoves[i].getScore() >= 90){
						return countermoves[i].getTurn();
					}
					else{
						break;
					}
				}
				return moves[moves.size() - 1].getTurn();
			}
			void erase(const char c, const MyGameDef::Player p) {
				int col = c - 'a';
				for (int row = 0; row < m_row; ++row) {
					if (!m_map[row][col].isOccupied())
						continue;			assert(m_map[row][col].isMe(p));
					m_map[row][col].erase();
					break;
				}
				--m_num;
			}
			friend ostream& operator<<(ostream& o, const Board& m) {
				o << endl;
				for (int col = 0; col < m.m_col; ++col)
					o << setw(3) << (char) ('a' + col) << ' ';
				o << endl;
				for (int row = 0; row < m.m_row; ++row) {
					o << '|';
					for (int col = 0; col < m.m_col; ++col)
						o << setw(3) << m.m_map[row][col] << '|';
					o << endl;
				}
				o << endl;
				return o;
			}
		};
		int main() {
			Board b(6, 7); // traditional size Connect Four game.
			while (!b.isTie()) {
				cout << b;   // showing board status.
				cout << "Enter your turn: ";
				char turn;
				cin >> turn;
				if (turn == '?') {
					b.findBestMove(MyGameDef::Human, true);
					continue;
				}

				while (!b.isValid(tolower(turn), MyGameDef::Human)) {
					cout << "Invalid input, try again: ";
					cin >> turn;
				}
				if (b.isWon(turn, MyGameDef::Human)) {
					cout << "You won :)";
					break;
				}
				turn = b.findBestMove(MyGameDef::Progm, true);
				if (!b.isValid(tolower(turn), MyGameDef::Progm)) {
					cout << "Serious error occurred, program cannot continue..."
							<< endl;
					abort();
				}
				if (b.isWon(turn, MyGameDef::Progm)) {
					cout << "You lost :(";
					break;
				}
			}
			if (b.isTie())
				cout << "Tied..." << endl;
			cout << " Final board is" << endl << b << "Bye..." << endl;
			return 0;
		}

