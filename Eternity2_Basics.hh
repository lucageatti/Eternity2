// File Eternity2_Basics.hh
#ifndef Eternity2_BASICS_HH
#define Eternity2_BASICS_HH

#include "Eternity2_Data.hh"

using namespace EasyLocal::Core;

class Eternity2_State
{  
  friend ostream& operator<<(ostream& os, const Eternity2_State& st);
  friend bool operator==(const Eternity2_State& st1, const Eternity2_State& st2);
public:
  Eternity2_State(const Eternity2_Input &in);
  Eternity2_State& operator=(const Eternity2_State& s);
  IDO getIDOAt(Coord crd) const { return board.at(crd.first).at(crd.second); }
  unsigned getHeight() const { return board.size(); }
  unsigned getWidth() const { return board.at(0).size(); }
  void insertTile(IDO ido, Coord crd);
  Color getColor(IDO ido, CardinalPoint pc) const;
  //Coordinates for the moves
  vector<Coord> even_chessboard;
  vector<Coord> odd_chessboard;
  vector<Coord> random_singleton;
  vector<pair<Coord,int> > random_tts;
protected:
  const Eternity2_Input & in;
  vector<vector<IDO> > board;
  int strangeMod(int dividend, int divisor) const;
};



class Eternity2_GenericMove
{
  friend bool operator==(const Eternity2_GenericMove& m1, const Eternity2_GenericMove& m2);
  friend bool operator!=(const Eternity2_GenericMove& m1, const Eternity2_GenericMove& m2);
  friend bool operator<(const Eternity2_GenericMove& m1, const Eternity2_GenericMove& m2);
  friend ostream& operator<<(ostream& os, const Eternity2_GenericMove& c);
  friend istream& operator>>(istream& is, Eternity2_GenericMove& c);
  public:
    Eternity2_GenericMove();
    //Setters
    void setIndex(unsigned i, unsigned index) { permutation.at(i).first = index; }
    void setOrientation(unsigned i, Orientation orient ) { permutation.at(i).second = orient; }
    void setCoordinates(vector<Coord> crds) { coords = crds; }
    void createPermutationVector(unsigned size) { permutation = vector<pair<unsigned,Orientation> >(size); }
    //Getters
    unsigned getIndexAt(unsigned i) const { return permutation.at(i).first; }
    Orientation getOrientationAt(unsigned i) const { return permutation.at(i).second; }
    vector<Coord> getCoordinates() const { return coords; }
    unsigned getSize() const { return coords.size(); }
    void swap(unsigned i, unsigned j) { std::swap(permutation.at(i), permutation.at(j)); }
  protected:
    //Vector of (index,Orientation)
    vector<pair<unsigned,Orientation> > permutation;
    //Vector of Coordinates
    vector<Coord> coords;
};




class Eternity2_SingletonMove : public Eternity2_GenericMove
{
  public:
    Eternity2_SingletonMove();  
};


class Eternity2_EvenChessboardMove : public Eternity2_GenericMove
{
  public:
    Eternity2_EvenChessboardMove();
};


class Eternity2_OddChessboardMove : public Eternity2_GenericMove
{
  public:
    Eternity2_OddChessboardMove();
};




class Eternity2_ThreeTileStreakMove
{
    friend bool operator==(const Eternity2_ThreeTileStreakMove& m1, const Eternity2_ThreeTileStreakMove& m2);
    friend bool operator!=(const Eternity2_ThreeTileStreakMove& m1, const Eternity2_ThreeTileStreakMove& m2);
    friend bool operator<(const Eternity2_ThreeTileStreakMove& m1, const Eternity2_ThreeTileStreakMove& m2);
    friend ostream& operator<<(ostream& os, const Eternity2_ThreeTileStreakMove& c);
    friend istream& operator>>(istream& is, Eternity2_ThreeTileStreakMove& c);
 
  public:
    
    Eternity2_ThreeTileStreakMove();

    // Coords getter and setter
    // vector<Coord> getCoords() const { return coords; }
    // Coord getCoordAt(int i) const { return coords[i]; }
    // void setCoords(vector<Coord> c) { coords = c; size = c.size; }
  
    // Permutation getter and setter
    vector<pair<unsigned,int>> getPermutation() const { return permutation; }
    void setPermutation(vector<pair<unsigned,int>> p) { permutation = p; }

    int getTTSPerm(int i) const { return permutation[i].first; }
    int getTTSOrientation(int i) const { return permutation[i].second; }

    void setTTSPerm(int i, int j) { permutation[i].first = j; }
    void setTTSOrientation(int i, int o) { permutation[i].second = o; }

    int getSize() const {return permutation.size(); }

    vector<tuple<tileMove,tileMove,tileMove,int>> computeSimpleMoves(const Eternity2_State& st) const;
    void swapPerm(int i, int j);
 
  protected:
  
    // Needs to be initialized with 'setCoords' public method in 'firstMove'
    // and 'randomMove'. Not too reliable as a parameter, since we can't really
    // tell when it will be initialized.
    // vector<Coord> coords;

    // Move permutation
    vector<pair<unsigned,int>> permutation;
};


/****************************************
*
* L-shaped Move
*
* @author Elia
*
******************************************/
class Eternity2_LMove
{
  friend bool operator==(const Eternity2_LMove& m1, const Eternity2_LMove& m2);
  friend bool operator!=(const Eternity2_LMove& m1, const Eternity2_LMove& m2);
  friend bool operator<(const Eternity2_LMove& m1, const Eternity2_LMove& m2);
  friend ostream& operator<<(ostream& os, const Eternity2_LMove& c);
  friend istream& operator>>(istream& is, Eternity2_LMove& c);
 public:
  Eternity2_LMove();
  
  /*
  * Tell where the "hole" in the L-shaped pattern is.
  * U = up, L = left, ...
  * An example is found below.
  *
  */
  const unsigned HOLE_UL = 0;
  const unsigned HOLE_UR = 1;
  const unsigned HOLE_DR = 2;
  const unsigned HOLE_DL = 3;
  const unsigned NO_ELL = 4;
  const unsigned ANY_ELL = 5; // used in placementMatrix
  
  // Read the placement matrix, modifying it depending on the ell considered.
  // TODO explain this as well
  unsigned readPlacementMatrix(unsigned row, unsigned column, unsigned ell);
  
 //protected:
  /* 
  * This is a (flattened) matrix representing the (n-1)*(m-1) 2x2 squares, left to right and top to bottom.
  * n is the number of rows, m is the number of columns.
  * The number inside each cell represents if there in an L in the square and how it's oriented.
  *
  * Example:
  * 
  *  |2|4|      |X|X| |
  *  |4|0|  =>  |X| |X|
  *             | |X|X|
  */
  vector<vector<unsigned>> FlatEllMatrix;
  
  // Which Ls are selected for permutation
  vector<unsigned> EllSelection;
  
  // List of ells in the placement matrix and their coordinates on the board
  // EllList[i]= ( (coordX , coordY) , ellOrientation )
  vector<pair<pair<unsigned,unsigned>,unsigned>> EllList;
  
  // How many ells in the partition
  unsigned ells;
  
  // This matrix tells us if, which and where Ls can be placed around a given L.
  // TODO explain this
  unsigned placementMatrix[][];
  
};


#endif

