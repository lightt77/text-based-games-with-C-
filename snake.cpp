//
#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<time.h>
#include<set>
#include<deque>

using namespace std;

enum direction {STOP,UP,RIGHT,DOWN,LEFT};

class Snake
{
private:
	int x,y,posX,posY;
	set<pair<int,int>> occupied;
	deque<pair<int,int>> body;
	direction dir,lastdir;
	bool grow;

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
		//speed=1;
		dir=STOP;
		lastdir=UP;
		//extend(x,y);
		extend(x+1,y);
		grow=false;
		//
		//extend(x+1,y);
	}

	~Snake()
	{
		cout<<"Destructor called";
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
					//
					//else
						//extend(((body.rbegin())->first)+1,((body.rbegin())->second));
					break;
		case DOWN:	extend(posX,posY);
					posX++;
					if(grow==false)						//snake size stays the same
					{
						occupied.erase(*body.begin());
						body.pop_front();
					}
					//else
						//extend(((body.rbegin())->first)-1,((body.rbegin())->second));
					break;
		case LEFT:	extend(posX,posY);
					posY--;		
					if(grow==false)						//snake size stays the same
					{
						occupied.erase(*body.begin());
						body.pop_front();
					}	
					//else
						//extend(((body.rbegin())->first)+1,((body.rbegin())->second));
					break;
		case RIGHT:	extend(posX,posY);
					posY++;
					if(grow==false)						//snake size stays the same
					{
						occupied.erase(*body.begin());
						body.pop_front();
					}
					//else
						//extend(((body.rbegin())->first)+1,((body.rbegin())->second));
					break;
		}
	}

	bool isOccupied(int x,int y)					//
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

	/*inline int getSpeed()
	{
		return speed;
	}*/

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

	/*inline void setSpeed(int s)
	{
		speed=s;
	}*/

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
		posX=rand()%arenaWidth;
		posY=rand()%arenaHeight;
		this->s=s;
		//
		generate();

	}
	
	void generate()
	{
		srand(time(0));
		while(s->isOccupied(posX,posY) || (s->getX()==posX && s->getY()==posY))
		{
			posX=rand()%arenaWidth;
			posY=rand()%arenaHeight;
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
	bool quit;

public:
	Game(int x,int y)
	{
		s=new Snake(x/2,y/2);
		f=new Food(x,y,s);
		arenaWidth=x;
		arenaHeight=y;
		f->generate();
		quit=false;
		score=0;
	}

	void draw()
	{
		for(int i=0;i<=arenaWidth;i++)
		{
			for(int j=0;j<=arenaHeight;j++)
			{
				if(i==0 || j==0 || i==arenaWidth || j==arenaHeight)
					cout<<"#";
				else if(i==s->getX() && j==s->getY())
					cout<<"X";
				else if(s->isOccupied(i,j))
					cout<<"0";
				else if(i==f->getX() && j==f->getY())
					cout<<"*";
				else
					cout<<" ";
				//
				//Sleep(20);
			}
			cout<<endl;
		}
		cout<<"Score:"<<score; 
	}
	
	void move()
	{
		char ch='c';

		if(kbhit())
		{
			ch=getch();
		}

		if(s->getX()==f->getX() && s->getY()==f->getY())
		{
			s->setgrow(true);
		}

		switch(ch)
		{
		case 'c':	//
					//s->move(true);
					s->move();
					break;
		case 'w':	if(s->getDirection()!=DOWN)
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
		case 'q':	quit=true;
					break;
		case 'p':	s->setDirection(STOP);
					s->move();
					break;
		}

		if(s->getGrow()==true)
		{
			s->setgrow(false);
			score+=10;
			f->generate();
		}
	}

	void check()
	{
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
		cout<<"Game destructor called!!"<<endl;
		delete s;
		delete f;
	}
};

int main()
{
	
	Game g(40,40);
	int level=1,base_speed=500;

	cout<<"Choose a level:(1-10)";
	cin>>level;

	if(level>0 && level<11)
		base_speed/=level;
	else
		base_speed=0;					//rebels wont be dissappointed

	while(1)
	{
		if(g.getQuit())
			break;
		system("cls");
		g.draw();
		g.move();
		g.check();
		Sleep(base_speed);
	}
	
	cout<<endl<<endl<<"\t\t\t"<<"   GAME OVER!!"<<endl<<"\t\t\tYour Score is "<<g.getScore();
	while(1);
	return 0;
}