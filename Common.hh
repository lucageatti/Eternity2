#ifndef COMMON_HH
#define COMMON_HH

#define SOUTH 0
#define WEST 1
#define NORTH 2
#define EAST 3

#include <stdint.h>
#include <vector>
#include <easylocal/easylocal.hh>

using namespace std;

typedef int ID;

typedef int Color;

enum DFSColor { WHITE, GREY, BLACK };

typedef int Orientation; // {0..3}

typedef pair<ID,Orientation> IDO;

typedef int CardinalPoint; // for the "getColor" function

typedef pair<unsigned,unsigned> Coord;

typedef pair<IDO,Coord> tileMove;

struct Tile {
  ID id;
  vector<Color> cardinals; // 0.Sud - 1.Ovest - 2.Nord - 3.Est
};


#endif
