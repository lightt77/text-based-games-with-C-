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
					cout<<"    |";
				else
				{
					cout<<matrix[i][j];
					if(matrix[i][j]<10)
						cout<<"   |";
					else if(matrix[i][j]<100)
						cout<<"  |";
					else if(matrix[i][j]<1000)
						cout<<" |";
					else if(matrix[i][j]<10000)
						cout<<"|";
				}
				//cout<<" ";
			}
			cout<<endl;
		}
		cout<<endl<<"Score : "<<score<<endl;
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

	void move(char ch)				//moves and combines tiles by value,according to the key pressed
	{
		if(key_pressed=='w')
		{
			for(int j=0;j<dimension;j++)		//move each tile column-wise in vertical direction starting with first column.
			{
				//first tile in a column moves up the first and then other tiles in the column follow
				for(int i=1;i<dimension;i++)
				{
					if(matrix[i][j]==0)					//if tile is vacant,do nothing
						continue;
					else if(matrix[i][j]==matrix[i-1][j])		//if tile value is equal to upper tile value,merge the two of them in to a tile of twice the value
					{
						matrix[i-1][j]*=2;
						vacant.insert(make_pair(i,j));
						matrix[i][j]=0;
						score+=matrix[i-1][j];
					}
					else if(matrix[i-1][j]==0)					//if upper tile is vacant
					{
						int p=i-1,q=j;

						while(inside_matrix(p,q) && matrix[p][q]==0)		//move upwards while upper tile is vacant,stop when upper tile is occupied or when board border is reached
						{
							p--;
						}

						if(!inside_matrix(p,q))						//move p into the board if it goes out of the board
							p++;

						if(matrix[p][q]==matrix[i][j])				//if value of vertically next occupied tile is equal to given tile,merge the two
						{
							matrix[p][q]*=2;
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
							score+=matrix[p][q];
						}
						else if(matrix[p][q]==0)					//if tile at the vertical extreme is vacant,shift current tile to its position
						{
							matrix[p][q]=matrix[i][j];
							vacant.erase(make_pair(p,q));
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
						}
						else                                       //if tile at vertical extreme is of different value,move the current tile to a position vertically below the extreme tile
						{
							p++;
							matrix[p][q]=matrix[i][j];
							vacant.insert(make_pair(i,j));
							vacant.erase(make_pair(p,q));
							matrix[i][j]=0;
						}
					}

				}
			}
		}
		else if(key_pressed=='s')
		{
			for(int j=0;j<dimension;j++)
			{
				for(int i=dimension-2;i>=0;i--)
				{
					if(matrix[i][j]==0)
						continue;
					else if(matrix[i][j]==matrix[i+1][j])
					{
						matrix[i+1][j]*=2;
						vacant.insert(make_pair(i,j));
						matrix[i][j]=0;
						score+=matrix[i+1][j];
					}
					else if(matrix[i+1][j]==0)
					{
						int p=i+1,q=j;

						while(inside_matrix(p,q) && matrix[p][q]==0)
						{
							p++;
						}

						if(!inside_matrix(p,q))
							p--;

						if(matrix[p][q]==matrix[i][j])
						{
							matrix[p][q]*=2;
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
							score+=matrix[p][q];
						}
						else if(matrix[p][q]==0)
						{
							matrix[p][q]=matrix[i][j];
							vacant.erase(make_pair(p,q));
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
						}
						else
						{
							p--;
							matrix[p][q]=matrix[i][j];
							vacant.insert(make_pair(i,j));
							vacant.erase(make_pair(p,q));
							matrix[i][j]=0;
						}
					}

				}
			}
		} 
		else if(key_pressed=='a')				//if left arrow is pressed,move left most tile in a row first
		{
			for(int i=0;i<dimension;i++)
			{
				for(int j=1;j<dimension;j++)
				{
					if(matrix[i][j]==0)
						continue;
					else if(matrix[i][j]==matrix[i][j-1])
					{
						matrix[i][j-1]*=2;
						vacant.insert(make_pair(i,j));
						matrix[i][j]=0;
						score+=matrix[i][j-1];
					}
					else if(matrix[i][j-1]==0)
					{
						int p=i,q=j-1;

						while(inside_matrix(p,q) && matrix[p][q]==0)
						{
							q--;
						}

						if(!inside_matrix(p,q))
							q++;

						if(matrix[p][q]==matrix[i][j])
						{
							matrix[p][q]*=2;
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
							score+=matrix[p][q];
						}
						else if(matrix[p][q]==0)
						{
							matrix[p][q]=matrix[i][j];
							vacant.erase(make_pair(p,q));
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
						}
						else
						{
							q++;
							matrix[p][q]=matrix[i][j];
							vacant.insert(make_pair(i,j));
							vacant.erase(make_pair(p,q));
							matrix[i][j]=0;
						}
					}

				}
			}
		}
		else if(key_pressed=='d')
		{
			for(int i=0;i<dimension;i++)
			{
				for(int j=dimension-2;j>=0;j--)
				{
					if(matrix[i][j]==0)
						continue;
					else if(matrix[i][j]==matrix[i][j+1])
					{
						matrix[i][j+1]*=2;
						vacant.insert(make_pair(i,j));
						matrix[i][j]=0;
						score+=matrix[i][j+1];
					}
					else if(matrix[i][j+1]==0)
					{
						int p=i,q=j+1;

						while(inside_matrix(p,q) && matrix[p][q]==0)
						{
							q++;
						}

						if(!inside_matrix(p,q))
							q--;

						if(matrix[p][q]==matrix[i][j])
						{
							matrix[p][q]*=2;
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
							score+=matrix[p][q];
						}
						else if(matrix[p][q]==0)
						{
							matrix[p][q]=matrix[i][j];
							vacant.erase(make_pair(p,q));
							vacant.insert(make_pair(i,j));
							matrix[i][j]=0;
						}
						else
						{
							q--;
							matrix[p][q]=matrix[i][j];
							vacant.insert(make_pair(i,j));
							vacant.erase(make_pair(p,q));
							matrix[i][j]=0;
						}
					}

				}
			}
		} 

	}

	bool getQuit()							//returns the quit flag
	{
		return quit;
	}

	bool move_possible()					//checks if any move is possible,this part is buggy
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

	void gameover_check()					//checks if game is over
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
	g_2048 *g=new g_2048(3);

	game_loop(g);

	return 0;
}