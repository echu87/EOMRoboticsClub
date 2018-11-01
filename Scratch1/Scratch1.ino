#include <IRControl.h>
#include <MotorControl.h>
#include <UltrasonicControl.h>

IRControl irL(0);
IRControl irR(1);
MotorControl motorRight(1);
MotorControl motorLeft(2);
UltrasonicControl sonic (13, 12);
const int LEFT = 1, UP = 2, RIGHT = 3, DOWN = 0 , n = 28;
int dir, path[7], counter = 0, nodePos = 0 ;
byte graph[n][n];
int  sonicman = 0;
int motrspeed= 110*.7;
int motlspeed = 120*.7;
void setup()
{
  Serial.begin(9600);

  for (int x = 0; x < n; x++) {

    if (x + 4 < 28) {
      graph[x][(x + 4)] = 1;
    }
    if (x + 1 < 28 && !((x + 1) % 4 == 0)  ) {
      graph[x][(x + 1)] = 1;
    }
    if (x - 4 >= 0 ) {
      graph[x][(x - 4)] = 1;
    }
    if (x - 1 >= 0  && !(x % 4 == 0) ) {
      graph[x][(x - 1)] = 1;
    }

  }


  dir = UP;

  dijkstra(graph, nodePos);

}


void loop()
{
  Serial.print(irL.isBlack());
  Serial.print("\t");
  Serial.print(irR.isBlack());
  Serial.print("\t");
  Serial.println(sonic.getDistance());


  centering();

}


void centering() {
  if (irL.isBlack() == 0 && irR.isBlack() == 0)
  {
    motorRight.forward(motrspeed);
    motorLeft.forward(motlspeed);
  }

  else if ((irL.isBlack() == 1) && (irR.isBlack() == 0)) //moving left
  {
    motorRight.forward(motrspeed);  //turn up left wheel to correct to the right
    motorLeft.reverse((motlspeed * .7));
  }

  else if ((irL.isBlack() == 0) && (irR.isBlack() == 1))//moving right
  {
    motorRight.reverse((motrspeed* .7));  //turn up the right wheel to correct to the left
    motorLeft.forward(motlspeed);
  }
  else if ((irL.isBlack() == 1) && (irR.isBlack() == 1))  //hit and intersection and stop!
  {
    motorRight.halt();  //turn up the right wheel to correct to the left
    motorLeft.halt();
    turnRight();
    //node();

  }

}


void kickLeft() {
  while (irL.isBlack()) {
     motorRight.reverse(80);
    motorLeft.forward(120);
  }
}
void turnRight() {
  //  int turnCounter = 0;
  //  kickLeft();
  //  while (turnCounter < 2) {
  //    motorRight.reverse(80);
  //    motorLeft.forward(90);
  //    if (irL.isBlack()) {
  //      kickLeft();
  //      turnCounter++;
  //    }
  //  }
  //  while (!irR.isBlack()) {
  //    motorRight.forward(70);
  //    motorLeft.reverse(60);
  //  }
  //
  //
  //  switch (dir) {
  //    case UP:
  //      dir = LEFT;
  //      break;
  //    case DOWN:
  //      dir = RIGHT;
  //      break;
  //    case RIGHT:
  //      dir = UP;
  //      break;
  //    case LEFT:
  //      dir = DOWN;
  //  }
  //  checkNode();
  for (long i = 0; i < 10000 ; i++) //Note: Needs to be long for 4 byte storage
  { //Rather than 2 byte storage of int
    motorRight.halt();
    motorLeft.halt();
  }

  for (long j = 0; j < 20000; j++)
  {
    motorRight.forward(160);
    motorLeft.forward(0);
  }
  kickRight();
  kickLeft();
}

void kickRight() {
  while (irR.isBlack()) {
    motorLeft.reverse(80);
    motorRight.forward(120);
  }
}

void turnLeft() {
  //  int turnCounter = 0;
  //  kickRight();
  //  while (turnCounter < 1) {
  //    motorLeft.reverse(60);
  //    motorRight.forward(90);
  //    if (irR.isBlack()) {
  //      kickRight();
  //      turnCounter++;
  //    }
  //  }
  //  while (!irL.isBlack()) {
  //    motorRight.reverse(30);
  //    motorLeft.forward(70);
  //  }
  //
  //  switch (dir) {
  //    case UP:
  //      dir = RIGHT;
  //      break;
  //    case DOWN:
  //      dir = LEFT;
  //      break;
  //    case RIGHT:
  //      dir = DOWN;
  //      break;
  //    case LEFT:
  //      dir = UP;
  //  }
  //  checkNode();

  for (long i = 0; i < 10000 ; i++) //Note: Needs to be long for 4 byte storage
  { //Rather than 2 byte storage of int
    motorRight.halt();
    motorLeft.halt();
  }

  for (long j = 0; j < 20000; j++)
  {
    motorRight.forward(0);
    motorLeft.forward(120);
  }


}

//void turnLeft() {
//  while (irL.isBlack() == 1 && irR.isBlack() == 1) {
//    motorRight.reverse(120);
//    motorLeft.reverse(120);
//
//  }
//
//  while (irL.isBlack() == 0 && irR.isBlack() == 0) {
//    motorRight.reverse(150);
//    motorLeft.reverse(150);
//
//  }
//
//  while (irL.isBlack() == 1 && irR.isBlack() == 1) {
//    motorRight.forward(85);
//
//  }
//  while (irL.isBlack() == 0 && irR.isBlack() == 1) {
//    motorRight.forward(85);
//
//
//  }
//  while (irL.isBlack() == 1 && irR.isBlack() == 0) {
//    motorRight.forward(85);
//
//
//  }
//  while (irL.isBlack() == 1 && irR.isBlack() == 1) {
//    motorRight.forward(85);
//
//  }
//
//  while (irL.isBlack() == 0 && irR.isBlack() == 0) {
//    motorRight.forward(85);
//
//
//  }
//
//  switch (dir) {
//    case UP:
//      dir = LEFT;
//      break;
//    case DOWN:
//      dir = RIGHT;
//      break;
//    case RIGHT:
//      dir = UP;
//      break;
//    case LEFT:
//      dir = DOWN;
//  }
//  checkNode();
//}
//
//void turnRight() {
//  while (irL.isBlack() == 0 && irR.isBlack() == 0) {
//    motorRight.reverse(125);
//    motorLeft.reverse(125);
//
//  }
//
//  while (irL.isBlack() == 1 && irR.isBlack() == 1) {
//    motorLeft.forward(83);
//
//  }
//  while (irL.isBlack() == 0 && irR.isBlack() == 1) {
//    motorLeft.forward(83);
//
//
//  }
//  while (irL.isBlack() == 1 && irR.isBlack() == 1) {
//    motorLeft.forward(83);
//
//  }
//
//  while (irL.isBlack() == 0 && irR.isBlack() == 0) {
//    motorLeft.forward(83);
//
//
//  }
//  switch (dir) {
//    case UP:
//      dir = RIGHT;
//      break;
//    case DOWN:
//      dir = LEFT;
//      break;
//    case RIGHT:
//      dir = DOWN;
//      break;
//    case LEFT:
//      dir = UP;
//  }
//  checkNode();
//}


void node()
{

  switch (dir) {
    case UP:
      nodePos += 4;
      break;
    case DOWN:
      nodePos -= 4;
      break;
    case RIGHT:
      nodePos++;
      break;

    case LEFT:

      nodePos--;

  }


  checkNode();

  if ((nodePos + 1) == path[0] && dir == RIGHT)
  {
    straight();
  }

  else if ((nodePos - 1) == path[0] && dir == RIGHT)
  {
    turnRight();
    turnRight();
     straight();
  }

  else if ((nodePos - 4) == path[0] && dir == RIGHT)
  {
    turnLeft();
     straight();
  }
  else if ((nodePos + 4) == path[0] && dir == RIGHT)
  {
    turnRight();
     straight();
  }

  else if ((nodePos + 1) == path[0] && dir == LEFT)
  {
    turnRight();
    turnRight();
     straight();
  }

  else if ((nodePos - 1) == path[0] && dir == LEFT)
  {
    straight();
  }

  else if ((nodePos - 4) == path[0] && dir == LEFT)
  {
    turnRight();
     straight();
  }
  else if ((nodePos + 4) == path[0] && dir == LEFT)
  {
    turnLeft();
     straight();
  }

  else if ((nodePos + 1) == path[0] && dir == UP)
  {

    turnRight();
   straight();
  }

  else if ((nodePos - 1) == path[0] && dir == UP)
  {
    turnLeft();
     straight();
  }

  else if ((nodePos + 4) == path[0] && dir == UP)
  {
    straight();
  }
  else if ((nodePos - 4) == path[0] && dir == UP)
  {
    turnRight();
    turnRight();
     straight();
  }

  else if ((nodePos + 1) == path[0] && dir == DOWN)
  {

    turnLeft();
     straight();

  }

  else if ((nodePos - 1) == path[0] && dir == DOWN)
  {
    turnRight();
     straight();
  }

  else if ((nodePos - 4) == path[0] && dir == DOWN)
  {
    turnRight();
    turnRight();
     straight();
  }
  else if ((nodePos + 4) == path[0] && dir == DOWN)
  {
    straight();

  }
  else {
    straight();
  }





}

void checkNode() {
  sonicman = 0;
  for (int i = 0; i <= 5; i++) {
    sonicman += sonic.getDistance();

  }
  sonicman = sonicman / 5;
  if (sonic.detect() ) {
    switch (dir) {
      case UP:
        graph[nodePos][(nodePos + 4)] = 0;
        graph[(nodePos + 4)][nodePos] = 0;
        break;
      case DOWN:
        graph[nodePos][(nodePos - 4)] = 0;
        graph[(nodePos - 4)][nodePos] = 0;
        break;
      case RIGHT:
        graph[nodePos][(nodePos + 1)] = 0;
        graph[(nodePos + 1)][nodePos] = 0;
        break;

      case LEFT:
        graph[nodePos][(nodePos - 1)] = 0;
        graph[(nodePos - 1)][nodePos] = 0;

    }
  }
  dijkstra(graph, nodePos);
}

void straight() {
  int stime = millis();
  while (stime <= stime + 100) {
    motorRight.forward(90);
    motorLeft.forward(90);
  }
}



void dijkstra(byte graph[n][n], int src)
{
  counter = 0;
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
  printSolution(parent);
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

  addToPath(j);

}

int printSolution(int parent[])
{
  int src = 0;
  Serial.print("Path ");
  printPath(parent, 27);

}
void addToPath(int j)
{
  path[counter] = j;
  counter++;

}







