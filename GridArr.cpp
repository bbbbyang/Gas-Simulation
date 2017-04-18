#include "GridArr.h"

GridArr::GridArr(){
	for(int y = 0; y < MAXROWS; y++){
		for(int x = 0; x < MAXCOLS; x++){
			mass[x][y] = 0;
			Xvelocity[x][y] = 0;
			Yvelocity[x][y] = 0;
		}
	}
}

void GridArr::CopyGrid(GridArr Old){
	for(int y = 0; y < MAXROWS; y++){
		for(int x = 0; x < MAXCOLS; x++){
			mass[x][y] = Old.mass[x][y];
			Xvelocity[x][y] = Old.Xvelocity[x][y];
			Yvelocity[x][y] = Old.Yvelocity[x][y];
		}
	}
}

void GridArr::InitialGrid(){
	float chance;
	for(int y = 0; y < MAXROWS; y++){
		for(int x = 0; x < MAXCOLS; x++){
			chance = rand() % 20;
			if(chance == 1){
				mass[x][y] = rand() % 100;
				Xvelocity[x][y] = rand() % 6 -3;
				Yvelocity[x][y] = rand() % 6 -3;
			}
		}
	}
}

void GridArr::Reset(){
	for(int y = 0; y < MAXROWS; y++){
		for(int x = 0; x < MAXCOLS; x++){
			mass[x][y] = 0;
			Xvelocity[x][y] = 0;
			Yvelocity[x][y] = 0;
		}
	}
}

void GridArr::UpdateCell(GridArr& Other){
	int intx, inty;
	double newx, newy, fractionx, fractiony;
	/* First figure out where the mass from this cell is going, based */
	/* on its velocity.  Assume that its new location is a cell-sized shape, with */
	/* coordinates of lower left hand corner given by newx and newy. */
	for(int y = 0; y < MAXROWS; y++){
		for(int x = 0; x < MAXCOLS; x++){
			newx = x + Xvelocity[y][x];
			newy = y + Yvelocity[y][x];

			/* If the point is outside the grid, move it back inside the grid */
			/* using wraparound. */
			while (newy >= MAXROWS) newy = newy - MAXROWS;
			while (newy < 0.0)             newy = newy + MAXROWS;
			while (newx >= MAXCOLS) newx = newx - MAXCOLS;
			while (newx < 0.0)             newx = newx + MAXCOLS;

			/* Since the velocity may not be an integer, the new location may */
			/* straddle several adjacent cells.  Find which cells the shape */
			/* overlaps and the fractions of each of these cells that it overlaps. */
			/* These cells are called the destination cells. */
			intx = (int) newx;
			inty = (int) newy;
			fractionx = newx - intx;
			fractiony = newy - inty;
			/* The outflow from the current cell (x y) becomes the  */
			/* inflow to the destination cells. It is divided according to  */
			/* the fraction of the shape that overlaps each of these cells. */

			UpdateFromInflow(
				intx,
				inty,
				(1.0 - fractionx) * (1.0 - fractiony)  * mass[y][x],
				Xvelocity[y][x],
				Yvelocity[y][x],
				Other);

			UpdateFromInflow(
				intx,
				inty + 1,
				(1.0 - fractionx) * fractiony          * mass[y][x],
				Xvelocity[y][x],
				Yvelocity[y][x],
				Other);

			UpdateFromInflow(
				intx + 1,
				inty,
				fractionx          * (1.0 - fractiony) * mass[y][x],
				Xvelocity[y][x],
				Yvelocity[y][x],
				Other);

			UpdateFromInflow(
				intx + 1,
				inty + 1,
				fractionx          * fractiony         * mass[y][x],
				Xvelocity[y][x],
				Yvelocity[y][x],
				Other);
		}
	}
}

void GridArr::UpdateFromInflow(int x, int y, double m2, double vx2, double vy2, GridArr& Other){
	double m1, vx1, vy1;
	/* If the point is outside the grid, move it back inside the grid */
	/* using wraparound.  Code not shown.*/
	
	/* current conditions in this cell before adding the inflow */
	m1 = Other.mass[y][x];
	vx1 = Other.Xvelocity[y][x];
	vy1 = Other.Yvelocity[y][x];

	if (m1 + m2 == 0.0)    /* avoid dividing by zero */
	{
		Other.mass[y][x] = 0.0;
		Other.Xvelocity[y][x] = 0.0;
		Other.Yvelocity[y][x] = 0.0;
	}
	else
	{
		/* Compute new mass as the sum of the existing and inflowing gases */
		Other.mass[y][x] = m1 + m2;

		/* Calculate the new velocity as the weighted average of the velocities  */
		/* of the existing and inflowing gases.  The weighting is done according */
		/* to the masses.  */
		Other.Xvelocity[y][x] = (m1 * vx1 + m2 * vx2) / (m1 + m2);
		Other.Yvelocity[y][x] = (m1 * vy1 + m2 * vy2) / (m1 + m2);
	}
}