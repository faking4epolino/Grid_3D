#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>
#include "glut.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glut32.lib")


float zoom = 10.0;
int last_x, last_y;
float rotation_x = 0.0;
float rotation_y = 0.0;
float rotation_z = 0.0;
int numVetrices;
int numEdges;
int numFaces;
long double minX = 0;
long double minY = 0;
long double minZ = 0;
long double maxX = 0;
long double maxY = 0;
long double maxZ = 0;
float initial_rotation_x = 0.0; // the initial position of the virtual camera
float initial_rotation_y = 0.0;
float initial_zoom = 10.0;
bool rotX = false;
bool rotY = false;
bool rotZ = false;
int rotationSpeed = 1;
bool flag = false;
bool flagSpace = false; 
bool flagAxes = false; 

class Vertex
{
public:

	long double x;
	long double y;
	long double z;

};

class Edge
{
public:

	long double point1;
	long double point2;
};

class Face
{
public:

	long double edge1;
	long double edge2;
	long double edge3;

};

class Grid
{
	std::vector <Vertex> vertices; 
	std::vector <Edge> edges;     
	std::vector <Face> faces;     
	std::vector <char> locations; 
								
public:

	bool LoadFromFile(std::string fileName)
	{
		std::string line;

		std::ifstream file(fileName);
		if (!file.is_open())
		{
			std::cerr << "ERROR: Failed to open file " << fileName << std::endl;
			exit(NULL);
			return false;
		}
		else
		{
			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());//удаляем из строки все элементы которые не являются числом и преобразовываем оставшееся число в int
			numVetrices = std::stoi(line);

			for (int i = 0; i < numVetrices; ++i)
			{
				std::getline(file, line);

				std::istringstream str(line);		
				long double x = 0, y = 0, z = 0;
				str >> x;
				str.ignore();
				str >> y;
				str.ignore();
				str >> z;
				vertices.push_back({ x, y, z });

			}



			std::getline(file, line);
			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());
			numEdges = std::stoi(line);

			for (int i = 0; i < numEdges; ++i)
			{
				std::getline(file, line);

				std::istringstream sss(line);
				long double point1 = 0, point2 = 0;
				sss >> point1;
				sss.ignore();
				sss >> point2;

				edges.push_back({ point1, point2 });


			}


			std::getline(file, line);
			std::getline(file, line);
			line.erase(std::remove_if(line.begin(), line.end(), [](char c) {return !isdigit(c);  }), line.end());
			numFaces = std::stoi(line);

			for (int i = 0; i < numFaces; ++i)
			{
				std::getline(file, line);

				std::istringstream sss(line);
				long double edge1 = 0, edge2 = 0, edge3 = 0;
				sss >> edge1;
				sss.ignore();
				sss >> edge2;
				sss.ignore();
				sss >> edge3;

				faces.push_back({ edge1, edge2, edge3 });


			}

		}


		file.close();
		return true;
	}

	void MinMaxValues() {

		 minX = vertices[0].x;
		 minY = vertices[0].y;
		 minZ = vertices[0].z;

		 maxX = vertices[0].x;
		 maxY = vertices[0].y;
		 maxZ = vertices[0].z;

		for (const auto& vertex : vertices) {

			minX = std::min(minX, vertex.x);
			minY = std::min(minY, vertex.y);
			minZ = std::min(minZ, vertex.z);

			maxX = std::max(maxX, vertex.x);
			maxY = std::max(maxY, vertex.y);
			maxZ = std::max(maxZ, vertex.z);
		}

		std::cout << "Min X: " << minX << ", Max X: " << maxX << std::endl;
		std::cout << "Min Y: " << minY << ", Max Y: " << maxY << std::endl;
		std::cout << "Min Z: " << minZ << ", Max Z: " << maxZ << std::endl;

	}


	void drawGrid() {

		for (int i = 0; i < numEdges; ++i) {



			glBegin(GL_LINES);

			glLineWidth(4);
			glColor3f(1.0, 1.0, 1.0);

			glVertex3f(vertices[edges[i].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[i].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[i].point1].z - (maxZ - abs(minZ)) / 2.0);
			
			glVertex3f(vertices[edges[i].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[i].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[i].point2].z - (maxZ - abs(minZ)) / 2.0);


			glEnd();

		}

	}


	void drawTriangles() {



		for (int i = 0; i < numFaces; ++i) {


			glBegin(GL_TRIANGLES);
			glColor3f(0.0, 0.0, 1.0);
				
			if (faces[i].edge1 < 0) {

				long double a = abs(faces[i].edge1) - 1;

				glVertex3f(vertices[edges[a].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[a].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[a].point2].z - (maxZ - abs(minZ)) / 2.0);

			}
			else {

				glVertex3f(vertices[edges[faces[i].edge1].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge1].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge1].point1].z - (maxZ - abs(minZ)) / 2.0);

			}


			if (faces[i].edge2 < 0) {

				long double b = abs(faces[i].edge2) - 1;

				glVertex3f(vertices[edges[b].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[b].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[b].point2].z - (maxZ - abs(minZ)) / 2.0);

			}
			else {
				glVertex3f(vertices[edges[faces[i].edge2].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge2].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge2].point1].z - (maxZ - abs(minZ)) / 2.0);

			}

			if (faces[i].edge3 < 0) {

				long double c = abs(faces[i].edge3) - 1;

				glVertex3f(vertices[edges[c].point2].x - (maxX - abs(minX)) / 2.0, vertices[edges[c].point2].y - (maxY - abs(minY)) / 2.0, vertices[edges[c].point2].z - (maxZ - abs(minZ)) / 2.0);

			}
			else {
				glVertex3f(vertices[edges[faces[i].edge3].point1].x - (maxX - abs(minX)) / 2.0, vertices[edges[faces[i].edge3].point1].y - (maxY - abs(minY)) / 2.0, vertices[edges[faces[i].edge3].point1].z - (maxZ - abs(minZ)) / 2.0);
			}


			glEnd();


		}
	}
	void minmaxLine() {


		glBegin(GL_LINES);


		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(4);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01 - 0.3, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01 - 0.3);



		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01 + 0.3, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, (maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, (maxZ - abs(minZ)) / 2.0 + 0.01 - 0.3);


		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01 - 0.3, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, -(maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f((maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01 + 0.3);



		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01 + 0.3, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01 - 0.3, -(maxZ - abs(minZ)) / 2.0 + 0.01);

		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01);
		glVertex3f(-(maxX - abs(minX)) / 2.0 + 0.01, (maxY - abs(minY)) / 2.0 + 0.01, -(maxZ - abs(minZ)) / 2.0 + 0.01 + 0.3);




		glColor3f(0.0, 0.0, 1.0);
		glLineWidth(4);
		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01 + 0.3, (abs(minY) - maxY) / 2.0+ 0.01, (abs(minZ) - maxZ) / 2.0+ 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01 + 0.3, (abs(minZ) - maxZ) / 2.0+ 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01 + 0.3);


		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01 - 0.3, (abs(minY) - maxY) / 2.0+ 0.001, (abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.001);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01 + 0.3, (abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, (abs(minZ) - maxZ) / 2.0 + 0.01 + 0.3);



		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01 + 0.3, (abs(minY) - maxY) / 2.0+ 0.001, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01 + 0.3, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f((abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01 - 0.3);



		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01 - 0.3, (abs(minY) - maxY) / 2.0+ 0.001, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01 + 0.3, -(abs(minZ) - maxZ) / 2.0 + 0.01);

		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01);
		glVertex3f(-(abs(minX) - maxX) / 2.0 + 0.01, (abs(minY) - maxY) / 2.0 + 0.01, -(abs(minZ) - maxZ) / 2.0 + 0.01 - 0.3);
		glEnd();


	}



};


Grid mesh;

void rotationObj(int value) {

	if (rotX) {
		rotation_x += rotationSpeed;
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, rotationObj, 0);
	}
	if (rotY) {
		rotation_y += rotationSpeed;
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, rotationObj, 0);

	}
	if (rotZ) {
		rotation_z += rotationSpeed;
		glutPostRedisplay();
		glutTimerFunc(1000 / 60, rotationObj, 0);
	}
}

void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		last_x = x;
		last_y = y;
	}
}

void motion(int x, int y) {
	rotation_x += (y - last_y);
	rotation_y += (x - last_x);
	last_x = x;
	last_y = y;
	glutPostRedisplay();
}

void read_special_kb(int key, int, int) {
	switch (key) {
	case GLUT_KEY_HOME:
		rotation_x = initial_rotation_x;
		rotation_y = initial_rotation_y;
		zoom = initial_zoom;
		glutPostRedisplay();
		break;
	case GLUT_KEY_F7: {

		if (!rotX && !rotY && !rotZ) {

			rotX = true;
			glutTimerFunc(1000 / 60, rotationObj, 0);

		}
		else if (rotX && !rotY && !rotZ) {

			rotX = false;
			rotY = true;
			glutTimerFunc(1000 / 60, rotationObj, 0);
		}
		else if (!rotX && rotY && !rotZ) {

			rotY = false;
			rotZ = true;
			glutTimerFunc(1000 / 60, rotationObj, 0);
		}
		else if (!rotX && !rotY && rotZ) {

			rotZ = false;
		}
		break;
		glutPostRedisplay();
	}


	}
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '+':
		zoom -= 1.0; // reducing the zoom value to get closer to the shape
		break;
	case '-':
		zoom += 1.0; // increasing the zoom value to move away from the shape
		break;
	case 27:
		exit(0); // the Esc key to exit the program
		break;	
	case 'm':

		if (flag == true) {
			flag = false;

		}
		else {
			flag = true;
		}

		break;
	case ' ':
		if (flagSpace == true) {
			flagSpace = false;

		}
		else {
			flagSpace = true;
		}
	default:
		break;
	}

	glutPostRedisplay();
}

void drawAxes()
{
	glLoadIdentity();
	gluLookAt(0, 0, 18.0f, 11.0f, 7.0f, 0, 0, 1, 0);


	glRotatef(rotation_x, 1, 0, 0);
	glRotatef(rotation_y, 0, 1, 0);
	glRotatef(rotation_z, 0, 0, 1);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();


}



void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glTranslatef(0, 0, -zoom);
	glRotatef(rotation_x, 1, 0, 0);
	glRotatef(rotation_y, 0, 1, 0);
	glRotatef(rotation_z, 0, 0, 1);
	mesh.minmaxLine(); 

	if (flagSpace == true)
	{
		mesh.drawGrid();
	}
	if (flag == true) {
		mesh.drawGrid();
	}
	if (flag == false) {
		mesh.drawTriangles();
	}

	drawAxes();
	glutSwapBuffers();
}

void ReshapeWindow(GLsizei width, GLsizei height) {
	if (height == 0)
		height = 1;
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
}

int main(int argc, char* argv[]) {

	std::string fileName = "diode.txt";

	mesh.LoadFromFile(fileName);
	std::cout << "Success! File is open!\n" << std::endl;
	mesh.MinMaxValues();
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1920, 1080);
	glutCreateWindow("3D Grid");
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearColor(0, 0, 0, 1);

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ReshapeWindow);

	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(read_special_kb);


	glutMainLoop();

	return 0;
}


