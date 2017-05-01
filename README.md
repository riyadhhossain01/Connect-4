# Connect-4
#Connect Four
The mission of this assignment is to realize a traditional Connect Four game. You are to deliver the program that will facilitate a game play with a computer logic that can respond to you at the level of the average "adult-level" strength. A completion of this assignment needs your thorough understanding of 2-dimensional vector treatment as well as clear understanding of the explanation described here. It is not possible to realize this assignment in just one-weekend rush work -- start early.

First of all, download the template implementation of this game attached at the bottom of this page. Compile and play with it. Note that this code is very incomplete although it has all the required input and output interface. It has no intelligence at all. Below shows three sessions taken from my implemented application. The game starts with your turn (marked 'o') and the program responds by showing its turn (marked 'x'). Win/Lose/Tie will be shown at the end of the game. Also, typing a hint key ('?') will tell you a promising move for your next turn.

``` 
  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | x | . | o | . |
Enter your turn: b
         	

  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | x | . | . | . |
| . | x | . | o | . | x | . |
| . | o | x | x | . | o | . |
| . | o | o | x | o | o | x |
Enter your turn: f
         	
You lost :( Final board is
  a   b   c   d   e   f   g 
| . | . | x | o | x | x | x |
| . | . | x | o | x | o | o |
| x | o | x | x | o | o | o |
| o | x | o | o | x | x | x |
| o | o | x | x | x | o | o |
| x | o | o | x | o | o | x |
Bye...
```
Your first task is to add the logic to check the status of the game, i.e., which player is winning. Note that unlike the tic-tac-toe game demonstrated in class, finding out the presence of "connect four" on all board positions is extremely inefficient. Instead, the check mechanism must start at the point in which the mark is placed. For instance, to check the horizontal direction, the logic needs to inspect the right (eastbound) direction and the left (westbound) direction to count the "consecutive" marks to be found along the way walking through the board location. The posted template has already implemented a board structure with a traditional size (6 x 7) 2-dimensional cells. Study the already implemented horizontal direction check and extend it to realize for other directions. This is not difficult but take extra care to manipulate "row" and "column" indexes. They are very fragile and causing strange errors. 

You can also find a logic to "count" a "value" of the position you visited, which will be utilized for the development of the intelligence in your logic. The heuristics approach applied here (Akira's version) is the next computation:

The place to be marked will have a unit score (1 point) for every empty position reachable from that position; up to 4 positions apart from that position are considered if no opponent is blocking the direction.
The same mark will have a double score (2 points); having three same marks and one empty space is considered to be a winning position and 50 points are added; having four consecutive same marks is "connect four" and 100 points are added,
The total score for all the four (horizontal, vertical, and two diagonal) directions originated at a chosen position is considered as a "score" of the turn.
For instance, look at the next example and understand how the computation is applied:
```
  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | x | . | . | . | . | . |
| . | o | . | x | . | . | . |
| . | o | . | o | x | . | . |

Enter your turn: f
              	

If machine chose (a) - H (0, 0), V (0, 3), L (0, 0), R (0, 0), total score 5
If machine chose (b) - H (0, 4), V (1, 2), L (2, 2), R (0, 3), total score 69
If machine chose (c) - H (0, 0), V (0, 3), L (0, 0), R (1, 2), total score 9
If machine chose (d) - H (1, 5), V (1, 3), L (0, 4), R (0, 4), total score 22
If machine chose (e) - H (1, 3), V (1, 3), L (0, 3), R (0, 2), total score 17
If machine chose (f) - H (1, 3), V (0, 3), L (0, 4), R (1, 1), total score 17
If machine chose (g) - H (0, 0), V (0, 3), L (0, 3), R (0, 0), total score 8

```
The computation occurs for the machine's turn of 'x' after human placed 'o' to the column (f).  If the machine chooses the column (a), there is no other mark of 'x' found in any direction and s	o the left of the pair of H(orizontal), V(ertical), L(eftup), and R(ightup) directions have the value 0. There are 3 positions empty above the mark of 'x' at the bottom row. Therefore, the score is (number of 'x' marks) 1 x 2 + (number of reachable empty positions) 3 = 5. If the machine chooses the column (b), the computation is 4 x 2 + 11 + 50 = 69. Notice that the extra 50 points are added because 3 'x' marks are found in Leftup-diagonal in columns (b), (d), (e). The report above shows the number of the same mark one less than the actual count observed.

You need to compute each possible turn among the choices (a) through (g) based on this defined rule. Place the turn to the board, compute scores, and then erase the placed turn using an "erase" method already implemented. The intention is to select the turn with the highest score computed. However, choosing the identified turn may be too hasty because that choice for you (or machine) may not be damaging the opponent but rather contributing to the winning position of the opponent. The right way is to apply the "simulated" motion of each turn the machine can make, followed by the same simulated motion at the human-side to evaluate how the human-side would be losing based on the same computation. The iterative round of this simulated implementation is the so-called "Minimax with alpha-beta pruning method" in the Artificial Intelligence Course and this is beyond this course's material. 

A simplified method is applied here. After your (human) turn, the machine will inspect all the possible turns by placing 'x' move to the board and select the one with the highest score. The board is reset. In the second round, the machine applies the human turn by placing 'o' move to the board in the same way and selects the best turn. This means that the machine is allowing you (human) to play in two consecutive rounds (double play). In this way, the machine can find out a devastating turn missed from its own computation of turns, in which case by preoccupying that endangered position by placing 'x' (thus not allowing to place 'o') the machine can avoid immediate lose and continue the game. This is not a deep-thinker player, and therefore the machine may easily lose (as my 7-year-old son proved below).

The same principle applies to a hint processing ('?' input). The machine will compute the best score by placing 'o' to the board, unset it, and then applies its double play by placing 'x' to find out its losing motion. The next hint sessions show the case you can win (courtesy of my son).

```
  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | x | . | . | . |
| . | . | . | x | . | o | . |
| . | x | . | o | o | x | o |

Enter your turn: ?
If you chose (a) - H (0, 0), V (0, 3), L (0, 0), R (0, 3), total score 8
If you chose (b) - H (0, 2), V (0, 3), L (0, 2), R (0, 4), total score 13
If you chose (c) - H (2, 0), V (0, 3), L (0, 2), R (0, 0), total score 11
If you chose (d) - H (0, 6), V (0, 2), L (2, 3), R (0, 5), total score 72
If you chose (e) - H (1, 1), V (1, 3), L (0, 0), R (1, 2), total score 14
If you chose (f) - H (0, 2), V (1, 3), L (0, 4), R (1, 2), total score 17
If you chose (g) - H (1, 1), V (1, 3), L (0, 3), R (0, 0), total score 13
After sorted...        
--> (a) with score 8        
--> (c) with score 11        
--> (b) with score 13        
--> (g) with score 13        
--> (e) with score 14        
--> (f) with score 17        
--> (d) with score 72
If machine chose (a) - H (1, 1), V (0, 3), L (0, 0), R (0, 3), total score 11
If machine chose (b) - H (1, 3), V (1, 3), L (0, 2), R (0, 4), total score 18
If machine chose (c) - H (1, 1), V (0, 3), L (0, 2), R (1, 2), total score 14
If machine chose (d) - H (0, 6), V (2, 2), L (0, 3), R (0, 5), total score 72
If machine chose (e) - H (1, 2), V (0, 3), L (2, 2), R (0, 2), total score 67
If machine chose (f) - H (1, 3), V (0, 3), L (0, 4), R (0, 2), total score 16
If machine chose (g) - H (0, 0), V (0, 3), L (0, 3), R (1, 0), total score 10
After sorted...        
--> (g) with score 10        
--> (a) with score 11        
--> (c) with score 14        
--> (f) with score 16        
--> (b) with score 18        
--> (e) with score 67        
--> (d) with score 72
>>> your best score is 72 by taking a turn (d), while machine's best score is 72 by taking a turn (d)...

  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | x | . | . | . |
| . | . | . | x | . | o | . |
| . | x | . | o | o | x | o |

Enter your turn: d

  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | o | . | . | . |
| . | . | . | x | . | . | . |
| . | . | . | x | x | o | . |
| . | x | . | o | o | x | o |
Enter your turn: ?
If you chose (a) - H (0, 0), V (0, 3), L (0, 0), R (1, 2), total score 9
If you chose (b) - H (0, 2), V (0, 3), L (0, 2), R (1, 3), total score 14
If you chose (c) - H (2, 0), V (0, 3), L (0, 2), R (0, 0), total score 11
If you chose (d) - H (0, 6), V (1, 1), L (0, 4), R (0, 4), total score 19
If you chose (e) - H (0, 2), V (0, 3), L (3, 0), R (0, 2), total score 115
If you chose (f) - H (0, 2), V (1, 3), L (0, 4), R (0, 1), total score 14
If you chose (g) - H (1, 0), V (1, 3), L (0, 3), R (0, 0), total score 12
After sorted...        
--> (a) with score 9        
--> (c) with score 11       
--> (g) with score 12        
--> (b) with score 14        
--> (f) with score 14        
--> (d) with score 19        
--> (e) with score 115
If machine chose (a) - H (1, 1), V (0, 3), L (0, 0), R (0, 2), total score 10
If machine chose (b) - H (2, 2), V (1, 3), L (0, 2), R (0, 2), total score 67
If machine chose (c) - H (1, 1), V (0, 3), L (0, 2), R (1, 2), total score 14
If machine chose (d) - H (0, 6), V (0, 1), L (0, 4), R (0, 4), total score 17
If machine chose (e) - H (1, 4), V (1, 3), L (0, 0), R (1, 3), total score 18
If machine chose (f) - H (1, 3), V (0, 3), L (0, 4), R (1, 1), total score 17
If machine chose (g) - H (0, 0), V (0, 3), L (0, 3), R (1, 0), total score 10
After sorted...
After sorted...        
--> (a) with score 10        
--> (g) with score 10        
--> (c) with score 14        
--> (d) with score 17        
--> (f) with score 17        
--> (e) with score 18        
--> (b) with score 67
>>> your best score is 115 by taking a turn (e), while machine's best score is 67 by taking a turn (b)...

  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | o | . | . | . |
| . | . | . | x | . | . | . |
| . | . | . | x | x | o | . |
| . | x | . | o | o | x | o |
Enter your turn: e

You won :) Final board is
  a   b   c   d   e   f   g 
| . | . | . | . | . | . | . |
| . | . | . | . | . | . | . |
| . | . | . | o | . | . | . |
| . | . | . | x | o | . | . |
| . | . | . | x | x | o | . |
| . | x | . | o | o | x | o |
Bye...

```
