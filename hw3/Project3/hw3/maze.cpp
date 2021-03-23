#include <string>

class Coord
{
public:
	Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
	int r() const { return m_row; }
	int c() const { return m_col; }
private:
	int m_row;
	int m_col;
};

bool pathExists(std::string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	if (sr == er && sc == ec) return true;

	maze[sr][sc] = 'X';

	if(sr > 0){
		if (maze[sr - 1][sc] == '.')
			if (pathExists(maze, nRows, nCols, sr - 1, sc, er, ec))
				return true;
	}
	if (sc < nCols) {
		if (maze[sr][sc + 1] == '.')
			if (pathExists(maze, nRows, nCols, sr, sc + 1, er, ec))
				return true;
	}
	if (sr < nRows) {
		if (maze[sr + 1][sc] == '.')
			if (pathExists(maze, nRows, nCols, sr + 1, sc, er, ec))
				return true;
	}
	if (sc > 0) {
		if (maze[sr][sc - 1] == '.')
			if (pathExists(maze, nRows, nCols, sr, sc - 1, er, ec))
				return true;
	}


	return false;
}