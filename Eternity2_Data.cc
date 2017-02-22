// File Eternity2_Data.cc
#include "Eternity2_Data.hh"
#include <fstream>
#include <cassert>

using namespace std;

/**********************************************************************************************
*     INPUT CLASS
***********************************************************************************************/

/*
* Constructor for the "Eternity2_Input" class: it fill the "box" vector with the tiles it reads.
*/
Eternity2_Input::Eternity2_Input(string file_name)
{
  ifstream is(file_name.c_str());
  assert(!is.fail());

  int nord, sud, ovest, est;
  
  is >> height >> width;
  box = vector<Tile>(width * height);
  for(unsigned r = 0; r < width * height; r++){
    Tile t;
    is >> sud >> ovest >> nord >> est;
    t.id = r;
    t.cardinals.push_back(sud);
    t.cardinals.push_back(ovest);
    t.cardinals.push_back(nord);
    t.cardinals.push_back(est);
    box.at(r) = t;
  }
}


/*
* Returns the vector of the tiles with the default orientation set to 0.
*/
vector<IDO> Eternity2_Input::getIDOs() const{
  vector<IDO> idos;
  for(unsigned c = 0; c < width*height; c++){
    //idos.push_back( pair<ID,Orientation>(box.at(c).id,0) ); //Default orientation = 0
    idos.push_back( pair<ID,Orientation>(c,0) ); //Default orientation = 0
  }
  return idos;
}


/*
* Prints the input object.
*/
ostream& operator<<(ostream& os, const Eternity2_Input& pa)
{
  os << pa.width << " " << pa.height << endl;
  for(unsigned c = 0; c < pa.width * pa.height; c++){
    Tile tc = pa.box.at(c);
    vector<Color> tc_card = tc.cardinals;
    for(unsigned i = 0; i < 4; i++){
      os << tc_card.at(i) << " ";
    }
    os << "id = " << tc.id << endl;
  }
  return os;
}








/**********************************************************************************************
*     OUTPUT CLASS
***********************************************************************************************/

/*
* Constructor for the "Eternity2_Output" class.
*/
Eternity2_Output::Eternity2_Output(const Eternity2_Input& my_in)
  : in(my_in)
{
  //initializing "board" vector
  board = vector<vector<IDO> > (my_in.getHeight());
  for(unsigned c = 0; c < my_in.getHeight(); c++){
    board.at(c) = vector<IDO> (my_in.getWidth());
  }
}


/*
* Assignment operator: copies each element of the board of "out" in the board of this object. It resizes the
* current board in order to have the same sizes.
*/
Eternity2_Output& Eternity2_Output::operator=(const Eternity2_Output& out)	
{
  board = vector<vector<IDO> >(out.board.size());
  for(unsigned c = 0; c < out.board.size(); c++){
    vector<IDO> old_vector_c = out.board.at(c);
    board.at(c) = vector<IDO>(old_vector_c.size());
    for(unsigned d = 0; d < old_vector_c.size(); d++){
      board.at(c).at(d) = old_vector_c.at(d);
    }
  }
  return *this;
}


/*
* Given an ID of a tile and its orientation, return the color of a given Cardinal Point.
* Used in: "<<" function.
*/
Color Eternity2_Output::getColor(IDO ido, CardinalPoint pc) const {
  Tile t = in.getTileAt(ido.first);
  Color cl = t.cardinals.at( strangeMod(pc-ido.second,4) );
  return cl;
}

int Eternity2_Output::strangeMod(int dividend, int divisor) const {
  if(dividend < 0){
    dividend += divisor;
  }
  return dividend % divisor;
}


/*
* Insert the tile with the given IDO in the given position.
*/
void Eternity2_Output::insertTile(IDO ido, Coord crd){
  board.at(crd.first).at(crd.second) = ido;
}


/*
* Prints the output object.
*/
ostream& operator<<(ostream& os, const Eternity2_Output& out)
{ 
  for(unsigned c = 0; c < out.in.getWidth(); c++){
    CardinalPoint pc = 2;
    unsigned counter = 0;
    while( counter != 3 ){

      for(unsigned d = 0; d < out.in.getHeight(); d++){
        
        IDO ido_cd = out.board.at(c).at(d);
        
        if( pc % 2 == 0 ){
          os << "   " << out.getColor( ido_cd, pc ) << "     ";
        }else{
          os << out.getColor( ido_cd, pc ) << "      ";
          os << out.getColor( ido_cd, pc+2 ) << " ";
        }

      }
      os << endl;
      switch(pc){
        case 0: pc = 0; break;
        case 1: pc = 0; break;
        case 2: pc = 1; break;
        default: exit(0);
      }
      counter++;
    }
    //os << endl;
  }
  cout << "Orientations:" << endl;
  for(int i = 0; i < out.board.size(); i++){
    for(int j = 0; j < out.board[i].size(); j++){
      cout << "(" << i << "," << j << ") --> " << out.board[i][j].second << endl;
    }
  }

  return os;
}


/*
* Reads the output from a file.
*/
istream& operator>>(istream& is, Eternity2_Output& out)
{
  // Insert the code that reads the output object
  throw logic_error("operator>>(istream& is, Eternity2_Output& out) not implemented yet");	
  return is;
}
