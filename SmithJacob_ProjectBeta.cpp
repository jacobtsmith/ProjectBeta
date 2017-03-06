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

#define JSRand (double)rand()/RAND_MAX

struct q_learner{

    double up;
    double left;
    double down;
    double right;
};

class grid{
public:
    grid(int m, int n);
    ~grid();
    void print_grid();
    void play_game();
    void run_q();   

    bool move(char direction);
    bool initialize();
    bool do_it_yourself;
    bool out_of_bound;
    bool at_goal (int x, int y);

    void update_q(int x, int y);
    void initialize_q();
    double q_equation(double previous, double reward, double max_q);
    double max_q(q_learner);

private:
    q_learner **q_values;
    int **dimensions;
    int agent_location[2];
    int goal_location[2];
    int m;
    int n;
};

// max q function

double grid::max_q(q_learner values){
    double max = values.up;
    if (values.left>max)
        max= values.left;
    if (values.right>max)
        max= values.right;
    if (values.down>max)
        max= values.down;

    return max;
}

// q equation function

double grid::q_equation(double previous, double reward, double max_q){
    return previous + .1 * (reward + .9 * max_q - previous);
}

// Creates grid and q values
grid::grid(int row,int column){
    m = column;
    n = row;
    dimensions = new int*[m];
    for(int i = 0; i < m; ++i)
        dimensions[i] = new int[n];

    q_values = new q_learner*[m];
    for(int i = 0; i < m; ++i)
        q_values[i] = new q_learner[n];
}

// initialize q values
void grid::initialize_q(){
    q_learner temp;
    srand (time(NULL));
    for (int i=0; i < m; ++i){
        for (int j=0; j < n; ++j){
            temp.up = ((double) rand() / (RAND_MAX)) / 1000;
            temp.down = ((double) rand() / (RAND_MAX)) / 1000;
            temp.left = ((double) rand() / (RAND_MAX)) / 1000;
            temp.right = ((double) rand() / (RAND_MAX)) / 1000;

            q_values[i] [j] = temp;
        }
    }
}

// asigns rewards

bool grid::at_goal (int x, int y){
    if (x == goal_location[0] && y == goal_location[1])
        return true;
    else
        return false;
}

// update q values
void grid::update_q (int x, int y){
    double max = max_q (q_values [x] [y]);
    int r_up, r_down, r_left, r_right;
    if (y-1 < 0)
        q_values [x] [y].up = 0;
    else {
        if (at_goal (x,y-1))
            r_up = m * n;
        else
            r_up = -1;
        q_values [x] [y].up = q_equation (q_values [x] [y].up, r_up, max);
    }

    if (y+1 > n-1)
        q_values [x] [y].down = 0;
    else {
        if (at_goal (x,y+1))
            r_down = m * n;
        else
            r_down = -1;
        q_values [x] [y].down = q_equation (q_values [x] [y].down, r_down, max);
    }

    if (x-1 < 0)
        q_values [x] [y].left = 0;
    else {
        if (at_goal (x-1,y))
            r_left = m * n;
        else
            r_left = -1;
        q_values [x] [y].left = q_equation (q_values [x] [y].left, r_left, max);
    }

    if (x+1 > m-1)
        q_values [x] [y].right = 0;
    else {
        if (at_goal (x+1,y))
            r_right = m * n;
        else
            r_right = -1;
        q_values [x] [y].right = q_equation (q_values [x] [y].right, r_right, max);
    }
}

// Runs q learner
void grid::run_q(){
    while (agent_location[0] != goal_location[0] && agent_location[1] != goal_location[1]){
        update_q (agent_location[0], agent_location[1]);
        if (JSRand<.01){
             int rand_action = rand()%4;
            switch (rand_action){
                case 0:
                move('w');
                break;
                case 1:
                move('a');
                break;
                case 2:
                move('s');
                break;
                case 3:
                move('d');
                break; 
                }
            }
        else{
            if (q_values[agent_location[0]][agent_location[1]].up == max_q(q_values[agent_location[0]][agent_location[1]]))
                move('w');
            else if (q_values[agent_location[0]][agent_location[1]].down == max_q(q_values[agent_location[0]][agent_location[1]]))
                move('s');
            else if (q_values[agent_location[0]][agent_location[1]].right == max_q(q_values[agent_location[0]][agent_location[1]]))
                move('d');
            else
                move('a');
            }
     }
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

    for(int i = 0; i < m; ++i)       
        delete[] q_values[i];
    delete[] q_values;
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

