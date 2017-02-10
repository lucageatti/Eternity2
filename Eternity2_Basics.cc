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

  //Setting the coordinates for the Singleton Moves
  for(unsigned r = 0; r < in.getHeight(); r++){
    for(unsigned c = 0; c < in.getWidth(); c++){
      if( r % 2 == 0  &&  c % 2 == 0 )
        even_chessboard.push_back( pair<unsigned,unsigned>(r,c) );
      if( r % 2 == 1  &&  c % 2 == 1 )
        even_chessboard.push_back( pair<unsigned,unsigned>(r,c) );
      if( r % 2 == 0  &&  c % 2 == 1 )
        odd_chessboard.push_back( pair<unsigned,unsigned>(r,c) );
      if( r % 2 == 1  &&  c % 2 == 0 )
        odd_chessboard.push_back( pair<unsigned,unsigned>(r,c) );
    }
  }

  //Singleton Move
  vector<vector<bool> > cover = vector<vector<bool> >(in.getHeight());
  int num_free = in.getHeight() * in.getWidth(); //All the cells are free
  for(unsigned r = 0; r < cover.size(); r++){
    cover.at(r) = vector<bool>(in.getWidth());
    for(unsigned c = 0; c < in.getWidth(); c++){
      cover.at(r).at(c) = 0;
    }
  }

  while( num_free > 0 ){
    int rdm_pos_x;
    int rdm_pos_y;
    do{
      rdm_pos_x = Random::Int(0,in.getHeight()-1);
      rdm_pos_y = Random::Int(0,in.getWidth()-1);
      if( ! cover.at(rdm_pos_x).at(rdm_pos_y) ){
        cover.at(rdm_pos_x).at(rdm_pos_y) = 1;
        num_free--;
        if( rdm_pos_x > 0 ){
          if( ! cover.at(rdm_pos_x-1).at(rdm_pos_y) )
            num_free--;
          cover.at(rdm_pos_x-1).at(rdm_pos_y) = 1;
        }
        if( rdm_pos_x < in.getHeight()-1 ){
          if( ! cover.at(rdm_pos_x+1).at(rdm_pos_y) )
            num_free--;
          cover.at(rdm_pos_x+1).at(rdm_pos_y) = 1;
        }
        if( rdm_pos_y > 0 ){
          if( !cover.at(rdm_pos_x).at(rdm_pos_y-1) )
            num_free--;
          cover.at(rdm_pos_x).at(rdm_pos_y-1) = 1;
        }
        if( rdm_pos_y < in.getWidth()-1 ){
          if( !cover.at(rdm_pos_x).at(rdm_pos_y+1) )
            num_free--;
          cover.at(rdm_pos_x).at(rdm_pos_y+1) = 1;
        }
        random_singleton.push_back(pair<unsigned,unsigned>(rdm_pos_x,rdm_pos_y));
      }
    } while( ! cover.at(rdm_pos_x).at(rdm_pos_y) );
  }


  //ThreeTilesStreak
  random_tts = vector<pair<Coord,int> >(2);
  random_tts.at(0) = pair<Coord,int>(pair<unsigned,unsigned>(1,0),1);
  random_tts.at(1) = pair<Coord,int>(pair<unsigned,unsigned>(1,2),1);
}


/*
* Given an ID of a tile and its orientation, return the color of a given Cardinal Point.
* Used in: "<<" function.
*/
Color Eternity2_State::getColor(IDO ido, CardinalPoint pc) const {
  Tile t = in.getTileAt(ido.first);
  Color cl = t.cardinals.at( strangeMod(pc-ido.second,4) );
  return cl;
}

int Eternity2_State::strangeMod(int dividend, int divisor) const {
  if(dividend < 0){
    dividend += divisor;
  }
  return dividend % divisor;
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
      board.at(r).at(c) = st.getIDOAt(pair<unsigned,unsigned>(r,c));
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
      IDO t1 = st1.getIDOAt(pair<unsigned,unsigned>(r,c));
      IDO t2 = st2.getIDOAt(pair<unsigned,unsigned>(r,c));
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
  vector<vector<IDO> > board = st.board;
  for(unsigned r = 0; r < st.getHeight(); r++){
    for(unsigned c = 0; c < st.getWidth(); c++){
      os << "(" << board.at(r).at(c).first << "," << board.at(r).at(c).second << ")" << "   ";
    }
    os << endl;
  }
  return os;
}








/**********************************************************************************************
*    SINGLETON MOVE
***********************************************************************************************/


/*
* Constructor of the "Eternity2_GenericMove" class. It initializes the "permutation" vector. It cannot take as
* parameters the list of coordinates, since easylocal++ requires all the move's constructors to have no parameters.
* The "coords" vector is set with the public method "setCoords()".
*/
Eternity2_GenericMove::Eternity2_GenericMove()
{
  
}


/*
* Equality operator: if the two moves are not defined over the same set of positions, then they are different, i.e.:
* check if the vectors "coords" are the same.
* Otherwise, checks if the vectors "permutation" of the two object are the same, i.e. for all i, the i-esim
* cell of the two vector must contain the same IDO.
*
* ASSUMPTION: the elements of these vector are ordered. This allows to check the two vectors "coords" cell by cell.
*/
bool operator==(const Eternity2_GenericMove& mv1, const Eternity2_GenericMove& mv2)
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
    vector<pair<unsigned,Orientation> > perm1 = mv1.permutation;
    vector<pair<unsigned,Orientation> > perm2 = mv2.permutation;
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
bool operator!=(const Eternity2_GenericMove& mv1, const Eternity2_GenericMove& mv2)
{
  return ! (mv1 == mv2);
}


/*
* Less-than operator. It checks first if ... 
*/
bool operator<(const Eternity2_GenericMove& mv1, const Eternity2_GenericMove& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
  throw logic_error("operator<(const Eternity2_GenericMove& mv1, const Eternity2_GenericMove& mv2) not implemented yet");	
  return true;
}

istream& operator>>(istream& is, Eternity2_GenericMove& mv)
{
  // Insert the code that read a move
  throw logic_error("operator>>(istream& is, Eternity2_GenericMove& mv) not implemented yet");	
  return is;
}

ostream& operator<<(ostream& os, const Eternity2_GenericMove& mv)
{
  os << endl;
  for(unsigned crd = 0; crd < mv.coords.size(); crd++){
    os << "(" << mv.coords.at(crd).first << "," << mv.coords.at(crd).second << ")   :   ";
    os << "old position = (" << mv.coords.at( mv.permutation.at(crd).first ).first << "," << mv.coords.at( mv.permutation.at(crd).first ).second << ")";
    os << "   orientation = " << mv.permutation.at(crd).second << endl;
  }
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
  ellMatrix = vector<vector<unsigned>>();
  ellSelection = vector<unsigned>();
  ells = 0;
}


bool operator==(const Eternity2_LMove& mv1, const Eternity2_LMove& mv2)
{
  // Insert the code that checks if two moves are identical
  unsigned m = mv1.ellMatrix.size();
  if(m != mv1.ellMatrix.size()) return false;
  for(unsigned i = 0; i<m; i++){
	 if(mv1.ellMatrix.at(i)!=mv2.ellMatrix.at(i)) 
		 return false;
  }
  
  m = mv1.ellSelection.size();
  if(m != mv1.ellSelection.size()) return false;
  for(unsigned i = 0; i<m; i++){
	 if(mv1.ellSelection.at(i)!=mv2.ellSelection.at(i)) 
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
  unsigned n = mv1.ellMatrix.size();
  if(n!=mv2.ellMatrix.size())
	  throw logic_error("operator< for Eternity2_LMove called on instances with different size!");
  for(unsigned i=0; i<n; i++){
	 if(mv1.ellMatrix.at(i)<mv2.ellMatrix.at(i)) return true;
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

/* Read the placement matrix for a given ell.
* The placement matrix tells us which ell placements are valid after
* placing a given ell in a 5x5 area around it. 
* Only the placement matrix for HOLE_UL is stored; the others are
* computed by modifying it at run-time.
* This function essentially maps a position on a matrix into the corresponding
* position in the rotated matrix, and adds something to the result.*/
unsigned Eternity2_LMove::readPlacementMatrix(unsigned row, unsigned column, unsigned ell){
  unsigned ret = 4; // NO_ELL
  unsigned rows = sizeof(Eternity2_LMove::placementMatrix);
  unsigned cols = sizeof(Eternity2_LMove::placementMatrix[0]);
  switch(ell){
    case 0:
      ret = *Eternity2_LMove::placementMatrix[row,column];
      break;
    case 1:
      ret = *Eternity2_LMove::placementMatrix[rows-1-column,row];
      break;
    case 2:
      ret = *Eternity2_LMove::placementMatrix[rows-1-row,cols-1-column];
      break;
    case 3:
      ret = *Eternity2_LMove::placementMatrix[column,cols-1-row];
      break;
    default:
      ret = ell;
      break;
  }
  if(ret < Eternity2_LMove::NO_ELL) ret+=ell;
  return ret;
}

/*
* Partition the board into L-shaped clusters such that no two of them "touch" each
* other horizontally or vertically.
* This should be called only once as the partition is not modified by the move.
*/
vector<vector<unsigned>> Eternity2_LMove::EllGeneration(const Eternity2_State& st)
{
  unsigned r = st.getHeight(); // Rows
  unsigned c = st.getWidth(); // Columns
  ells = 0;
  ellList = vector<tuple<unsigned,unsigned,unsigned>>(ells);
  
  /* Space between the current cell and each border of the matrix
  *  L= left, U = up, ... 
  */
  unsigned spaceL=0;
  unsigned spaceR=0;
  unsigned spaceU=0;
  unsigned spaceD=0;
  
  // Current row and column
  unsigned cr = 0;
  unsigned cc = 0;
  // Orientation of the last L inserted
  unsigned lo = 0;
  
  bool done = false;
  
  unsigned nextPosr = 0;
  unsigned nextPosc = 0;
  
  // Initialize the matrix
  vector<vector<unsigned>> partition = vector<vector<unsigned>>(r);
  for(int i = 0; i<r; i++){
    partition.at(i) = vector<unsigned>(c, ANY_ELL);
  }
    
  while(/*!done*/ /*cc < c &&*/ cr < r){
    
    if(cc==0 && cr == 0){
      // Randomized start
      lo = Random::Int(0,3);
      partition.at(0).at(0) = lo;
    }else{ 
    
      // Use the computed constraints
      if(partition.at(cr).at(cc) == ANY_ELL){
        // No constraints, use random
        lo = Random::Int(0,3);
        partition.at(cr).at(cc) = lo;       
      }else if(partition.at(cr).at(cc) < NO_ELL){
        // A constraint has been placed here
        lo = partition.at(cr).at(cc);       
      }else{
        lo = NO_ELL;
        if(++cc == c){ // Move to the next position
          cc = 0;
          cr++;
        } 
        continue;
      }
      
    }

    ells++;
    ellList.push_back(tuple<unsigned,unsigned,unsigned>(cr,cc,lo));
      
    /* Now we want to place constraints on which ells can be placed next
    * based on the ell placed this iteration. */
    // Compute the area to work on
    spaceL = cc;
    spaceR = c-cc-1;
    //spaceU = cr;
    spaceD = r-cr-1;
    unsigned i1 = std::max((int)(2-spaceL), 0);
    unsigned j1 = 2;//2 - std::max(2,spaceU);
    unsigned i2 = std::min(4, (int)(2+spaceR));
    unsigned j2 = std::min(4,(int)(2+spaceD));
    
    /* The placement matrix tells us which constraints to put around the 
    * last placed ell in a 5x5 radius.
    * I read the placement matrix from (i1,2) to (i2,j2) so constraints 
    * can be placed on which ells (if any) to place next.
    * Actually I can skip (i1,j1) to (2,2)*/
    for(unsigned i=3; i<=i2; i++){
      unsigned constraint = readPlacementMatrix(2,i,lo);
      partition.at(2).at(i) = constraint;
      // Jump to the first position that allows and ell
      if(nextPosr == 0 && nextPosc == 0 && constraint != NO_ELL){ 
        nextPosr = 2;
        nextPosc = i;
      }
    }
    for(unsigned j=j1; j<=j2; j++){
      for(unsigned i=i1; i<=i2; i++){
        unsigned constraint = readPlacementMatrix(j,i,lo);
        partition.at(j).at(i) = constraint;
        // Jump to the first position that allows and ell
        if(nextPosr == 0 && nextPosc == 0 && constraint != NO_ELL){ 
          nextPosr = 2;
          nextPosc = i;
        }
      }
    }
    
    // Jump over squares where ells are not allowed
    if(nextPosr > 0 && nextPosc > 0){
      cc = nextPosc;
      cr = nextPosr;
      nextPosc = 0;
      nextPosr = 0;
    }else if(++cc == c){ // Move to the next position
      cc = 0;
      cr++;
    } 
  }
  // End while

}







Eternity2_SingletonMove::Eternity2_SingletonMove() : Eternity2_GenericMove() {

};


Eternity2_EvenChessboardMove::Eternity2_EvenChessboardMove() : Eternity2_GenericMove() {

};


Eternity2_OddChessboardMove::Eternity2_OddChessboardMove() : Eternity2_GenericMove() {

};








// The Eternity2 Move constructor simply instantiate an unsized vector. To define 
// a move properly, 'setCoords' method needs to be called. If 'size' is set to -1
// the move is not specified yet.
Eternity2_ThreeTileStreakMove::Eternity2_ThreeTileStreakMove()
{
    permutation = vector<pair<unsigned,int>>();
}

// Compute simple tile-wise moves from the three tile streak moves.
vector<tuple<tileMove,tileMove,tileMove,int>> Eternity2_ThreeTileStreakMove::computeSimpleMoves(const Eternity2_State& st) const
{
    vector<tuple<tileMove,tileMove,tileMove,int>> changes;
    Coord from,to;
    int from_dir,to_dir;

    for (int i = 0; i < st.random_tts.size(); ++i)
    {
        pair<IDO,Coord> m,m1,m2;

        from = st.random_tts[permutation[i].first].first;
        from_dir = st.random_tts[permutation[i].first].second;
        to = st.random_tts[i].first;
        to_dir = st.random_tts[i].second;

        m = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*permutation[i].second + 3*from_dir + to_dir) % 4),to);

        from = make_pair(from.first - from_dir,from.second + from_dir - 1);
        to = make_pair(to.first - to_dir + to_dir*2*permutation[i].second,to.second + to_dir - 1 + (1 - to_dir)*2*permutation[i].second);

        m1 = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*permutation[i].second + 3*from_dir + to_dir) % 4),to);
        
        from = make_pair(from.first + 2*from_dir,from.second + 2 - 2*from_dir);
        to = make_pair(to.first + 2*to_dir - to_dir*4*permutation[i].second,to.second + 2 - 2*to_dir - (1 - to_dir)*4*permutation[i].second);

        m2 = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*permutation[i].second + 3*from_dir + to_dir) % 4),to);

        if (permutation[i].second)
        {
          changes.push_back(make_tuple(m2,m,m1,to_dir));

        } else {

          changes.push_back(make_tuple(m1,m,m2,to_dir));
        }
    }

    return changes;
}


// Swaps two elements in a permutation of the tiles
void Eternity2_ThreeTileStreakMove::swapPerm(int i, int j)
{
    swap(permutation[i],permutation[j]);
}

// Checks the equality between moves.
// N.B. we assume the elements of the 'coords' vectors are ordered.
bool operator==(const Eternity2_ThreeTileStreakMove& mv1, const Eternity2_ThreeTileStreakMove& mv2)
{
    int i;

    if (mv1.getSize() != mv2.getSize())
    {
      return false;
    
    } else {

      for (i = 0; i < mv1.getSize() && mv1.permutation[i] == mv2.permutation[i]; ++i);
      return (i == mv1.getSize());
    }
}

bool operator!=(const Eternity2_ThreeTileStreakMove& mv1, const Eternity2_ThreeTileStreakMove& mv2)
{
  return !(mv1 == mv2);
}

bool operator<(const Eternity2_ThreeTileStreakMove& mv1, const Eternity2_ThreeTileStreakMove& mv2)
{
  // Insert the code that checks if one move precedes another one
  // (in any selected order)
  throw logic_error("operator<(const Eternity2_ThreeTileStreakMove& mv1, const Eternity2_ThreeTileStreakMove& mv2) not implemented yet"); 
  return true;
}

istream& operator>>(istream& is, Eternity2_ThreeTileStreakMove& mv)
{
  // Insert the code that read a move
  throw logic_error("operator>>(istream& is, Eternity2_ThreeTileStreakMove& mv) not implemented yet"); 
  return is;
}

ostream& operator<<(ostream& os, const Eternity2_ThreeTileStreakMove& mv)
{
    for (int i = 0; i < mv.getSize(); ++i)
    {
        os << i << "<-- <" << mv.getPermutation()[i].first << "," << mv.getPermutation()[i].second << ">" << endl;
    }
    return os;
}

