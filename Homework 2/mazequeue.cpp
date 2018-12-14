#include <string>
#include <queue>
#include <iostream>

using namespace std;

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

class Coord
{
public:
	Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
	int r() const { return m_r; }
	int c() const { return m_c; }
private:
	int m_r;
	int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	// In the maze, the grid that has been investigated is denoted as '@'

	queue<Coord> coordQueue;
	coordQueue.push(Coord(sr, sc));
	maze[sr][sc] = '@';

	Coord current(0, 0);
	while (!coordQueue.empty())
	{
		current = coordQueue.front();
		//cout << current.r() << ", " << current.c() << endl;
		coordQueue.pop();
		if (current.r() == er && current.c() == ec)
			return true;

		// check if the NORTH of current is viable
		if (maze[current.r() - 1][current.c()] == '.')
		{
			coordQueue.push(Coord(current.r() - 1, current.c()));
			maze[current.r() - 1][current.c()] = '@';
		}

		// check if the EAST of current is viable
		if (maze[current.r()][current.c() + 1] == '.')
		{
			coordQueue.push(Coord(current.r(), current.c() + 1));
			maze[current.r()][current.c() + 1] = '@';
		}

		// check if the SOUTH of current is viable
		if (maze[current.r() + 1][current.c()] == '.')
		{
			coordQueue.push(Coord(current.r() + 1, current.c()));
			maze[current.r() + 1][current.c()] = '@';
		}

		// check if the WEST of current is viable
		if (maze[current.r()][current.c() - 1] == '.')
		{
			coordQueue.push(Coord(current.r(), current.c() - 1));
			maze[current.r()][current.c() - 1] = '@';
		}
	}
	return false;
}

int main()
{
	string maze[10] = {
		"XXXXXXXXXX",
		"X........X",
		"XX.X.XXXXX",
		"X..X.X...X",
		"X..X...X.X",
		"XXXX.XXX.X",
		"X.X....XXX",
		"X..XX.XX.X",
		"X...X....X",
		"XXXXXXXXXX"
	};

	cerr << "This is maze queue!!" << endl;
	if (pathExists(maze, 10, 10, 6, 4, 1, 1))
		cout << "Solvable!" << endl;
	else
		cout << "Out of luck!" << endl;;
}