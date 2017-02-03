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
  void insertTile(IDO ido, unsigned x, unsigned y);
  Color getColor(IDO ido, CardinalPoint pc) const;
protected:
  const Eternity2_Input & in;
  vector<vector<IDO> > board;
};

class Eternity2_Move
{
  friend bool operator==(const Eternity2_Move& m1, const Eternity2_Move& m2);
  friend bool operator!=(const Eternity2_Move& m1, const Eternity2_Move& m2);
  friend bool operator<(const Eternity2_Move& m1, const Eternity2_Move& m2);
  friend ostream& operator<<(ostream& os, const Eternity2_Move& c);
  friend istream& operator>>(istream& is, Eternity2_Move& c);
 public:
  Eternity2_Move();
};
#endif

