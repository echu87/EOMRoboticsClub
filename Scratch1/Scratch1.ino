#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irL(0);
IRControl irR(1);
MotorControl motL(1);
MotorControl motR(2);
UltrasonicControl sonic (13,12);
const int LEFT=1, UP=2, RIGHT=3, DOWN=0 , n =28;
boolean moveableSpot[4];
int dir, posX,posY,path[30] ;
void setup() 
{
    Serial.begin(9600);

    dir = UP;
    posX=0,posY=0;
    setNodes();
    
}


void loop() 
{  
//  Serial.print(irL.isBlack());
//  Serial.print("\t");
//  Serial.print(irR.isBlack());
//  Serial.print("\t");
//  Serial.println(sonic.getDistance());
  
  centering();
 
}


void centering(){
  
 if (irL.isBlack() == 0 && irR.isBlack() == 0)
  {
    motL.forward(90);
    motR.forward(90);
  }
  
  else if ((irL.isBlack() == 1) && (irR.isBlack() == 0)) //moving left
  {
    motL.forward(120);  //turn up left wheel to correct to the right
    motR.forward(70);
  }

   else if ((irL.isBlack() == 0) && (irR.isBlack() == 1))//moving right
  {
    motL.forward(70);  //turn up the right wheel to correct to the left
    motR.forward(120);
  }
  
   else if ((irL.isBlack() == 1) && (irR.isBlack() == 1))  //hit and intersection and stop!
  {


    stopMotors();
    node();
  }
 
  }

  void stopMotors() {
    motL.forward(0);
    motR.forward(0);
  }

void kickLeft() {
  while (irL.isBlack()) {
    motL.forward(0);
    motR.forward(90);
  }
}

void turnLeft() {
  int turnCounter = 0;
  kickLeft();
  while (turnCounter < 2) {
    motL.forward(0);
    motR.forward(90);
    if (irL.isBlack()) {
      kickLeft();
      turnCounter++;
    }
  }
  while (!irR.isBlack()) {
    motL.forward(70);
    motR.forward(0);
  }
}

void kickRight() {
  while (irR.isBlack()) {
    motR.forward(0);
    motL.forward(90);
  }
}

void turnRight() {
  int turnCounter = 0;
  kickRight();
  
  while (turnCounter < 1) {
    motR.forward(0);
    motL.forward(90);
    if (irR.isBlack()) {
      kickRight();
      turnCounter++;
    }
  }
  while (!irL.isBlack()) {
    motL.forward(0);
    motR.forward(70);
  }
}

void node()
{
  clearPresets();
 switch(dir){
    case UP: posY++;
    break;
    case DOWN: posY--;
    break;
    case RIGHT: posX++;
    break;
    case LEFT: posX--;
    }
    if(sonic.detect()){
    moveableSpot[dir] =false;
    }
    if(!moveableSpot[dir]){
      for(int i =1; i < 4; i++){
        if(moveableSpot[i]){
          turn(i);
          }
        }
      }else{
        turnRight();
        turnRight();
        
        }
}

void clearPresets(){
  for(int i =0; i < 4 ; i++)
  {
    moveableSpot[i] =true;  
  }
  switch(posX){
    case 0: moveableSpot[LEFT] =false;
    break;
    case 3: moveableSpot[RIGHT] = false;
    }
   switch(posY){
    case 6: moveableSpot[UP] =false;
    break;
    case 0: moveableSpot[DOWN] = false;
    }
  
  }
  
  void turn(int rotation){
    rotation = rotation - dir;
    if(rotation>0){
    turnRight();
    dir += rotation;
    }
    else if(rotation<0){
    turnLeft();
    dir += rotation;
    }
    else{
      turnRight();
      turnRight();
      if(dir+2 > 3){
        dir = dir+2 -3;
      }
        else
        dir += 2;
        }

    }

    

    

  void setNodes()
  {
      int graph[28][28] = {

                    {0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 0 
                    {1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 1
                    {0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 2
                    {0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 3
                    {1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 4
                    {0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 5
                    {0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 6
                    {0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 7
                    {0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 8
                    {0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 9
                    {0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 10
                    {0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0}, // Node 11
                    {0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0}, // Node 12
                    {0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0}, // Node 13
                    {0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0}, // Node 14
                    {0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0}, // Node 15
                    {0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0}, // Node 16
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0}, // Node 17
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0}, // Node 18
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,0,0,0,0}, // Node 19
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0,1,0,0,0}, // Node 20
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0}, // Node 21
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0}, // Node 22
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1}, // Node 23
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,0}, // Node 24
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0}, // Node 25
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1}, // Node 26
                    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0}  // Node 27
                  
                   };
          dijkstra(graph,0);
  }

//  for(int x = 0; x<n;x++){
//for(int i = 0;i<n;i++){
//  if(i%8==1){
//    graph[x][i]=1;
//  }else{
//    graph[x][i]=0;
//  }
  
  
//}
// } 




  

void dijkstra(int graph[n][n], int src)
{
    int dist[n]; 
    bool sptSet[n];
    int parent[n];
    for (int i = 0; i < n; i++)
    {
        parent[0] = -1;
        dist[i] = 9999;
        sptSet[i] = false;
    }
    dist[src] = 0;
    for (int count = 0; count < n - 1; count++)
    {
        int u = minDistance(dist, sptSet);
        sptSet[u] = true;
        for (int v = 0; v < n; v++)
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v] = u;
                dist[v] = dist[u] + graph[u][v];
            } 
    }
    printSolution(dist, n, parent);
}
int minDistance(int dist[], 
                bool sptSet[])
{
 
    int min = 9999, min_index;
 
    for (int v = 0; v < n; v++)
        if (sptSet[v] == false &&
                   dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;
}
void printPath(int parent[], int j)
{
     
    // Base Case : If j is source
    if (parent[j] == - 1)
        return;
 
    printPath(parent, parent[j]);

    Serial.print(j);
}

int printSolution(int dist[], int r, 
                      int parent[])
{
    int src = 0;
  Serial.print("G Path ");
  
       
        printPath(parent, 25);
    
}
 

    
  


