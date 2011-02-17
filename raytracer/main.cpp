//
//  Framework for a raytracer
//  File: main.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"

int main(int argc, char *argv[])
{
	cout << "Introduction to Computer Graphics - Raytracer" << endl << endl;
	if (argc < 2 || argc > 3) {
		cerr << "Usage: " << argv[0] << " in-file [out-file.png]" << endl;
		return 1;
	}

	Raytracer raytracer;

	if (!raytracer.readScene(argv[1])) {
		cerr << "Error: reading scene from " << argv[1] << " failed - no output generated."<< endl;
		return 1;
	}
	std::string ofname;
	if (argc>=3) {
		// Output filename provided on command line
		ofname = argv[2];
	} else {
		// Output filename not provided. Replace .yaml with .png and add timestamp
		ofname = argv[1];
		if (ofname.size()>=5 && ofname.substr(ofname.size()-5)==".yaml") {
			ofname = ofname.substr(0,ofname.size()-5);
		}
		
		// Generate formatted timestamp
	time_t rawtime;
	struct tm *timeinfo;
	char appendix[30];
	time( &rawtime );
	timeinfo = localtime( &rawtime );
	strftime(appendix, 30, "-%Y%m%d-%H%M%S.png", timeinfo);

	ofname += appendix;
	}
	raytracer.renderToFile(ofname);

	return 0;
}
