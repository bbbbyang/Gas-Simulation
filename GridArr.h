#include <cstdlib>
#include <iostream>
using namespace std;
#define MAXROWS 200
#define MAXCOLS 200

class GridArr{
	public:
		float mass[MAXROWS][MAXCOLS];
		float Xvelocity[MAXROWS][MAXCOLS];
		float Yvelocity[MAXROWS][MAXCOLS];
	public:
		GridArr();
		void CopyGrid(GridArr Old);
		void InitialGrid();
		void Reset();
		void UpdateCell(GridArr& Other);
		void UpdateFromInflow(int x, int y, double m2, double vx2, double vy2, GridArr& Other);
};