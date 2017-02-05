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
void Eternity2_State::insertTile(IDO ido,Coord crd){
  board.at(crd.first).at(crd.second) = ido;
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
*    SINGLETON MOVE
***********************************************************************************************/


/*
* Constructor of the "Eternity2_SingletonMove" class. It initializes the "permutation" vector. It cannot take as
* parameters the list of coordinates, since easylocal++ requires all the move's constructors to have no parameters.
* The "coords" vector is set with the public method "setCoords()".
*/
Eternity2_SingletonMove::Eternity2_SingletonMove()
{
   permutation = vector<IDO>(); //Size unknown
}


/*
* Equality operator: if the two moves are not defined over the same set of positions, then they are different, i.e.:
* checks if the vectors "coords" are the same.
* Otherwise, checks if the vectors "permutation" of the two object are the same, i.e. for all i, the i-esim
* cell of the two vector must contain the same IDO.
*
* ASSUMPTION: the elements of these vector are ordered. This allows to check the two vectors "coords" cell by cell.
*/
bool operator==(const Eternity2_SingletonMove& mv1, const Eternity2_SingletonMove& mv2)
{
  vector<Coord> coords1 = mv1.coords;
  vector<Coord> coords2 = mv2.coords;
  if( coords1.size() != coords2.size() )
    return false;
  else{
    //Check if "coords1" and "coords2" are the same. 
    //ASSUMPTION: the elements of these vector are ordered
    for(unsigned c = 0; c < coords1.size(); c++){
      if( coords1.at(c).first != coords2.at(c).first || coords1.at(c).second != coords2.at(c).second )
        return false;
    }
    //Check if "perm1" and "perm2" are the same.
    vector<IDO> perm1 = mv1.permutation;
    vector<IDO> perm2 = mv2.permutation;
    for(unsigned c = 0; c < perm1.size(); c++){
      if( perm1.at(c).first != perm2.at(c).first || perm1.at(c).second != perm2.at(c).second )
        return false;
    }
  }
  return true;
}


/*
* Disequality operator
*/
bool operator!=(const Eternity2_SingletonMove& mv1, const Eternity2_SingletonMove& mv2)
{
  return ! (mv1 == mv2);
}


/*
* Less-than operator. It checks first if ... 
*/
bool operator<(const Eternity2_SingletonMove& mv1, const Eternity2_SingletonMove& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
  throw logic_error("operator<(const Eternity2_SingletonMove& mv1, const Eternity2_SingletonMove& mv2) not implemented yet");	
  return true;
}

istream& operator>>(istream& is, Eternity2_SingletonMove& mv)
{
  // Insert the code that read a move
  throw logic_error("operator>>(istream& is, Eternity2_SingletonMove& mv) not implemented yet");	
  return is;
}

ostream& operator<<(ostream& os, const Eternity2_SingletonMove& mv)
{
  // Insert the code that writes a move
  throw logic_error("operator<<(ostream& os, const Eternity2_SingletonMove& mv) not implemented yet");	
  return os;
}






/**************************************************************************************************************************
***************************************************************************************************************************
***************************************************************************************************************************
*
* L-shaped Move
*
* @author Elia
*
*		                                 @@@
*		######                          @   @
*		######                          @   @
*		######                           ### 
*		######                           ###                   
*		######                           @ @                   
*		######                          @   @                  
*		######                         @     @                
*		######                        @       @              
*		######                        @       @               
*		##################            @       @               
*		##################             @     @                  
*		##################              @@@@@
*
*
***************************************************************************************************************************
***************************************************************************************************************************
***************************************************************************************************************************/
Eternity2_LMove::Eternity2_LMove()
{
  // Insert the code that initializes the move
  flatEllMatrix = vector<unsigned>();
}

Eternity2_LMove::Eternity2_LMove(unsigned width, unsigned height)
{
  // Insert the code that initializes the move
  flatEllMatrix = vector<unsigned>(width*height);
}


bool operator==(const Eternity2_LMove& mv1, const Eternity2_LMove& mv2)
{
  // Insert the code that checks if two moves are identical
  unsigned n = mv1.flatEllMatrix.size();
  if(m != mv1.flatEllMatrix.size()) return false;
  for(unsigned i = 0; i<m; i++){
	 if(mv1.flatEllMatrix.at(i)!=mv2.flatEllMatrix.at(i)) 
		 return false;
  }
  return true;
}

bool operator!=(const Eternity2_LMove& mv1, const Eternity2_LMove& mv2)
{
  // Insert the code that checks if two moves are different
  return !(mv1==mv2);
}

bool operator<(const Eternity2_LMove& mv1, const Eternity2_LMove& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
  unsigned n = mv1.flatEllMatrix.size();
  if(n!=mv2.flatEllMatrix.size())
	  throw logic_error("operator< for Eternity2_LMove called on instances with different size!");
  for(unsigned i=0; i<; i++){
	if(mv1.flatEllMatrix.at(i)<mv2.flatEllMatrix.at(i)) return true;
  }
  return false;
}

istream& operator>>(istream& is, Eternity2_LMove& mv)
{
  // Insert the code that read a move
  throw logic_error("operator>>(istream& is, Eternity2_LMove& cìmv) not implemented yet");	
  return is;
}

ostream& operator<<(ostream& os, const Eternity2_LMove& mv)
{
  // Insert the code that writes a move
  throw logic_error("operator<<(ostream& os, const Eternity2_LMove& mv) not implemented yet");	
  return os;
}