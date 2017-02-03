/****************************************************************************************************************************
														2048 game
****************************************************************************************************************************/

#include<iostream>
#include<time.h>
#include<Windows.h>
#include<conio.h>
#include<set>

using namespace std;

class g_2048
{
private:
	int dimension;
	int **matrix;
	int score;
	bool quit;
	char key_pressed;
	set<pair<int,int> > vacant;
	int probof2,probof4;

public:
	g_2048(int dim)
	{
		dimension=dim;
		matrix=new int*[dim];
		for(int i=0;i<dim;i++)
		{
			matrix[i]=new int[dim];
			fill(matrix[i],matrix[i]+dim,0);
			for(int j=0;j<dimension;j++)
			{
				vacant.insert(make_pair(i,j));
			}
		}
		score=0;
		key_pressed='C';
		quit=false;
		probof2=90;
		probof4=100-probof2;
	}

	~g_2048()
	{
		delete matrix;
	}

	void rand_number_gen()
	{
		if(vacant.size()==0)
		{
			return;
		}

		srand(time(0));

		int posX=rand()%dimension,posY=rand()%dimension;
		
		while(matrix[posX][posY]!=0)
		{
			posX=rand()%dimension;
			posY=rand()%dimension;
		}

		int r=rand()%100;

		if(r<=probof2)
			matrix[posX][posY]=2;
		else
			matrix[posX][posY]=4;

		vacant.erase(make_pair(posX,posY));
	}

	void game_display()
	{
		//
		system("cls");
		for(int i=0;i<dimension;i++)
		{
			for(int j=0;j<dimension;j++)
			{
				if(matrix[i][j]==0)
					cout<<" ";
				else
					cout<<matrix[i][j];
				cout<<" ";
			}
			cout<<endl;
		}
		cout<<"Score : "<<score<<endl;
	}

	void get_input()
	{
		while(!kbhit());
		key_pressed=getch();
	}

	void logic()
	{
		switch(key_pressed)
		{
		case 'q':	quit=true;
					return;
		case 'w':	move('w');
					return;
		case 'a':	move('a');
					return;
		case 's':	move('s');
					return;
		case 'd':	move('d');
					return;
		default:	return;
		}
	}

	inline bool inside_matrix(int i,int j)
	{
		return (i>=0 && i<dimension && j>=0 && j<dimension);
	}

	void move(char ch)				//note:remember to shift some of the functions to private
	{
		if(ch=='w')
		{
			for(int i=dimension-1;i>=1;i--)
			{
				bool breakouterloop=false;
				for(int j=0;j<dimension;j++)
				{
					if(matrix[i][j]==0)
						continue;
					if(matrix[i-1][j]==0)
					{
						vacant.erase(make_pair(i-1,j));
						matrix[i-1][j]=matrix[i][j];
						vacant.insert(make_pair(i,j));
						matrix[i][j]=0;
					}
					else if(matrix[i-1][j]==matrix[i][j])
					{
						matrix[i-1][j]*=2;
						score+=matrix[i-1][j];
						matrix[i][j]=0;
						vacant.insert(make_pair(i,j));
						breakouterloop=true;
					}
				}
				if(breakouterloop)
					break;
			}
		}
		

	}

	bool getQuit()
	{
		return quit;
	}

	bool move_possible()		//test this
	{
		if(vacant.size()==0)
		{
			for(int i=0;i<dimension-1;i++)
			{
				for(int j=0;j<dimension-1;j++)
				{
					if(matrix[i][j]==matrix[i][j+1] || matrix[i][j]==matrix[i+1][j])
						return true;
				}
			}
			if(matrix[dimension-1][dimension-1]==matrix[dimension-1][dimension-2] || matrix[dimension-1][dimension-1]==matrix[dimension-2][dimension-1])
				return true;
		}
		return true;
	}

	void gameover_check()
	{
		if(move_possible()==false)
		{
			quit=true;
		}
	}
};

void game_loop(g_2048 *g)
{
	g->rand_number_gen();
	while(1)
	{
		g->gameover_check();
		if(g->getQuit()==true)
			return;
		g->game_display();
		g->get_input();
		g->logic();
		g->game_display();
		Sleep(500);
		g->rand_number_gen();
	}
}

int main()
{
	g_2048 *g=new g_2048(4);

	game_loop(g);

	return 0;
}