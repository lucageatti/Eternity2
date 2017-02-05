// File Eternity2_Basics.hh
#ifndef Eternity2_BASICS_HH
#define Eternity2_BASICS_HH

#include "Eternity2_Data.hh"

class Eternity2_State
{  
  friend ostream& operator<<(ostream& os, const Eternity2_State& st);
  friend bool operator==(const Eternity2_State& st1, const Eternity2_State& st2);
public:
  Eternity2_State(const Eternity2_Input &in);
  Eternity2_State& operator=(const Eternity2_State& s);
  IDO getIDOAt(unsigned i, unsigned j) const { return board.at(i).at(j); }
  unsigned getHeight() const { return board.size(); }
  unsigned getWidth() const { return board.at(0).size(); }
  void insertTile(IDO ido, Coord crd);
  Color getColor(IDO ido, CardinalPoint pc) const;
protected:
  const Eternity2_Input & in;
  vector<vector<IDO> > board;
};

class Eternity2_SingletonMove
{
  friend bool operator==(const Eternity2_SingletonMove& m1, const Eternity2_SingletonMove& m2);
  friend bool operator!=(const Eternity2_SingletonMove& m1, const Eternity2_SingletonMove& m2);
  friend bool operator<(const Eternity2_SingletonMove& m1, const Eternity2_SingletonMove& m2);
  friend ostream& operator<<(ostream& os, const Eternity2_SingletonMove& c);
  friend istream& operator>>(istream& is, Eternity2_SingletonMove& c);
 public:
  Eternity2_SingletonMove();
  
  void setCoords(vector<Coord> coord) { coords = coord; }
  vector<Coord> getCoords() const { return coords; }
  
  vector<unsigned> getAuxPerm() const { return aux_perm; }
  void setAuxPerm(vector<unsigned> ap) { aux_perm = ap; }
  
  vector<IDO> getFirstPermutation() const { return first_permutation; };
  void setFirstPermutation(vector<IDO> fp) { first_permutation = fp; };
  
  vector<IDO> permutation; //The only real parameter of this move.
 protected:
  //Initialized by the constructor: fixed parameter. It stores the coordinates considered by this move.
  vector<Coord> coords;
  //Current permutation. It stores the index of the current permutation, like: 3,4,1,5,2.
  vector<unsigned> aux_perm;
  //Used as an hash-map from the permutation to the IDOs.
  vector<IDO> first_permutation;
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
  Eternity2_LMove(unsigned width, unsigned height);
  
  /*
  * Tell where the "hole" in the L-shaped pattern is.
  * U = up, L = left, ...
  * An example is found below.
  */
  const unsigned HOLE_UL = 0;
  const unsigned HOLE_UR = 1
  const unsigned HOLE_DR = 2
  const unsigned HOLE_DL = 3;
  const unsigned NO_ELL = 4;
  
  /* 
  * This is a (flattened) matrix representing the (n-1)*(m-1) 2x2 squares, left to right and top to bottom.
  * The number inside each cell represents if there in an L in the square and how it's oriented.
  *
  * Example:
  * 
  *  |2|4|      |X|X| |
  *  |4|0|  =>  |X| |X|
  *             | |X|X|
  */
  vector<unsigned> FlatEllMatrix;
  
  
};






#endif

