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

  //Setting the coordinates for the Generic Moves (Even-Chessboard and Odd-Chessboard)
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
  singleton_counter = 0;
  singletonRandomCoords();

  //ThreeTilesStreak
  tts_counter = 0;
  ttsRandomCoords();

  //L-Move
  L_counter = 0;
  LRandomCoords();
}



void Eternity2_State::singletonRandomCoords(){
  random_singleton = vector<Coord>();

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
}

void Eternity2_State::ttsRandomCoords(){
  
  random_tts = vector<pair<Coord,int> >();
  int i,j,rnd;
  vector<vector<bool>> feas_board(in.getHeight(),vector<bool>(in.getWidth(),0));
  int pseudo_distribution = std::max((unsigned int)2,(in.getWidth() * in.getHeight()) / 6);

  for (i = 0; i < in.getWidth(); ++i)
  {
      for ( j = 0; j < in.getHeight(); ++j)
      {
          if( ! Random::Int(0,1))
          {
              rnd = Random::Int(0,1);

              if (  !feas_board[i][j]
                 && ((i - rnd) >= 0 && (j + rnd - 1) >= 0 && !feas_board[i - rnd][j + rnd - 1])
                 && ((i + rnd) < in.getHeight() && (j - rnd + 1) < in.getWidth() && !feas_board[i + rnd][j - rnd + 1])
                 && ((i + rnd - 1) < 0 || (j - rnd) < 0 || !feas_board[i + rnd - 1][j - rnd])
                 && ((i - rnd + 1) >= in.getHeight() || (j + rnd) >= in.getWidth() || !feas_board[i - rnd + 1][j + rnd])
                 && ((i - 2*rnd) < 0 || (j + 2*rnd - 2) < 0 || !feas_board[i - 2*rnd][j + 2*rnd - 2])
                 && ((i + 2*rnd) >= in.getHeight() || (j - 2*rnd + 2) >= in.getWidth() || !feas_board[i + 2*rnd][j - 2*rnd + 2])
                 && ((i - 1) < 0 || (j - 1) < 0 || !feas_board[i - 1][j - 1])
                 && ((i - 1) < 0 || (j + 1) >= in.getWidth() || !feas_board[i - 1][j + 1])
                 && ((i + 1) >= in.getHeight() || (j - 1) < 0 || !feas_board[i + 1][j - 1])
                 && ((i + 1) >= in.getHeight() || (j + 1) >= in.getWidth() || !feas_board[i + 1][j + 1]))
               {
                    feas_board[i][j] = 1;
                    feas_board[i - rnd][j + rnd - 1] = 1;
                    feas_board[i + rnd][j - rnd + 1] = 1;

                    random_tts.push_back(make_pair(make_pair( (unsigned int)i, (unsigned int)j ),rnd));
               }
          }
      }
  }

  if (!random_tts.size())
      random_tts.push_back(make_pair(make_pair(0,1),0));

}

bool Eternity2_State::inRange(int val, bool parity) const {
  return val >= 0 && val < ((parity) ? getWidth() : getHeight()); 
}

void Eternity2_State::LRandomCoords(){
  random_L = vector<pair<Coord,int> >();
  int i,j,r;
  int wing1_x;
  int wing1_y;
  int wing2_x;
  int wing2_y;
  vector<vector<bool>> feas_board(in.getHeight(),vector<bool>(in.getWidth(),0));
  int pseudo_distribution = std::max((unsigned int)2,(in.getWidth() * in.getHeight()) / 6);
  for (i = 0; i < in.getWidth(); ++i)
  {
      for ( j = 0; j < in.getHeight(); ++j)
      {
          if( ! Random::Int(0,pseudo_distribution-1))
          {
              r = Random::Int(0,3);

              wing1_x = (r-2)*strangeMod(r,2);
              wing1_y = (r-1)*strangeMod(r+1,2);

              wing2_x = (r-1)*strangeMod(r+1,2);
              wing2_y = (r-2)*strangeMod(r,2);

              bool g1 = !inRange(i-wing1_x,0);
              bool g2 = !inRange(j-wing1_y,1);
              bool g3 = !inRange(i-wing2_x,0);
              bool g4 = !inRange(j-wing2_y,1);
              bool g5 = !inRange(i-wing1_x+wing2_x,0);
              bool g6 = !inRange(j-wing1_y+wing2_y,1);
              bool g7 = !inRange(i+wing1_x-wing2_x,0);
              bool g8 = !inRange(j+wing1_y-wing2_y,1);
              bool g9 = !inRange(i+2*wing1_x,0);
              bool g10 = !inRange(j+2*wing1_y,1);
              bool g11 = !inRange(i+2*wing2_x,0);
              bool g12 = !inRange(j+2*wing2_y,1);
              bool g13 = inRange(i+wing1_x,0);
              bool g14 = inRange(j+wing1_y,1);
              bool g15 = inRange(i+wing2_x,0);
              bool g16 = inRange(j-wing2_y,1);

              bool g17 = !inRange(i+wing1_x+wing2_x,0);
              bool g18 = !inRange(j+wing1_y+wing1_y,1);
              
              if (  !feas_board[i][j]
                 // first wing
                 && g13 && g14
                 && !feas_board[i+wing1_x][j+wing1_y]
                 //second wing
                 && g15 && g16
                 && !feas_board[i+wing2_x][j-wing2_y]
                 //third wing
                 && ( g1 || g2 || !feas_board[i-wing1_x][j-wing1_y] )
                 //fourth wing
                 && ( g3 || g4 || !feas_board[i-wing2_x][j-wing2_y] )
                 //center wing
                 && ( g17 || g18 || !feas_board[i+wing1_x+wing2_x][j+wing1_y+wing1_y])
                 //borders wings
                 && ( g5 || g6 || !feas_board[i-wing1_x+wing2_x][j-wing1_y+wing2_y])
                 && ( g7 || g8 || !feas_board[i+wing1_x-wing2_x][j+wing1_y-wing2_y])
                 && ( g9 || g10 || !feas_board[i+2*wing1_x][j+2*wing1_y])
                 && ( g11 || g12 || !feas_board[i+2*wing2_x][j+2*wing2_y])
                )
               {
                    feas_board[i][j] = 1;
                    feas_board[i + wing1_x][j + wing1_y] = 1;
                    feas_board[i + wing2_x][j + wing2_y] = 1;
                    random_L.push_back(make_pair(make_pair( (unsigned int)i, (unsigned int)j ),r));
               }
               
          }
      }

  }
  if( random_L.size() < 1 ) random_L.push_back(make_pair(make_pair( (unsigned int)Random::Int(0,getHeight()-2), 
      (unsigned int)Random::Int(0,getWidth()-2) ), Random::Int(0,3)));
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
*    GENERIC MOVE
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

/*
* Reads a move from file. The syntax of the file is something like this:
* (2,0) -> (0,1) 0
* (1,2) -> (1,2) 1
* (0,1) -> (2,0) 2
*/
istream& operator>>(istream& is, Eternity2_GenericMove& mv)
{
  throw logic_error("operator<(const Eternity2_GenericMove& mv1, const Eternity2_GenericMove& mv2) not implemented yet");
  return is;
}


/*
* Print of the move.
*/
ostream& operator<<(ostream& os, const Eternity2_GenericMove& mv)
{
  os << endl;
  for(unsigned crd = 0; crd < mv.coords.size(); crd++){
    os << "From: (" << mv.coords.at( mv.permutation.at(crd).first ).first << "," << mv.coords.at( mv.permutation.at(crd).first ).second << ")\t";
    os << "To: (" << mv.coords.at(crd).first << "," << mv.coords.at(crd).second << ")\t";
    os << "Orientation: " << mv.permutation.at(crd).second << endl;
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
  //ellMatrix = vector<vector<unsigned>>();
  ellSelection = vector<unsigned>();
  ells = 0;
  unsigned temp[5][5] = {{5,5,2,3,5},{5,2,4,4,3},{2,4,0,4,4},{1,4,4,4,0},{5,1,4,0,5}};
  for (int i = 0; i < 5; ++i){
    for (int j = 0; j < 5; ++j)
    {
      placementMatrix[i][j]=temp[i][j];
    }
  }
  /*HOLE_UL=0;
  HOLE_UR=1;
  HOLE_DR=2;
  HOLE_DL=3;*/
  NO_ELL=4;
  ANY_ELL=5;
}


bool operator==(const Eternity2_LMove& mv1, const Eternity2_LMove& mv2)
{
  // Insert the code that checks if two moves are identical
  unsigned m = mv1.ellList.size();
  if(m != mv1.ellList.size()) return false;
  for(unsigned i = 0; i<m; i++){
	 if(mv1.ellList.at(i)!=mv2.ellList.at(i)) 
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
  unsigned n = mv1.ellList.size();
  if(n!=mv2.ellList.size())
	  throw logic_error("operator< for Eternity2_LMove called on instances with different size!");
  for(unsigned i=0; i<n; i++)
  {
	 if(mv1.ellList.at(i).first.first < mv2.ellList.at(i).first.first ||
      mv1.ellList.at(i).first.second < mv2.ellList.at(i).first.second ||
      mv1.ellList.at(i).second < mv2.ellList.at(i).second ) return true;
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
  os << endl;
  for(unsigned i = 0; i < mv.ellList.size(); i++)
  {
    os << "From: (<" << mv.ellList.at(i).first.first << "," << mv.ellList.at(i).first.second << 
      ">," << mv.ellList.at(i).second << ")\t";
    os << "To: (<" << mv.ellList.at(mv.ellSelection.at(i)).first.first << "," << 
      mv.ellList.at(mv.ellSelection.at(i)).first.second << ">," << mv.ellList.at(mv.ellSelection.at(i)).second << ")\t";
    os << endl;
  }
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
      ret = placementMatrix[row][column];
      break;
    case 1:
      ret = placementMatrix[rows-1-column][row];
      break;
    case 2:
      ret = placementMatrix[rows-1-row][cols-1-column];
      break;
    case 3:
      ret = placementMatrix[column][cols-1-row];
      break;
    default:
      ret = ell;
      break;
  }
  if(ret < Eternity2_LMove::NO_ELL) ret+=ell;
  return ret;
}







Eternity2_SingletonMove::Eternity2_SingletonMove() : Eternity2_GenericMove() {
 
};


Eternity2_EvenChessboardMove::Eternity2_EvenChessboardMove() : Eternity2_GenericMove() {

};


Eternity2_OddChessboardMove::Eternity2_OddChessboardMove() : Eternity2_GenericMove() {

};






/**********************************************************************************************
*    THREE TILE STREAK MOVE
***********************************************************************************************/

// The Eternity2 Move constructor simply instantiate an unsized vector. To define 
// a move properly, 'setCoords' method needs to be called. If 'size' is set to -1
// the move is not specified yet.
Eternity2_ThreeTileStreakMove::Eternity2_ThreeTileStreakMove()
{
    permutation = vector<pair<unsigned,int>>();
}

// Compute simple tile-wise moves from the three tile streak moves.
tuple<tileMove,tileMove,tileMove,int> Eternity2_ThreeTileStreakMove::computeSimpleMove(const Eternity2_State& st, const pair<unsigned,int>& perm, int dest) const
{
    Coord from,to;
    int from_dir,to_dir;

    pair<IDO,Coord> m,m1,m2;

    from = st.random_tts[perm.first].first;
    from_dir = st.random_tts[perm.first].second;
    to = st.random_tts[dest].first;
    to_dir = st.random_tts[dest].second;

    m = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*perm.second + 3*from_dir + to_dir) % 4),to);

    from = make_pair(from.first - from_dir,from.second + from_dir - 1);
    to = make_pair(to.first - to_dir + to_dir*2*perm.second,to.second + to_dir - 1 + (1 - to_dir)*2*perm.second);

    m1 = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*perm.second + 3*from_dir + to_dir) % 4),to);
    
    from = make_pair(from.first + 2*from_dir,from.second + 2 - 2*from_dir);
    to = make_pair(to.first + 2*to_dir - to_dir*4*perm.second,to.second + 2 - 2*to_dir - (1 - to_dir)*4*perm.second);

    m2 = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*perm.second + 3*from_dir + to_dir) % 4),to);

    if (perm.second)
    {
      return make_tuple(m2,m,m1,to_dir);

    } else {

      return make_tuple(m1,m,m2,to_dir);
    }
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
    auto perm = mv.getPermutation();
    auto coords = mv.getCoordinates();
    os << endl;
    for (int i = 0; i < mv.getSize(); ++i)
    {
        os << "From: ("  << coords[perm[i].first].first.first << "," << coords[perm[i].first].first.second << ") "
           << ((coords[perm[i].first].second) ? "Vertical\t" : "Horizontal\t")
           << "To: " << "(" << coords[i].first.first << "," << coords[i].first.second << ") "
           << ((coords[i].second) ? "Vertical\t" : "Horizontal\t")
           << "Dir/Inv: " << ((perm[i].second) ? "Inverse" : "Direct") << endl;
    }
    return os;
}





