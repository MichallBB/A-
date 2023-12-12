#include <iostream>
#include <fstream>
#include<algorithm>
#include<vector>
using namespace std;

struct Point{
    int x,y;
    int type;
    int distance;
    Point *parent;
    int state; // 0 nie przeanalizowane 1-do analizy 2-przeanalizowane
};

Point** load_map_from_file(string file, int sizeY, int sizeX){
    ifstream fin(file.c_str());
    Point **map_result = new Point*[sizeY];
    for(int i=0; i<sizeY; ++i){
        map_result[i] = new Point[sizeX];
        for(int j=0; j<sizeX; j++)
        {
            fin>>map_result[i][j].type;
            map_result[i][j].parent = 0;
            map_result[i][j].state = 0;
            map_result[i][j].y=i;
            map_result[i][j].x=j;
        }
    }
    return map_result;
}

void remove_point(vector<Point *> *vec, Point* point){
    std::vector<Point*>::iterator it = std::find(vec->begin(), vec->end(), point);
    if (it != vec->end())
        vec->erase(it);
}

int heuristic(int point1_x, int point1_y, int point2_x, int point2_y){
    return abs(point1_x - point2_x) + abs(point1_y - point2_y);
}

void a_star(Point **map, int sizeY, int sizeX, int startY, int startX, int targetY, int targetX){

    auto compare_point = [&](Point* const & lp, Point* const & rp) {
        return lp->distance + heuristic(lp->y, lp->x, targetY, targetX) > rp->distance + heuristic(rp->y, rp->x, targetY, targetX);
        };

    vector<Point*> vector;

    map[startY][startX].state=1;
    map[startY][startX].distance = 0;
    map[startY][startX].parent = 0;

    vector.push_back(&map[startY][startX]);

    while(!vector.empty()){
        Point* currentPoint = vector.front();
        currentPoint->state=2;
        if(currentPoint->y == targetY && currentPoint->x == targetX){
            break;
        }
        remove_point(&vector,currentPoint);



        int count_neighbours = 0;
        Point* neighbours[4];
        if(currentPoint->x + 1 < sizeX){
            neighbours[count_neighbours]=&map[currentPoint->y][currentPoint->x + 1];
            if(neighbours[count_neighbours]->state != 2 && neighbours[count_neighbours]->type != 5)
                count_neighbours++;
        }
        if(currentPoint->y - 1 >= 0){
            neighbours[count_neighbours]=&map[currentPoint->y -1 ][currentPoint->x];
            if(neighbours[count_neighbours]->state != 2 && neighbours[count_neighbours]->type != 5)
                count_neighbours++;
        }
        if(currentPoint->y + 1 < sizeY){
            neighbours[count_neighbours]=&map[currentPoint->y + 1 ][currentPoint->x];
            if(neighbours[count_neighbours]->state != 2 && neighbours[count_neighbours]->type != 5)
                count_neighbours++;
        }
        if(currentPoint->x - 1 >= 0){
            neighbours[count_neighbours]=&map[currentPoint->y][currentPoint->x - 1];
            if(neighbours[count_neighbours]->state != 2 && neighbours[count_neighbours]->type != 5)
                count_neighbours++;
        }


        for(int i=0; i<count_neighbours; i++){
                if(neighbours[i]->state == 1)
                    if(currentPoint->distance + 1 >= neighbours[i]->distance)
                        continue;
                    else
                        remove_point(&vector,neighbours[i]);
                neighbours[i]->parent = currentPoint;
                neighbours[i]->distance = currentPoint->distance + 1;
                neighbours[i]->state = 1;
                vector.push_back(neighbours[i]);
                push_heap(vector.begin(), vector.end(),compare_point);
        }
    }

    if(map[targetY][targetX].state == 2){
        Point* way = &map[targetY][targetX];

        do{
            way->type = 1;
        }while(way = way->parent);
    }
}

void show_map(Point ** map, int sizeY, int sizeX){
    for(int i=0; i<sizeY; i++){
        for(int j=0; j<sizeX; j++){
            cout<<map[i][j].type << " ";
        }
        cout<<endl;
    }
}

int main()
{
    Point **map;

    int sizeY = 20;
    int sizeX = 20;

    map = load_map_from_file("grid.txt", sizeY, sizeX);

    show_map(map, sizeY, sizeX);

    a_star(map,sizeY,sizeX,0,0,19,19);

    cout<<endl;
    show_map(map, sizeY, sizeX);
    return 0;
}







