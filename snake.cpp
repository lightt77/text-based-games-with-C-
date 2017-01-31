/**************************************************************************************************************************
													TEXT BASED SNAKE GAME
This game has been tested on Windows 7 with gcc compiler(version 5.3.0)
For proper view,set command prompt window size to 75x45
***************************************************************************************************************************/

#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<set>
#include<deque>
#include<stdio.h>

using namespace std;

enum direction {STOP,UP,RIGHT,DOWN,LEFT};

class Snake
{
private:
	int x,y,posX,posY;
	set<pair<int,int> > occupied;			//set of coordinates occupied by snake's body
	deque<pair<int,int> > body;
	direction dir,lastdir;
	bool grow;								//flag to indicate if the snake should grow in size or not

	void extend(int x,int y)				//extends the snake body in the direction of head
	{
		occupied.insert(make_pair(x,y));
		body.push_back(make_pair(x,y));
	}

public:
	Snake(int x,int y)
	{
		posX=x;
		posY=y;
		dir=STOP;
		lastdir=UP;
		extend(x+1,y);
		grow=false;
	}

	void move()							//moves the snake in the direction specified
	{
		switch(dir)
		{
		case STOP:	break;
		case UP:	extend(posX,posY);
					posX--;
					if(grow==false)						//snake size stays the same
					{
						occupied.erase(*body.begin());
						body.pop_front();
					}
					break;
		case DOWN:	extend(posX,posY);
					posX++;
					if(grow==false)						//snake size stays the same
					{
						occupied.erase(*body.begin());
						body.pop_front();
					}
					break;
		case LEFT:	extend(posX,posY);
					posY--;		
					if(grow==false)						//snake size stays the same
					{
						occupied.erase(*body.begin());
						body.pop_front();
					}	
					break;
		case RIGHT:	extend(posX,posY);
					posY++;
					if(grow==false)						//snake size stays the same
					{
						occupied.erase(*body.begin());
						body.pop_front();
					}
					break;
		}
	}

	bool isOccupied(int x,int y)						//checks if coordinate (x,y) is occupied by snake's body
	{
		if(occupied.find(make_pair(x,y))==occupied.end())
			return false;
		return true;
	}

	bool getGrow()		
	{
		return grow;
	}

	void setgrow(bool f)
	{
		grow=f;
	}

	inline direction getDirection()
	{
		return dir;
	}

	inline int getX()
	{
		return posX;
	}

	inline int getY()
	{
		return posY;
	}

	inline void setDirection(direction d)
	{
		dir=d;
	}

	inline void setX(int x)
	{
		posX=x;
	}

	inline void setY(int y)
	{
		posY=y;
	}
};

class Food
{
private:
	int posX,posY,arenaWidth,arenaHeight;
	Snake *s;
	
public:
	Food(int x,int y,Snake *s)
	{
		srand(time(0));
		arenaWidth=x;
		arenaHeight=y;
		posX=rand()%(arenaWidth-1)+1;
		posY=rand()%(arenaHeight-1)+1;
		this->s=s;
		generate();
	}
	
	void generate()									//produces food at a random location on the arena
	{
		srand(time(0));
		while(s->isOccupied(posX,posY) || (s->getX()==posX && s->getY()==posY))		//so that food is not generated on snake's body
		{
			posX=rand()%(arenaWidth-1)+1;
			posY=rand()%(arenaHeight-1)+1;
		}
	}
	
	inline int getX()
	{
		return posX;
	}
	
	inline int getY()
	{
		return posY;
	}
};

class Game
{
private:
	Snake *s;
	Food *f;
	int arenaWidth,arenaHeight;
	int score;									
	bool quit;									//set if 'q' key is pressed

public:
	Game(int x,int y)
	{
		s=new Snake(x/2,y/2);					//initialise snake at the center of arena
		f=new Food(x,y,s);						//produce food at a random location on arena
		arenaWidth=x;
		arenaHeight=y;
		f->generate();
		quit=false;
		score=0;
	}

	void draw()											//draws pixels on the arena
	{
		for(int i=0;i<=arenaWidth;i++)
		{
			for(int j=0;j<=arenaHeight;j++)
			{
				if(i==0 || j==0 || i==arenaWidth || j==arenaHeight)		//arena boundary
					cout<<"#";
				else if(i==s->getX() && j==s->getY())					//snake head
					cout<<"X";
				else if(s->isOccupied(i,j))								//snake body
					cout<<"0";
				else if(i==f->getX() && j==f->getY())					//food
					cout<<"*";
				else
					cout<<" ";
				//
				//Sleep(20);								//frame delay,introduces flickering when set to 0ms,but inproves game play 
			}
			cout<<endl;
		}
		cout<<"Score: "<<score; 
	}
	
	void move()
	{
		char ch='c';									//by default,continue.Also,implements unpause on any key pressed after pausing

		if(kbhit())										//checks if any key is pressed
		{
			ch=getch();
		}

		if(s->getX()==f->getX() && s->getY()==f->getY())	//checks if snake has eaten food
		{
			s->setgrow(true);
		}

		switch(ch)										//action to be performed on various key presses
		{
		case 'c':	s->move();							//default
					break;
		case 'w':	if(s->getDirection()!=DOWN)			//so that snake doesnt make a 180-degree turn
						s->setDirection(UP);
					s->move();
					break;
		case 'a':	if(s->getDirection()!=RIGHT)
						s->setDirection(LEFT);
					s->move();
					break;
		case 's':	if(s->getDirection()!=UP)
						s->setDirection(DOWN);
					s->move();
					break;
		case 'd':	if(s->getDirection()!=LEFT)
						s->setDirection(RIGHT);
					s->move();
					break;
		case 'q':	quit=true;							//quit game
					break;
		case 'p':	s->setDirection(STOP);				//pause game
					s->move();
					break;
		}

		if(s->getGrow()==true)							//check if snake has eaten food
		{
			s->setgrow(false);							//so that snake doesnt grow indefinitely
			score+=10;									//update score
			f->generate();								//generate food at relevant location,as food is now eaten
		}
	}

	void gameover_check()								//update quit flag if game is over
	{
		//check if snake head collides with walls of arena or the snake's body
		if(s->getX()==0 || s->getY()==0 || s->getX()==arenaWidth || s->getY()==arenaHeight || s->isOccupied(s->getX(),s->getY()))
			quit=true;
	}

	bool getQuit()
	{
		return quit;
	}

	int getScore()
	{
		return score;
	}

	~Game()
	{
		delete s;
		delete f;
	}
};

void instructions_page()					//displays the indtruction page
{
	system("cls");
	cout<<"Controls:"<<endl<<"W : UP"<<endl<<"A : LEFT"<<endl<<"S : DOWN"<<endl<<"D : RIGHT"<<endl<<"P : PAUSE"<<endl<<"ANY KEY : UNPAUSE"<<endl;
	cout<<"Press any key to start the game!!"<<endl;
	getch();
}

void gameloop(Game *g)
{
	int level,base_frame_delay=500;
	
	fflush(stdin);							//clears input buffer if more than one character was inputted

	system("cls");
	cout<<"Choose a level:(1-10)"<<endl;
	cin>>level;

	fflush(stdin);							//clears input buffer if more than one character was inputted
	
	instructions_page();
	if(level>0 && level<11)
		base_frame_delay/=level;
	else
		base_frame_delay=0;					//if any other level is chosen,sets speed to maximum

	while(1)
	{
		if(g->getQuit())
			break;
		system("cls");
		g->draw();
		g->move();
		g->gameover_check();
		Sleep(base_frame_delay);
	}
}

int main()
{
	Game *g;
	while(1)
	{
		g=new Game(40,40);
		gameloop(g);
		cout<<endl<<endl<<"\t\t\t"<<"   GAME OVER!!"<<endl<<"\t\t\tYour Score is "<<g->getScore();
		cout<<endl<<"\t\tpress 'R' to Restart,'E' to Exit"<<endl;
		
		fflush(stdin);							//clears input buffer if more than one character was inputted
		char ch;
		cin>>ch;
		if(ch=='E' || ch=='e')
		{
			delete g;
			break;
		}
	}
	return 0;
}