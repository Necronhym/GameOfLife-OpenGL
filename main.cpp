#include <iostream>
#include <vector>
#include <fstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
class Cell
	{
		bool status;
		bool newStatus;
		public:
		Cell(bool status){ this->status = status; }
		void setNewStatus(bool status) { this->newStatus = status; }
		void update() { this->status = this->newStatus; }
		bool isAlive() { return this->status; }
		void setAlive(bool alive) { this->status = alive; }
		friend std::ostream& operator<<(std::ostream& Out, Cell& C)
			{
				C.status? Out<<"██" : Out<<"  ";
				return Out;
			}
	};
class Tissue
	{
		std::vector <std::vector<Cell>> Cells;
		public:
		int x, y;
		Tissue(int x, int y)
			{
				this->x=x;
				this->y=y;
				for(int i=0; i<y; i++)
					{
						std::vector<Cell> C;
						for(int j=0; j<x; j++)
							{
								C.push_back(Cell(0));
							}
						Cells.push_back(C);
					}
			}
		void update()
			{
				for(int j=0; j<y; j++)
					{
						for(int i=0; i<x; i++)
							{
								int numLiving=0;
								int up = i-1;
								up<0?up=y-1:up;
								int down = i+1;
								down>y-1 ? down=0 : down;
								int right=j-1;		
								right<0 ? right=x-1 : right;
								int left=j+1;
								left>x-1 ? left=0 :left;
								numLiving+=Cells.at(left).at(up).isAlive();
								numLiving+=Cells.at(j).at(up).isAlive();
								numLiving+=Cells.at(right).at(up).isAlive();
								numLiving+=Cells.at(left).at(i).isAlive();
								numLiving+=Cells.at(right).at(i).isAlive();
								numLiving+=Cells.at(left).at(down).isAlive();
								numLiving+=Cells.at(j).at(down).isAlive();
								numLiving+=Cells.at(right).at(down).isAlive();
								if(numLiving<2)
									{
										Cells.at(j).at(i).setNewStatus(0);
									}
								if(numLiving==2 || numLiving==3 && Cells.at(j).at(i).isAlive())
									{
										Cells.at(j).at(i).setNewStatus(Cells.at(j).at(i).isAlive());
									}
								if(numLiving>3)
									{
										Cells.at(j).at(i).setNewStatus(0);
									}
								if(numLiving==3 && !Cells.at(j).at(i).isAlive())
									{
										Cells.at(j).at(i).setNewStatus(1);
									}

							}
					}
				for(int i=0; i<y; i++)
					{
						for(int j=0; j<x; j++)
							{
								Cells[j][i].update();
							}
					}
			}
		void animateCell(int x, int y)
			{
				Cells.at(y).at(x).setAlive(true);
			}
		bool isAlive(int x, int y)
			{
				return Cells.at(y).at(x).isAlive();
			}
		friend std::ostream& operator<<(std::ostream& Out, Tissue& T)
			{
				std::cout<<" ";
				for(int i=0; i<T.x*2; i++)
					{
						Out<<"─";
					}
				std::cout<<std::endl;
				for(auto a: T.Cells)
					{
						Out<<"|";
						for(auto b: a)
							{
								Out<<b;
							}
						Out<<"|"<<std::endl;
					}
				std::cout<<" ";
				for(int i=0; i<T.x*2; i++)
					{
						Out<<"─";
					}
				return Out;
			}
	};
Tissue* T;
int HEIGHT=640, WIDTH=640;
const char* NAME = "Game Of Life";
void init()
	{
		std::ifstream in("input.txt");
		std::string str;
		in>>str;
		T=new Tissue(str.length(), str.length());
		int row=0;
		int col=0;
		do
			{
				for(auto a = str.begin(); a!=str.end(); a++)
					{
						if(*a == '1')
							{
								T->animateCell(row,col);
							}
						row++;		
					}
				row=0;
				col++;
			}
		while(in>>str);
		glClearColor(0.3, 0.3, 0.3, 0.3);
	}
void display()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for(int j=0; j<T->y; j++)
			{
				for(int i=0; i<T->y; i++)
					{
						T->isAlive(i, j)? glColor3f(0.9,0.9,0.3) :glColor3f(0.3, 0.3, 0.3);
						glBegin(GL_POLYGON);
						glVertex2f(((float)i-(T->y/2))/(T->y/2), -(((float)j+1)-(T->x/2))/(T->x/2));
						glVertex2f((((float)i+1)-(T->y/2))/(T->y/2), -(((float)j+1)-(T->x/2))/(T->x/2));
						glVertex2f((((float)i+1)-(T->y/2))/(T->y/2), -((float)j-(T->x/2))/(T->x/2));
						glVertex2f(((float)i-(T->y/2))/(T->y/2), -((float)j-(T->x/2))/(T->x/2));
						
						glEnd();
					}
			}
		T->update();
		glutPostRedisplay();
		glutSwapBuffers();
	}
int main(int argc, char* argv[])
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(200, 200);
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow(NAME);
		init();
		glutDisplayFunc(display);
		glutMainLoop();
		return 0;
	}
