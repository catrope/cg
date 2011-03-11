//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Roan Kattouw
//    Jan Paul Posma
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "triangle.h"
#include "quad.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
	assert(node.size()==3);
	node[0] >> t.x;
	node[1] >> t.y;
	node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
	Triple t;
	node[0] >> t.x;
	node[1] >> t.y;
	node[2] >> t.z;
	return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
	Material *m = new Material();
	node["color"] >> m->color;
	node["ka"] >> m->ka;
	node["kd"] >> m->kd;
	node["ks"] >> m->ks;
	node["n"] >> m->n;
	m->refract = parseOptionalDouble(node.FindValue("refract"), 0.0);
	m->eta = parseOptionalDouble(node.FindValue("eta"), 1.0);
	return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
	Object *returnObject = NULL;
	std::string objectType;
	node["type"] >> objectType;

	if (objectType == "sphere") {
		Point pos;
		node["position"] >> pos;
		double r;
		node["radius"] >> r;
		Sphere *sphere = new Sphere(pos,r);		
		returnObject = sphere;
	} else if (objectType == "triangle") {
		Point p1, p2, p3;
		node["p1"] >> p1;
		node["p2"] >> p2;
		node["p3"] >> p3;
		Triangle *triangle = new Triangle(p1, p2, p3);
		returnObject = triangle;
	} else if (objectType == "quad") {
		Point p1, p2, p3, p4;
		node["p1"] >> p1;
		node["p2"] >> p2;
		node["p3"] >> p3;
		node["p4"] >> p4;
		Quad *quad = new Quad(p1, p2, p3, p4);
		returnObject = quad;
	}
	
	if (returnObject) {
		// read the material and attach to object
		returnObject->material = parseMaterial(node["material"]);
	}

	return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
	Point position;
	node["position"] >> position;
	Color color;
	node["color"] >> color;
	return new Light(position,color);
}

Scene::RenderMode Raytracer::parseRenderMode(const YAML::Node* node)
{
	std::string mode;
	if(node == NULL) return Scene::phong;
	*node >> mode;
	if(mode == "zbuffer") return Scene::zbuffer;
	else if(mode == "normal") return Scene::normal;
	else return Scene::phong;
}

Camera Raytracer::parseCamera(const YAML::Node& node)
{
	Camera cam;
	cam.eye = parseTriple(node["eye"]);
	cam.center = parseTriple(node["center"]);
	cam.up = parseTriple(node["up"]);
	cam.viewWidth = node["viewSize"][0];
	cam.viewHeight = node["viewSize"][1];
	cam.apertureRadius = parseOptionalDouble(node.FindValue("apertureRadius"), 0.0);
	cam.apertureSamples = parseUnsignedInt(node.FindValue("apertureSamples"), 1);
	return cam;
}

bool Raytracer::parseBool(const YAML::Node* node, bool defaultVal)
{
	bool retval;
	if(node == NULL) {
		return defaultVal;
	}
	*node >> retval;
	return retval;
}

unsigned int Raytracer::parseUnsignedInt(const YAML::Node* node, unsigned int defaultVal)
{
	unsigned int retval;
	if(node == NULL) {
		return defaultVal;
	}
	*node >> retval;
	return retval;
}

double Raytracer::parseOptionalDouble(const YAML::Node* node, double defaultVal)
{
	double retval;
	if(node == NULL) {
		return defaultVal;
	}
	*node >> retval;
	return retval;
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
	// Initialize a new scene
	scene = new Scene();

	// Open file stream for reading and have the YAML module parse it
	std::ifstream fin(inputFilename.c_str());
	if (!fin) {
		cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
		return false;
	}
	try {
		YAML::Parser parser(fin);
		if (parser) {
			YAML::Node doc;
			parser.GetNextDocument(doc);

			// Read scene configuration options
			if (doc.FindValue("Eye")) scene->setEye(parseTriple(doc["Eye"]));
			else if (doc.FindValue("Camera")) scene->setCamera(parseCamera(doc["Camera"]));
			
			scene->setRenderMode(parseRenderMode(doc.FindValue("RenderMode")));
			scene->setShadows(parseBool(doc.FindValue("Shadows"), false));
			scene->setMaxRecursionDepth(parseUnsignedInt(doc.FindValue("MaxRecursionDepth"), 0));
			
			if (doc.FindValue("SuperSampling") != NULL)
			{
				scene->setSuperSamplingFactor(parseUnsignedInt(doc["SuperSampling"].FindValue("factor"), 1));
				scene->setSuperSamplingJitter(parseBool(doc["SuperSampling"].FindValue("jitter"), true));
			}
			else
			{
				scene->setSuperSamplingFactor(1);
				scene->setSuperSamplingJitter(true);
			}

			// Read and parse the scene objects
			const YAML::Node& sceneObjects = doc["Objects"];
			if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
				cerr << "Error: expected a sequence of objects." << endl;
				return false;
			}
			for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
				Object *obj = parseObject(*it);
				// Only add object if it is recognized
				if (obj) {
					scene->addObject(obj);
				} else {
					cerr << "Warning: found object of unknown type, ignored." << endl;
				}
			}

			// Read and parse light definitions
			const YAML::Node& sceneLights = doc["Lights"];
			if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
				cerr << "Error: expected a sequence of lights." << endl;
				return false;
			}
			for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
				scene->addLight(parseLight(*it));
			}
		}
		if (parser) {
			cerr << "Warning: unexpected YAML document, ignored." << endl;
		}
	} catch(YAML::ParserException& e) {
		std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
		return false;
	}

	cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
	return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
	Camera cam = scene->getCamera();
	Image img(cam.viewWidth, cam.viewHeight);
	cout << "Tracing..." << endl;
	scene->render(img);
	cout << "Writing image to " << outputFilename << "..." << endl;
	img.write_png(outputFilename.c_str());
	cout << "Done." << endl;
}
