#include "Mesh.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;
Mesh::Mesh()
{
	Mesh::nodes = 0;
	Mesh::polygons = 0;
	Mesh::edges = 0;
}


Mesh::~Mesh()
{
	for (int i = 0; i < polygons; i++)
		delete polygon[i].nodes;
	delete polygon;
	delete node;
}

bool Mesh::loadOff(const char* filename){
	ifstream modelfile(filename);
	if (modelfile.is_open()) {
		string line; //Using short-circuit-or intentionally here
		if (!getline(modelfile, line) || line.compare("OFF")){
			modelfile.close();
			return false;
		} //TODO error handling because file is empty or no OFF file
		if (getline(modelfile, line)){
			std::istringstream istr(line);
			istr >> nodes >> polygons >> edges;
		}
		else {
			modelfile.close();
			return false;
		}//TODO error handling because line is missing
		node = new nodestruct[nodes];
		polygon = new poly[polygons];
		for (int i = 0; i < nodes; i++)
		if (getline(modelfile, line)){
			std::istringstream istr(line);
			std::memset(node[i].normal, 0.0f, 3 * sizeof GLfloat);
			istr >> node[i].node[0] >> node[i].node[1] >> node[i].node[2];
		}
		else {
			modelfile.close();
			return false;
		}; //TODO error handling because line is missing
		{//Center the model
			GLfloat sum[3] = { 0.0f, 0.0f, 0.0f };
			for (int i = 0; i < nodes; i++)
			for (int j = 0; j < 3; j++)
				sum[j] += node[i].node[j];
			for (int i = 0; i < 3; i++)
				sum[i] /= nodes;
			for (int i = 0; i < nodes; i++)
			for (int j = 0; j < 3; j++)
				node[i].node[j] -= sum[j];
		}
		{//Normalize the distance
			GLfloat avg = 0;
			for (int i = 0; i < nodes; i++)
				avg += std::sqrt(node[i].node[0] * node[i].node[0] + node[i].node[1] * node[i].node[1] + node[i].node[2] * node[i].node[2]);
			avg /= nodes;
			for (int i = 0; i < nodes; i++)
			for (int j = 0; j < 3; j++)
				node[i].node[j] /= avg;
		}
		for (int i = 0; i < polygons; i++)
		if (getline(modelfile, line)){
			std::istringstream istr(line);
			istr >> polygon[i].size;
			polygon[i].nodes = new int[polygon[i].size];
			for (int j = 0; j < polygon[i].size; j++)
				istr >> polygon[i].nodes[j];
			{ //Calculate surface normal vector
				GLfloat a[3], b[3];
				for (int j = 0; j < 3; j++){ // a = v1 - v2, b = v2 - v3
					a[j] = node[polygon[i].nodes[0]].node[j] - node[polygon[i].nodes[1]].node[j];
					b[j] = node[polygon[i].nodes[1]].node[j] - node[polygon[i].nodes[2]].node[j];
				}
				this->crossproduct(a, b, polygon[i].normal);
				this->normalizevector(polygon[i].normal);
			}
			{ //Apply surface normal vector to vertex normal vectors
				for (int j = 0; j < 3; j++)
				for (int k = 0; k < 3; k++)
					node[polygon[i].nodes[j]].normal[k] += polygon[i].normal[k];
			}
		}
		else {
			modelfile.close();
			return false;
		}; //TODO error handling because line is missing
		modelfile.close();
		for (int i = 0; i < nodes; i++) //Normalize vertex normal vectors
			this->normalizevector(node[i].normal);
	}
	else
		return false; //TODO error handling because file was not opened
	return true;
}

void Mesh::setRenderMode(RenderMode mode){
	this->rendermode = mode;
}

void Mesh::render(){
	switch (rendermode){
	case FLAT_RENDERER:
		glShadeModel(GL_FLAT);
		this->renderFlat();
		break;
	case GOURAUD_RENDERER:
		glShadeModel(GL_SMOOTH);
		this->renderSmooth();
		break;
	}
}

void Mesh::normalizevector(GLfloat* vector){
	GLfloat scale = vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2];
	int integer = 0x5f3759df - (*(long*)&scale >> 1); //Using fast inverse square root algorithm
	GLfloat isr = *(GLfloat*)&integer;
	isr *= 1.5f - (scale * 0.5f * isr * isr); //One iteration should be precise enough
	//isr *= 1.5f - (scale * 0.5f * isr * isr);
	//isr *= 1.5f - (scale * 0.5f * isr * isr); //Three iterations are enough because GLfloat is less precise
	vector[0] *= isr; //Normalize the vector
	vector[1] *= isr; //isr is 1/sqrt(scale)
	vector[2] *= isr;
}

void Mesh::crossproduct(GLfloat* a, GLfloat* b, GLfloat* result){
	result[0] = a[1] * b[2] - a[2] * b[1];
	result[1] = a[2] * b[0] - a[0] * b[2];
	result[2] = a[0] * b[1] - a[1] * b[0];
}

void Mesh::renderFlat(){
	for (int i = 0; i < polygons; i++){
		glNormal3f(polygon[i].normal[0], polygon[i].normal[1], polygon[i].normal[2]);
		glBegin(GL_TRIANGLES); {
			for (int j = 0; j < polygon[i].size; j++)
				glVertex3f(node[polygon[i].nodes[j]].node[0], node[polygon[i].nodes[j]].node[1], node[polygon[i].nodes[j]].node[2]);
		} glEnd();
	}
}

void Mesh::renderSmooth(){
	for (int i = 0; i < polygons; i++){
		glBegin(GL_TRIANGLES); {
			for (int j = 0; j < polygon[i].size; j++){
				glNormal3f(node[polygon[i].nodes[j]].normal[0], node[polygon[i].nodes[j]].normal[1], node[polygon[i].nodes[j]].normal[2]);
				glVertex3f(node[polygon[i].nodes[j]].node[0], node[polygon[i].nodes[j]].node[1], node[polygon[i].nodes[j]].node[2]);
			}
		} glEnd();
	}
}

void Mesh::printmesh(){
	cout << "Nodes: " << nodes << " Polygons: " << polygons << " Edges: " << edges << "\n";
	cout << "Nodes: \n";
	for (int i = 0; i < nodes; i++){
		cout << i << ": " << node[i].node[0] << " " << node[i].node[1] << " " << node[i].node[2] << "\n";
		cout << "Normal " << node[i].normal[0] << " " << node[i].normal[1] << " " << node[i].normal[2] << "\n";
	}
	cout << "Polygons: \n";
	for (int i = 0; i < polygons; i++)
	{
		cout << i << ": with "<< polygon[i].size << " Nodes: ";
		for (int j = 0; j < polygon[i].size; j++)
			cout << polygon[i].nodes[j] << " ";
		cout << "Normal " << polygon[i].normal[0] << " " << polygon[i].normal[1] << " " << polygon[i].normal[2] << "\n";
	}
}

