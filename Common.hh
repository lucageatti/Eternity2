#ifndef COMMON_HH
#define COMMON_HH

#include <stdint.h>
#include <vector>

using namespace std;

typedef int ID;

typedef int Color;

typedef int Orientation; // {0..3}

typedef pair<ID,Orientation> IDO;

typedef int CardinalPoint; // for the "getColor" function

typedef pair<unsigned,unsigned> Coord;

struct Tile {
  ID id;
  vector<Color> cardinals; // 0.Sud - 1.Ovest - 2.Nord - 3.Est
};


#endif
