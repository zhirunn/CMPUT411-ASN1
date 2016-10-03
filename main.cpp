#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
vector<GLfloat> vertex_data, normalize_vertex;
vector<vector<GLfloat> > faces_data;
GLfloat sumx, sumy, sumz, meanx, meany, meanz, minx, miny, minz, maxx, maxy, maxz, scale, scalex, scaley, scalez, Xvalue, Yvalue, Zvalue, Xangle, Yangle, Zangle, CXvalue, CYvalue, CZvalue, CXangle, CYangle, CZangle = 0.0;
GLuint modelist;
GLboolean Orthographic = true;

void loadFile(unsigned int arguments, char **file) {
	if(arguments != 2) {
		cout << "This program requires exactly 2 arguments. Refer to 'README.md' for more infomation." << endl;
		abort();
	}
	ifstream modelfile(file[1]);
	if(modelfile.is_open()) {
		string line;
		while(getline(modelfile, line)) {
			if(line.c_str()[0] == 'v') {
				GLfloat x, y, z;
				line[0] = ' ';
				sscanf(line.c_str(), "%f %f %f", &x, &y, &z);
				sumx += x, sumy += y, sumz += z;
				
				if(minx == 0 && x != 0) {
					minx = x;
				}
				else if(x < minx){
					minx = x;
				}
				if(miny == 0 && y != 0) {
					miny = y;
				}
				else if(y < miny){
					miny = y;
				}
				if(minz == 0 && z != 0) {
					minz = z;
				}
				else if(z < minz){
					minz = z;
				}
				
				if(maxx == 0 && x != 0) {
					maxx = x;
				}
				else if(x > maxx){
					maxx = x;
				}
				if(maxy == 0 && y != 0) {
					maxy = y;
				}
				else if(y > maxy){
					maxy = y;
				}
				if(maxz == 0 && z != 0) {
					maxz = z;
				}
				else if(z > maxz){
					maxz = z;
				}
				vertex_data.push_back(x), vertex_data.push_back(y), vertex_data.push_back(z);
			}
			else if(line.c_str()[0] == 'f') {
				line[0] = ' ';
				istringstream iss(line);
				vector<GLfloat> face_data;
				while(iss) {
					unsigned int face_input;
					iss >> face_input;
					if(iss.fail()) {
						break;
					}
					face_data.push_back(face_input);
				}
				faces_data.push_back(face_data);
			}
		}
		modelfile.close();
	}
	else {
		cout << "Well this is embarassing but we can't open your file. Are you sure it's in .obj format?" << endl;
		abort();
	}

	meanx = sumx/(vertex_data.size()/3);
	meany = sumy/(vertex_data.size()/3);
	meanz = sumz/(vertex_data.size()/3);
	
	scalex = maxx - minx;
	scaley = maxy - miny;
	scalez = maxz - minz;
	if(scalex > scaley) {
		scale = scalex;
	}
	else if(scalex < scaley) {
		scale = scaley;	
	}
	if(scale < scalez) {
		scale = scalez;	
	}

	unsigned int i = 0;
	while(i < vertex_data.size()) {
		if(i % 3 == 2) {
			normalize_vertex.push_back(((vertex_data[i]-meanz)/scale) - 2);
		}
		else if(i % 3 == 1) {
			normalize_vertex.push_back((vertex_data[i]-meany)/scale);
		} 
		else if(i % 3 == 0) {
			normalize_vertex.push_back((vertex_data[i]-meanx)/scale);	
		}
		i++;
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	modelist = glGenLists(1);
	glNewList(modelist, GL_COMPILE);
	unsigned int u = 0;
	while(u < faces_data.size()) {
		glBegin(GL_POLYGON);
		unsigned int e = 0;
		while(e < faces_data[u].size()){
			glVertex3f(normalize_vertex.at(((faces_data[u][e])-1)*3), normalize_vertex.at((((faces_data[u][e])-1)*3)+1), normalize_vertex.at((((faces_data[u][e])-1)*3)+2));
			e++;
		}
		glEnd();
		u++;
	}
	glEndList();
}

void drawScene(void) {
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glTranslatef(Xvalue, Yvalue, Zvalue);
	glTranslatef(0, 0, -2);
	glRotatef(Xangle, 1.0, 0.0, 0.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glTranslatef(0, 0, 2);
	
	glTranslatef(CXvalue, CYvalue, CZvalue);
	glRotatef(CXangle, 1.0, 0.0, 0.0);
	glRotatef(CYangle, 0.0, 1.0, 0.0);
	glRotatef(CZangle, 0.0, 0.0, 1.0);
	
	glCallList(modelist);
	glutSwapBuffers();
}

void setup(void) {
   glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	GLfloat fogcolor[4] = {0.0, 0.0, 0.0, 0.0};
	glFogfv(GL_FOG_COLOR, fogcolor);
	glHint(GL_FOG_HINT, GL_NICEST);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 1);
	glFogf(GL_FOG_END, 5);
	glFogf(GL_FOG_DENSITY, 1);
   glDisable(GL_FOG);
}

void printInteraction(void)
{
   cout << "User Interactions:\n";
   cout << "Use the 'v' or 'V' keys to switch between orthographic and perspective views.\n";
   cout << "Use the 'Left' or 'Right' arrows to translate the model -0.1 or 0.1 units about the x-axis.\n";
   cout << "Use the 'Down' or 'Up' arrows to translate the model -0.1 or 0.1 units about the y-axis.\n";
   cout << "Use the 'n' or 'N' keys to translate the model -0.1 or 0.1 units about the z-axis.\n";
   cout << "Use the 'p' or 'P' keys to rotate the model -10 or 10 degrees counterclockwise around the x-axis.\n";
   cout << "Use the 'y' or 'Y' keys to rotate the model -10 or 10 degrees counterclockwise around the y-axis.\n";
   cout << "Use the 'r' or 'R' keys to rotate the model -10 or 10 degrees counterclockwise around the z-axis.\n";
   cout << "Use the 'd' or 'D' keys to translate the camera -0.1 or 0.1 units along the x-axis.\n";
   cout << "Use the 'c' or 'C' keys to translate the camera -0.1 or 0.1 units along the y-axis.\n";
   cout << "Use the 'z' or 'Z' keys to translate the camera -0.1 or 0.1 units along the z-axis.\n";
   cout << "Use the 't' or 'T' keys to rotate the camera -10 or 10 degrees counterclockwise around its x-axis.\n";
   cout << "Use the 'a' or 'A' keys to rotate the camera -10 or 10 degrees counterclockwise around its y-axis.\n";
   cout << "Use the 'l' or 'L' keys to rotate the camera -10 or 10 degrees counterclockwise around its z-axis.\n";
   cout << "Use the 'x' key to reset the model to its original position.\n";
   cout << "Use the 'q' key to quit the program.\n";
   cout << "Use the 'f' or 'F' key to disable or enable fog.\n";
}

void keyInput(unsigned char key, int x, int y) {
	switch (key) {
		case 'n':
		   Zvalue -= 0.1;
		   break;
		case 'N':
		   Zvalue += 0.1;
		   break;
		case 'p':
		   Xangle -= 10.0;
		   break;
		case 'P':
		   Xangle += 10.0;
		   break;
		case 'y':
		   Yangle -= 10.0;
		   break;
		case 'Y':
		   Yangle += 10.0;
		   break;
		case 'r':
		   Zangle -= 10.0;
		   break;
		case 'R':
		   Zangle += 10.0;
		   break;
		case 'd':
		   CXvalue += 0.1;
		   break;
		case 'D':
		   CXvalue -= 0.1;
		   break;
		case 'c':
		   CYvalue += 0.1;
		   break;
		case 'C':
		   CYvalue -= 0.1;
		   break;
		case 'z':
		   CZvalue += 0.1;
		   break;
		case 'Z':
		   CZvalue -= 0.1;
		   break;
		case 't':
		   CXangle += 10.0;
		   break;
		case 'T':
		   CXangle -= 10.0;
		   break;
		case 'a':
		   CYangle += 10.0;
		   break;
		case 'A':
		   CYangle -= 10.0;
		   break;
		case 'l':
		   CZangle += 10.0;
		   break;
		case 'L':
		   CZangle -= 10.0;
		   break;
		case 'q':
			exit(0);
			break;
		case 'x':
			Xvalue = 0.0;
			Yvalue = 0.0;
			Zvalue = 0.0;
			Xangle = 0.0;
			Yangle = 0.0;
			Zangle = 0.0;
			CXvalue = 0.0;
			CYvalue = 0.0;
			CZvalue = 0.0;
			CXangle = 0.0;
			CYangle = 0.0;
			CZangle = 0.0;
			break;
		case 'v':
			Orthographic = true;
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
			glMatrixMode(GL_MODELVIEW);
			break;
		case 'V':
			Orthographic = false;
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
			glMatrixMode(GL_MODELVIEW);
			break;
		case 'f':
			glDisable(GL_FOG);
			break;
		case 'F':
			glEnable(GL_FOG);
			break;
		case 'w':
			ofstream output;
			output.open("output.obj");
			unsigned int v = 0;
			while(v < vertex_data.size()){
				output << 'v' << ' ' <<	vertex_data[v] << ' ' << vertex_data[v+1] << ' ' << vertex_data[v+2] << endl;
				v += 3;
			}
			unsigned int f = 0;
			while(f < faces_data.size()){
				v = 0;
				output << 'f';
				while(v < faces_data[f].size()){
					output << ' ' << faces_data[f][v];
					v++;
				}
				output << endl;
				f++;
			}
			output.close();
		   break;
	}
	glutPostRedisplay();
}

void specialKeyInput(int key, int x, int y) {
   if(key == GLUT_KEY_UP) Yvalue += 0.1;
   if(key == GLUT_KEY_DOWN) Yvalue -= 0.1;
   if(key == GLUT_KEY_LEFT) Xvalue -= 0.1;
   if(key == GLUT_KEY_RIGHT) Xvalue += 0.1;
   glutPostRedisplay();
}

void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if(Orthographic == true) {
		glOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
   }
   else {
		glFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, 100.0);
   }
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

int main(int argc, char **argv) {
   printInteraction();
   glutInit(&argc, argv);
   glutInitContextVersion(3, 0);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("modelviewer.cpp");
   loadFile(argc, argv); 
   setup();
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   
   glewExperimental = GL_TRUE;
   glewInit();
   glutMainLoop();
   return 0;
}