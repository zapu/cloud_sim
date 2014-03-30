#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "system.h"
#include <iostream>

#include "../gnuplot-iostream/gnuplot-iostream.h"

Project project;

int main() {
	for(int i = 0; i < 10000; i++) {
		auto job = new Job();
		project.m_jobs.insert(job);
	}

	for(int i = 0; i < 10; i++) {
		auto node = new Node();
		node->m_trust = getRand(0.0f, 0.1f);
		project.addNode(node);
	}

	Gnuplot gp;

	std::vector<std::pair<double, double> > xy_pts_A;
	for(double x=-2; x<2; x+=0.01) {
		double y = x*x*x;
		xy_pts_A.push_back(std::make_pair(x, y));
	}

	std::vector<std::pair<double, double> > xy_pts_B;
	for(double alpha=0; alpha<1; alpha+=1.0/24.0) {
		double theta = alpha*2.0*3.14159;
		xy_pts_B.push_back(std::make_pair(cos(theta), sin(theta)));
	}

	//gp << "set terminal wx" << std::endl;
	gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
	// Data will be sent via a temporary file.  These are erased when you call
	// gp.clearTmpfiles() or when gp goes out of scope.  If you pass a filename
	// (e.g. "gp.file1d(pts, 'mydata.dat')"), then the named file will be created
	// and won't be deleted (this is useful when creating a script).
	gp << "plot" << gp.file1d(xy_pts_A) << "with lines title 'cubic',"
		<< gp.file1d(xy_pts_B) << "with points title 'circle'" << std::endl;

#ifdef _WIN32
	// For Windows, prompt for a keystroke before the Gnuplot object goes out of scope so that
	// the gnuplot window doesn't get closed.
	std::cout << "Press enter to exit." << std::endl;
	std::cin.get();
#endif

	std::cout << "Simulating..." << std::endl;

	//project.simulate();
}