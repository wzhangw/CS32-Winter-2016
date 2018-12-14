bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
	if (sr == er && sc == ec)
		return true;
	maze[sr][sc] = 'X';
	int pos1[4] = { sr - 1, sr, sr , sr + 1 };
	int pos2[4] = { sc, sc - 1, sc + 1, sc };
	for (int i = 0; i < 4; i++)
		if (maze[pos1[i]][pos2[i]] == '.')
			if (pathExists(maze, nRows, nCols, pos1[i], pos2[i], er, ec))
				return true;
	return false;
}