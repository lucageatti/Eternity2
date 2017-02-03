// File Eternity2_Data.hh
#ifndef Eternity2_DATA_HH
#define Eternity2_DATA_HH

#include "Common.hh"
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>


using namespace std;


class Eternity2_Input 
{
  friend ostream& operator<<(ostream& os, const Eternity2_Input& bs);
public:
  Eternity2_Input(string file_name);
  Tile getTileAt(unsigned i) const { return box.at(i); }
  unsigned getHeight() const { return height; }
  unsigned getWidth() const { return width; }
  vector<IDO> getIDOs() const;
protected:
  unsigned height;
  unsigned width;
  vector<Tile> box;
};


class Eternity2_Output 
{
  friend ostream& operator<<(ostream& os, const Eternity2_Output& out);
  friend istream& operator>>(istream& is, Eternity2_Output& out);
public:
  Eternity2_Output(const Eternity2_Input& i);
  Eternity2_Output& operator=(const Eternity2_Output& out);
  void insertTile(IDO ido, unsigned x, unsigned y);
protected:
  const Eternity2_Input& in;
  vector<vector<IDO> > board; //board of couples (id,orientation) where orientation \in {0..3}
  Color getColor(IDO ido, CardinalPoint pc) const; //for the "<<"
};


#endif
