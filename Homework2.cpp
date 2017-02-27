// Random set of all the includes I think I could ever need

#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>

// Namespace and something I had to define so it can be pulled later
using namespace std;

#define Grid

class grid{
public:
    grid(int m, int n);
    ~grid();
    void print_grid();
    void play_game();   

    bool move(char direction);
    bool initialize();
    bool do_it_yourself;
    bool out_of_bound;

private:
    int **dimensions;
    int agent_location[2];
    int goal_location[2];
    int m;
    int n;
};

// Creates grid
grid::grid(int row,int column){
    m = column;
    n = row;
    dimensions = new int*[m];
    for(int i = 0; i < m; ++i)
        dimensions[i] = new int[n];
}

// Intitializes the goal and agent locations
bool grid::initialize(){
    srand (time(NULL));
    agent_location [0] = rand()%m;
    agent_location [1] = rand()%n;
    goal_location [0] = rand()%m;
    goal_location [1] = rand()%n;
    while (goal_location[0] == agent_location[0] && goal_location[1] == agent_location[1]){
        goal_location [0] = rand()%m;
        goal_location [1] = rand()%n;      
    }
}

// Deletes old data
grid::~grid(){
    for(int i = 0; i < m; ++i)       
        delete[] dimensions[i];
    delete[] dimensions;
}

bool grid::move(char direction){
    switch (direction){
        case 'w':
            if(agent_location[0]-1 < 0)
                cout << "not a valid move dude" <<endl; 
            else
                agent_location[0] = agent_location[0]-1;
        break;
        case 'a':
            if(agent_location[1]-1 < 0)
                cout << "not a valid move dude" <<endl;
            else
                agent_location[1] = agent_location[1]-1;
        break;
        case 's':
            if(agent_location[0]+1 >= m)
                cout << "not a valid move dude" <<endl;
            else
                agent_location[0] = agent_location[0]+1;
        break;
        case 'd':
            if(agent_location[1]+1 >= n)
                cout << "not a valid move dude" <<endl;
            else
                agent_location[1] = agent_location[1]+1;
        break;

        default: cout<< "Invalid entry man";
        }
}

void grid::print_grid(){
    
        for (int i=0; i<m; ++i){
            for (int j=0; j<n; ++j){
                if  (i == agent_location[0] && j == agent_location[1])
                    cout<< "A ";
                else if (i == goal_location[0] && j ==goal_location[1])
                    cout<< "G ";
                else
                    cout<< "x ";
        }
        cout<< endl;

    }
}

void grid::play_game(){
    char choice;
    bool end = true;
    char input;
    cout<< "wanna play? y or n";
    cout<< endl;
    cin>> choice;

    switch (choice){
        case 'y':

            print_grid();
            while (end == true){
                cout<< "Make yer move w is up a is left s is down d is right ";
                cout<< endl;
                cin>> input;
                move(input);
            if (agent_location[0] == goal_location[0] && agent_location[1] == goal_location[1]){
                cout<< "ayyyy you did it";
                cout<< endl;
                end = false;
            }
            print_grid(); 
        }
        break;
           
        case 'n':

            print_grid();
            while (end == true) {
                if(agent_location[1] < goal_location[1])
                {
                    move('d');
                    print_grid();
                    cout << endl;
                }
                else if(agent_location[1] > goal_location[1])
                {
                    move('a');  
                    print_grid();
                    cout << endl;
                }
                else if(agent_location[0] < goal_location[0])
                {
                    move('s');
                    print_grid();
                    cout << endl;
                }
                else if(agent_location[0] > goal_location[0])
                {   
                    move('w');
                    print_grid();
                    cout << endl;
                }
                


                if((agent_location[0] == goal_location[0]) && (agent_location[1] == goal_location[1]))
                    end = false;
            }
       
             cout << endl << "Game over" << endl;
            break;
        default:
            cout<< "invalid choice";
   }
   }

int main() {
    int m;
    int n;
    cout<<"enter rows and columns seperated by a space";
    cin>> m >> n;
    grid my_grid(m,n);
    my_grid.initialize(); 

    my_grid.play_game();
 
    return 0;
}

