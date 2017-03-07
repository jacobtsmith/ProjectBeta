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
    int run_q();   

    bool move(char direction);
    bool initialize();
    bool do_it_yourself;
    bool out_of_bound;
    bool at_goal (int x, int y);
    void test_d();
    void test_e();
    void test_f();
    
    void update_q(int x, int y);
    void initialize_q();
    void initialize_visited();
    double q_equation(double previous, double reward, double max_q);
    double max_q(q_learner);
    bool stuck();
    void print_q();

private:
    q_learner **q_values;
    int **dimensions;
    bool **visited;
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

   return previous + .1 * (reward + (.9 * max_q) - previous);
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


    visited = new bool*[m];
    for(int i = 0; i < m; ++i)
        visited[i] = new bool[n];
}

// initialize q values
void grid::initialize_q(){
    q_learner temp;
    srand (time(NULL));
    for (int i=0; i < m; ++i){
        for (int j=0; j < n; ++j){
            temp.up = JSRand/10000;
            temp.down = JSRand/10000;
            temp.left = JSRand/10000;
            temp.right = JSRand/10000;

            q_values[i] [j] = temp;
        }
    }
}


// Initializes each spot as not visited
void grid::initialize_visited() {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            visited[i][j] = false; 
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

    int r_up, r_down, r_left, r_right;
    double max;

    if (x-1 < 0)
        q_values [x] [y].up = -100;
    else {
        if (at_goal (x-1,y))
            r_up = m * n;
        else
            r_up = -1;

        max = max_q(q_values[x-1][y]);
        q_values [x] [y].up = q_equation (q_values [x] [y].up, r_up, max);
    }

    if (x+1 > n-1)
        q_values [x] [y].down = -100;
    else {
        if (at_goal (x+1,y))
            r_down = m * n;
        else
            r_down = -1;
        
        max = max_q(q_values[x+1][y]);
        q_values [x] [y].down = q_equation (q_values [x] [y].down, r_down, max);
    }

    if (y-1 < 0)
        q_values [x] [y].left = -100;
    else {
        if (at_goal (x,y-1))
            r_left = m * n;
        else
            r_left = -1;

        max = max_q(q_values[x][y-1]);
        q_values [x] [y].left = q_equation (q_values [x] [y].left, r_left, max);
    }

    if (y+1 > m-1) 
        q_values [x] [y].right = -100;
    else {
        if (at_goal (x,y+1))
            r_right = m * n;
        else
            r_right = -1;

        max = max_q(q_values[x][y+1]);
        q_values [x] [y].right = q_equation (q_values [x] [y].right, r_right, max);
    }
}

// Runs q learner
int grid::run_q(){
    bool end_flag = true;
    int num_steps = 0;
 
    while (end_flag) {
        update_q(agent_location[0],agent_location[1]);
        visited[agent_location[0]][agent_location[1]] = true;

        if (JSRand<.1){
             int rand_action = rand()%4;
            switch (rand_action){
                case 0:
                    
                    if(move('w') == true)
                        num_steps++;
                    break;
                case 1:
                    if(move('a') == true)
                        num_steps++;
                    break;
                case 2:
                    if(move('s') == true)
                        num_steps++;
                    break;
                case 3:
                    if(move('d') == true)
                        num_steps++;
                    break; 
                }
            }
        else{
            if (q_values[agent_location[0]][agent_location[1]].up == max_q(q_values[agent_location[0]][agent_location[1]])) {
                if(move('w') == true)
                    num_steps++;
            }
            else if (q_values[agent_location[0]][agent_location[1]].down == max_q(q_values[agent_location[0]][agent_location[1]])) {
                if(move('s') == true)
                    num_steps++;
            }
            else if (q_values[agent_location[0]][agent_location[1]].right == max_q(q_values[agent_location[0]][agent_location[1]])) {
                if(move('d') == true)
                    num_steps++;
            }
            else
                if(move('a') == true)
                    num_steps++;
       }
       
        if((agent_location[0] == goal_location[0]) && (agent_location[1] == goal_location[1])) {
            end_flag = false;
            break;
        }


        if(stuck() == true) {
            num_steps = -1;
            break;
        }
        
     }

    return num_steps;
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

bool grid::stuck(){
    bool up, down, left, right;

    if(agent_location[0] - 1 < 0)
        up = true;
    else if(visited[agent_location[0]-1][agent_location[1]] == true)
        up = true;
    else
        up = false;

    if(agent_location[0] + 1 >= m)
        down = true;
    else if(visited[agent_location[0]+1][agent_location[1]] == true)
        down = true;
    else
        down = false;

    if(agent_location[1] - 1 < 0)
        left = true;
    else if(visited[agent_location[0]][agent_location[1]-1] == true)
        left = true;
    else
        left = false;


    if(agent_location[1] + 1 >= n)
        right = true;
    else if(visited[agent_location[0]][agent_location[1]+1] == true)
        right = true;
    else
        right = false;


    return (up && down && right && left);


}
// test D
void grid::test_d(){
    initialize();
    initialize_q();
    run_q();
    bool result = true;
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            if (max_q(q_values[i][j]) > m*n + 1)
                result = false;
        }
    }
  
    if(result == true)
        cout << "Test D Results: No q value is greater than max reward value" << endl;
    else
        cout << "Test D failed" << endl;
}



// test E
void grid::test_e(){
    initialize();
    initialize_q();
    
    int initial_state[2];
    initial_state[0] = agent_location[0];
    initial_state[1] = agent_location[1];
    cout << "Initial grid state:" << endl;
    print_grid();
    cout << endl;


    cout << "Initial q values for moves adjacent to starting location:" << endl;
    cout << "UP: " << q_values[agent_location[0]][agent_location[1]].up << endl;
    cout << "DOWN: " << q_values[agent_location[0]][agent_location[1]].down << endl;
    cout << "LEFT: " << q_values[agent_location[0]][agent_location[1]].left << endl;
    cout << "RIGHT: " << q_values[agent_location[0]][agent_location[1]].right << endl;


    cout << "Running q learner..." << endl;
    run_q();
    cout << "Agent has reached goal state. Resetting agent position to initial position..." << endl;
    agent_location[0] = initial_state[0];
    agent_location[1] = initial_state[1];
    print_grid();
    cout << endl;

    cout << "Updated q values for moves adjacent to starting location after running q learner once:" << endl;
    cout << "UP: " << q_values[agent_location[0]][agent_location[1]].up << endl;
    cout << "DOWN: " << q_values[agent_location[0]][agent_location[1]].down << endl;
    cout << "LEFT: " << q_values[agent_location[0]][agent_location[1]].left << endl;
    cout << "RIGHT: " << q_values[agent_location[0]][agent_location[1]].right << endl;

}

// Test F
void grid::test_f(){

    initialize();
    initialize_q();
    
    int initial_state[2];
    initial_state[0] = agent_location[0];
    initial_state[1] = agent_location[1];
    print_grid();
    for(int i = 0; i < 1000; ++i) {
        initialize_visited();
        int steps = run_q();
        if(steps == -1){
            i--;
            agent_location[0] = initial_state[0];
            agent_location[1] = initial_state[1];
        }
        else {
            cout << "Q Learner Run " << i + 1 << ": Num steps = " << steps << endl;
            agent_location[0] = initial_state[0];
            agent_location[1] = initial_state[1];
        }
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
                return false;
            else if(visited[agent_location[0]-1][agent_location[1]] == true)
                return false;
            else {
                agent_location[0] = agent_location[0]-1;
                return true;
            }
        break;
        case 'a':
            if(agent_location[1]-1 < 0) 
                return false;
            else if(visited[agent_location[0]][agent_location[1]-1] == true)
                return false;
            else{
                agent_location[1] = agent_location[1]-1;
            }
        break;
        case 's':
            if(agent_location[0]+1 >= m) 
                return false;
            else if(visited[agent_location[0]+1][agent_location[1]] == true)
                return false;
            else{
                agent_location[0] = agent_location[0]+1;
            }
        break;
        case 'd':
            if(agent_location[1]+1 >= n) 
                return false;
            else if(visited[agent_location[0]][agent_location[1]+1] == true)
                return false;
            else{
                agent_location[1] = agent_location[1]+1;
            }
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

void grid::print_q() {
    for(int i = 0; i < m; ++i) {
        for(int j = 0; j < n; ++j) {
            cout << i << "," << j << endl;
            cout << "UP: " << q_values[i][j].up << endl;
            cout << "DOWN: " << q_values[i][j].down << endl;
            cout << "LEFT: " << q_values[i][j].left << endl;
            cout << "RIGHT: " << q_values[i][j].right << endl;

        }
    }

}

int main() {
    int m;
    int n;
    cout<<"enter rows and columns seperated by a space";
    cin>> m >> n;
    grid my_grid(m,n);
    
    my_grid.test_d();
    //my_grid.test_e();
    my_grid.test_f();
    //my_grid.print_q();

    return 0;
}

