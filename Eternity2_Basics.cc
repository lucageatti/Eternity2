// File Eternity2_Basics.cc
#include "Eternity2_Basics.hh"

/**********************************************************************************************
*     STATE
***********************************************************************************************/


/*
* Constructor of the class "Eternity2_State": declares the matrix "board" with the correct size.
*/
Eternity2_State::Eternity2_State(const Eternity2_Input &my_in) 
  : in(my_in)
{
  //Empty Board
  board = vector<vector<IDO> > (in.getHeight());
  for(unsigned c = 0; c < in.getHeight(); c++){
    board.at(c) = vector<IDO> (in.getWidth());
  }
}


/*
* Given an ID of a tile and its orientation, return the color of a given Cardinal Point.
* Used in: "<<" function.
*/
Color Eternity2_State::getColor(IDO ido, CardinalPoint pc) const {
  Tile t = in.getTileAt(ido.first);
  Color cl = t.cardinals.at( abs((pc-ido.second)%4) );
  return cl;
}


/*
* Insert the tile with the given IDO in the given position.
*/
void Eternity2_State::insertTile(IDO ido, unsigned x, unsigned y){
  board.at(x).at(y) = ido;
}


/*
* Copies the element of the board of "st" in the current board. To avoid different sizes of the two boards,
* the current one is resized w.r.t. the size of the second one.
*/
Eternity2_State& Eternity2_State::operator=(const Eternity2_State& st)
{
  board = vector<vector<IDO> >(st.getHeight());
  for(unsigned r = 0; r < st.getHeight(); r++){
    board.at(r) = vector<IDO>(st.getWidth());
    for(unsigned c = 0; c < st.getWidth(); c++){
      board.at(r).at(c) = st.getIDOAt(r,c);
    }
  }

  return *this;
}


/*
* Checks if the two boards are equal. If they have different size, then they are different. If there exists 
* at least one position (x,y) such that the two board don't have the same Tile with the same orientation, then
* they are different.
* Otherwise the two boards are equal.
*/
bool operator==(const Eternity2_State& st1, const Eternity2_State& st2)
{
  if( st1.getHeight() != st2.getHeight() || st2.getWidth() != st2.getWidth() ){
    return false;
  }
  for(unsigned r = 0; r < st1.getHeight(); r++){
    for(unsigned c = 0; c < st1.getWidth(); c++){
      IDO t1 = st1.getIDOAt(r,c);
      IDO t2 = st2.getIDOAt(r,c);
      if( t1.first != t2.first || t1.second != t2.second )
        return false;
    }
  }
  return true;
}


/*
* Prints the state object.
*/
ostream& operator<<(ostream& os, const Eternity2_State& st)
{
  for(unsigned c = 0; c < st.in.getWidth(); c++){
    CardinalPoint pc = 2;
    unsigned counter = 0;
    while( counter != 3 ){

      for(unsigned d = 0; d < st.in.getHeight(); d++){
        
        IDO ido_cd = st.board.at(c).at(d);
        
        if( pc % 2 == 0 ){
          os << "   " << st.getColor( ido_cd, pc ) << "     ";
        }else{
          os << st.getColor( ido_cd, pc ) << "      ";
          os << st.getColor( ido_cd, pc+2 ) << " ";
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
  return os;
}






/**********************************************************************************************
*     MOVES
***********************************************************************************************/


Eternity2_Move::Eternity2_Move()
{
  // Insert the code that initialize the move
  cerr << "Eternity2_Move::Eternity2_Move() not implemented yet" << endl;
}

bool operator==(const Eternity2_Move& mv1, const Eternity2_Move& mv2)
{
  // Insert the code that checks if two moves are identical
  throw logic_error("operator==(const Eternity2_Move& mv1, const Eternity2_Move& mv2) not implemented yet");	
  return true;
}

bool operator!=(const Eternity2_Move& mv1, const Eternity2_Move& mv2)
{
  // Insert the code that checks if two moves are different
  throw logic_error("operator!=(const Eternity2_Move& mv1, const Eternity2_Move& mv2) not implemented yet");	
  return true;
}

bool operator<(const Eternity2_Move& mv1, const Eternity2_Move& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
  throw logic_error("operator<(const Eternity2_Move& mv1, const Eternity2_Move& mv2) not implemented yet");	
  return true;
}

istream& operator>>(istream& is, Eternity2_Move& mv)
{
  // Insert the code that read a move
  throw logic_error("operator>>(istream& is, Eternity2_Move& mv) not implemented yet");	
  return is;
}

ostream& operator<<(ostream& os, const Eternity2_Move& mv)
{
  // Insert the code that writes a move
  throw logic_error("operator<<(ostream& os, const Eternity2_Move& mv) not implemented yet");	
  return os;
}
