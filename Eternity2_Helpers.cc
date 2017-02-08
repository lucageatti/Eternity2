// File Eternity2_Helpers.cc
#include "Eternity2_Helpers.hh"




/**********************************************************************************************
*     COST-COMPONENTS
***********************************************************************************************/

/*
* Returns the total cost of the given state: it computes for each tile the number of edges that don't match.
* The total cost is the sum of all these costs dived by two. 
* In order to do this, the border edges has weigth 2 instead of 1 like all other edges.
* CONVENTION: the color 0 is the Grey color, i.e. the color for the borders.
*/
int Eternity2_CostComponent::ComputeCost(const Eternity2_State& st) const
{
  unsigned cost = 0;
  for(unsigned r = 0; r < st.getHeight(); r++){
    for(unsigned c = 0; c < st.getWidth(); c++){
      Coord crd = pair<unsigned,unsigned>(r,c);
      IDO ido_rc = st.getIDOAt(crd);
      cost += singleTileCost(ido_rc, crd, st);
    }
  }
  return cost/2;
}

/*
* ...
*/
void Eternity2_CostComponent::PrintViolations(const Eternity2_State& st, ostream& os) const
{
  // Insert the code that prints the violations of component 1 
  // of state st
	throw logic_error("Eternity2_CostComponent1::PrintViolations not implemented yet");	
}





/**********************************************************************************************
*     STATE-MANAGER
***********************************************************************************************/


/*
* Constructor for the class "Eternity2_StateManager"
*/
Eternity2_StateManager::Eternity2_StateManager(const Eternity2_Input & pin) 
  : StateManager<Eternity2_Input,Eternity2_State>(pin, "Eternity2StateManager") , in(pin) {} 


/*
* Creates a random state: given the initial order of the tile (i.e. the order in which the Input class has read them
* from the input file), it exploits the "Fisher-Yates Algorithm" to create an unbiased permutation of these tiles and
* their respectively orientations. It fills the board w.r.t. this random permutation.
*/
void Eternity2_StateManager::RandomState(Eternity2_State& st) 
{
  //Randomized version.
  vector<IDO> init_permutation = in.getIDOs(); //All with orientation = 0 (default).
  vector<IDO> rdm_permutation = FisherYatesShuffleIDO(init_permutation);
  unsigned counter = 0;
  for(unsigned c = 0; c < st.getHeight(); c++){
    for(unsigned d = 0; d < st.getWidth(); d++){
      st.insertTile(rdm_permutation.at(counter),pair<unsigned,unsigned>(c,d));
      counter++;
    }
  }
} 


/*
* Implementation of the "Fisher-Yates Algorithm" modified in order to take into account also a random
* orientation of the tiles. It generates a unbiased permutation of the vector "hat".
*/
vector<IDO> Eternity2_StateManager::FisherYatesShuffleIDO(vector<IDO> hat) const {
  unsigned sz = hat.size();
  unsigned j;
  for(unsigned i = sz-1; i > 0; i--){
    j = Random::Int(0,i);
    hat.at(i).second = Random::Int(0,3);
    swap(hat.at(i), hat.at(j));
  }
  return hat;
}


/*
* Checks if the state size and the input size are the same.
*/
bool Eternity2_StateManager::CheckConsistency(const Eternity2_State& st) const
{
  if( in.getWidth() * in.getHeight() != st.getWidth() * st.getHeight() )
    return false;
  return true;
}





/*****************************************************************************
 * Output Manager Methods
 *****************************************************************************/

void Eternity2_OutputManager::InputState(Eternity2_State& st, const Eternity2_Output& out) const 
{
  // Insert the code that "translates" an output object to a state object
	throw logic_error("Eternity2_OutputManager::InputState not implemented yet");	
}


/*
* Translates a state object into an output object
*/
void Eternity2_OutputManager::OutputState(const Eternity2_State& st, Eternity2_Output& out) const 
{
  for(unsigned r = 0; r < st.getHeight(); r++){
    for(unsigned c = 0; c < st.getWidth(); c++){
      out.insertTile(st.getIDOAt(pair<unsigned,unsigned>(r,c)), pair<unsigned,unsigned>(r,c));
    }
  }
}









/*****************************************************************************
 * GenericMove Neighborhood Explorer Methods
 *****************************************************************************/


/*
* The move is always feasible.
*/
bool GenericMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_GenericMove& mv) const
{
  return true;
} 


/*
* Update the state according to the move.
*/
void GenericMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_GenericMove& mv) const
{
  vector<Coord> coords = mv.getCoordinates();
  vector<IDO> old_tiles = vector<IDO>(coords.size());
  for(unsigned c = 0; c < coords.size(); c++){
    old_tiles.at(c) = st.getIDOAt(coords.at(c));
  }
  for(unsigned c = 0; c < coords.size(); c++){
    old_tiles.at(mv.getIndexAt(c)).second = mv.getOrientationAt(c);
    st.insertTile(old_tiles.at(mv.getIndexAt(c)), coords.at(c));
  }
}  



/*
* It computes the next move, computing the next permutation of the indexes 0, 1, ..., N.
*/
bool GenericMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_GenericMove& mv) const
{
  while( incrementOrientation(mv) ){
    return true;
  }
  while( incrementPermutation(mv) ){
    return true;
  }
  return false;
}


/*
* Increments the orientation: it makes an increment of a number in base 4.
*/
bool GenericMoveNeighborhoodExplorer::incrementOrientation(Eternity2_GenericMove& mv) const {
  unsigned c = 0;
  while( c < mv.getSize() && mv.getOrientationAt(c) == 3 ){
    c++;
  }
  if( c == mv.getSize() )
    return false;
  mv.setOrientation(c, mv.getOrientationAt(c)+1);
  int d = c-1;
  while(d > -1){
    mv.setOrientation(d, 0);
    d--;
  }
  return true;
}


/*
* Increments the permutation. 
*/
bool GenericMoveNeighborhoodExplorer::incrementPermutation(Eternity2_GenericMove& mv) const {
  unsigned j = mv.getSize();
  unsigned i = mv.getSize() - 1;

  while (i > 0 && mv.getIndexAt(i-1) >= mv.getIndexAt(i)) 
     i--; 
  if (i == 0) //All the elements are in decreasing order
    return false;

  // (i-1) is the first decreasing elements from the end
  while (mv.getIndexAt(j-1) <= mv.getIndexAt(i-1))
    j--;
  mv.swap(i-1, j-1);
  i++;
  j = mv.getSize();

  // revert the elements in the tail of the array
  while (i < j){
      mv.swap(i-1, j-1);
      i++;
      j--;
  }
  //setting to 0 all the orientations
  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setOrientation(c,0);
  }
  return true;
}


/*
* Computes the delta-cost of the move "mv" component by component.
*/
int GenericMoveDeltaCostComponent::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_GenericMove& mv) const
{
  int cost = 0;
  vector<Coord> coords = mv.getCoordinates();
  for(unsigned c = 0; c < coords.size(); c++){
    Coord xy = coords.at(c);
    cost -= singleTileCost(st.getIDOAt(xy), xy, st); //before
    
    IDO new_ido = pair<ID,Orientation>( st.getIDOAt(coords.at(mv.getIndexAt(c))).first, mv.getOrientationAt(c));
    cost += singleTileCost(new_ido, xy, st); //after
  }
  return cost;
}










/*****************************************************************************
 * SingletonMove Neighborhood Explorer Methods
 *****************************************************************************/

/*
* Creates a random move: this is done exploiting the "Fisher-Yates Algorithm" for compute a random permutation.
*/
void SingletonMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_GenericMove& mv) const  throw(EmptyNeighborhood)
{   
    mv.setCoordinates(st.random_singleton);
    mv.createPermutationVector(st.random_singleton.size());
    
    vector<unsigned> rdm_perm = FisherYatesShuffle(st.random_singleton.size());
    
    for(unsigned c = 0; c < mv.getSize(); c++){
      mv.setIndex(c, rdm_perm.at(c));
      mv.setOrientation(c, Random::Int(0,3));
    }
} 



/*
* It creates the first move.
*/
void SingletonMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_GenericMove& mv) const  throw(EmptyNeighborhood)
{
  mv.setCoordinates(st.random_singleton);
  mv.createPermutationVector(st.random_singleton.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, c);
    mv.setOrientation(c,0);
  }
}










/*****************************************************************************
 * EvenChessboardMove Neighborhood Explorer Methods
 *****************************************************************************/

/*
* Creates a random move: this is done exploiting the "Fisher-Yates Algorithm" for compute a random permutation.
* It modifies the vectors "permutation" and "aux_perm" of the object "mv".
*/
void EvenChessboardMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_GenericMove& mv) const  throw(EmptyNeighborhood)
{   
  mv.setCoordinates(st.even_chessboard);
  mv.createPermutationVector(st.even_chessboard.size());
  
  vector<unsigned> rdm_perm = FisherYatesShuffle(st.even_chessboard.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, rdm_perm.at(c));
    mv.setOrientation(c, Random::Int(0,3));
  }
} 



/*
* It creates the first move, choosing from the state "st" the tiles (IDOs) in the coordinates written in the vector "coords".
* This is the original/first permutation, corresponding to 0, 1, ..., N and all the orientations set to 0.
*/
void EvenChessboardMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_GenericMove& mv) const  throw(EmptyNeighborhood)
{
  mv.setCoordinates(st.even_chessboard);
  mv.createPermutationVector(st.even_chessboard.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, c);
    mv.setOrientation(c,0);
  }
}









/*****************************************************************************
 * OddChessboardMove Neighborhood Explorer Methods
 *****************************************************************************/

/*
* Creates a random move: this is done exploiting the "Fisher-Yates Algorithm" for compute a random permutation.
* It modifies the vectors "permutation" and "aux_perm" of the object "mv".
*/
void OddChessboardMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_GenericMove& mv) const  throw(EmptyNeighborhood)
{   
  mv.setCoordinates(st.odd_chessboard);
  mv.createPermutationVector(st.odd_chessboard.size());
    
  vector<unsigned> rdm_perm = FisherYatesShuffle(st.odd_chessboard.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, rdm_perm.at(c));
    mv.setOrientation(c, Random::Int(0,3));
  }
} 



/*
* It creates the first move, choosing from the state "st" the tiles (IDOs) in the coordinates written in the vector "coords".
* This is the original/first permutation, corresponding to 0, 1, ..., N and all the orientations set to 0.
*/
void OddChessboardMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_GenericMove& mv) const  throw(EmptyNeighborhood)
{
  mv.setCoordinates(st.odd_chessboard);
  mv.createPermutationVector(st.odd_chessboard.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, c);
    mv.setOrientation(c,0);
  }
}











/***************************************************************************
 * ThreeTileStreak Move Neighborhood Explorer:
 ***************************************************************************/


// Random move (permutation of the clusters of tiles): unbiased random permutation
// is achieved using Fisher-Yates shuffle algorithm.
void ThreeTileStreakMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_ThreeTileStreakMove& mv) const throw(EmptyNeighborhood)
{
    int i,r;
    vector<pair<unsigned,int>> perm = vector<pair<unsigned,int>>(st.random_tts.size());
    vector<unsigned> rand_perm = FisherYatesShuffle(st.random_tts.size());

    for (i = 0; i < st.random_tts.size(); ++i)
    {
      r = Random::Int(0,1);
      perm[i] = make_pair(rand_perm[i],r);
    }

    mv.setPermutation(perm);
} 




// First permutation of the three-tile streaks, corresponding to the sequence 1,2,...,n, with standard orientation (0).
void ThreeTileStreakMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_ThreeTileStreakMove& mv) const  throw(EmptyNeighborhood)
{
    vector<pair<unsigned,int>> perm = vector<pair<unsigned,int>>(st.random_tts.size());

    for (int i = 0; i < st.random_tts.size(); ++i)
    {
      perm[i] = make_pair(i,0);
    }

    mv.setPermutation(perm);
}

// Next permutation of the streaks. Orientations are incremented first, then comes the actuall permutation of the tiles
bool ThreeTileStreakMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_ThreeTileStreakMove& mv) const
{
    int s = st.random_tts.size();
    int j;
    int i = s-1;

    while (i >= 0 && mv.getTTSOrientation(i) == 1)
    {
        mv.setTTSOrientation(i,0);
        --i;
    }

    if (i >= 0)
    {
        mv.setTTSOrientation(i,1);
    
    } else {

        // Find the first descending value i starting from the bottom of the vector
        for (i = s-2; i >= 0 && mv.getTTSPerm(i) >= mv.getTTSPerm(i+1); --i);

        // If i < 0 this is the final permutation
        if (i < 0) return false;
        
        // Find the first value j smaller than i
        for (j = i+1; j < s && mv.getTTSPerm(j) >= mv.getTTSPerm(i); ++j);

        // Swap i and j-1
        mv.swapPerm(i,j-1);

        // Reverse the subvector [i+1,s-1]
        int begin = i+1, end = s-1;
        while (begin < end)
        {
            mv.swapPerm(begin,end);
            ++begin; --end;
        }
    }

    return true;
}

// Feasibility Check 
bool ThreeTileStreakMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_ThreeTileStreakMove& mv) const
{
    // Generated permutations of clusters of tiles are always feasible
    return true;
} 

//      -------------------                  -------------------
//      |     |     |     |    direct (0)    |     |     |     |
//      |  1  |  c  |  2  |   ----------->   |  1  |  c  |  2  |
//      |     |     |     |                  |     |     |     |
//      -------------------                  -------------------
//         horizontal (0)                       horizontal (0)
//
//      -------------------                  -------------------
//      |     |     |     |    inverse (1)   |     |     |     |
//      |  1  |  c  |  2  |   ----------->   |  2  |  c  |  1  |
//      |     |     |     |                  |     |     |     |
//      -------------------                  -------------------
//         horizontal (0)                       horizontal (0)

void ThreeTileStreakMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_ThreeTileStreakMove& mv) const
{
    vector<pair<IDO,Coord>> changes;
    Coord from,to;
    int from_dir,to_dir;

    for (int i = 0; i < st.random_tts.size(); ++i)
    {
        pair<IDO,Coord> m,m1,m2;

        from = st.random_tts[mv.getTTSPerm(i)].first;
        from_dir = st.random_tts[mv.getTTSPerm(i)].second;
        to = st.random_tts[i].first;
        to_dir = st.random_tts[i].second;

        m = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*mv.getTTSOrientation(i) + 3*from_dir + to_dir) % 4),to);

        from = make_pair(from.first - from_dir,from.second + from_dir - 1);
        to = make_pair(to.first - to_dir + to_dir*2*mv.getTTSOrientation(i),to.second + to_dir - 1 + (1 - to_dir)*2*mv.getTTSOrientation(i));

        m1 = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*mv.getTTSOrientation(i) + 3*from_dir + to_dir) % 4),to);
        
        from = make_pair(from.first + 2*from_dir,from.second + 2 - 2*from_dir);
        to = make_pair(to.first + 2*to_dir - to_dir*4*mv.getTTSOrientation(i),to.second + 2 - 2*to_dir - (1 - to_dir)*4*mv.getTTSOrientation(i));

        m2 = make_pair(make_pair(st.getIDOAt(from).first,(st.getIDOAt(from).second + 2*mv.getTTSOrientation(i) + 3*from_dir + to_dir) % 4),to);

        changes.push_back(m);
        changes.push_back(m1);
        changes.push_back(m2);
    }

    for (int i = 0; i < changes.size(); ++i)
    {
        st.insertTile(changes[i].first,changes[i].second);
    }
}





/*
* Computes the cost of a single tile, given its orientation and a state.
*/
int singleTileCost(IDO ido, Coord crd, const Eternity2_State& st){
  unsigned cost = 0;
  unsigned r = crd.first;
  unsigned c = crd.second;
  for(unsigned cp = 0; cp < 4; cp++){
    Color color_crd = st.getColor(ido,cp);
    //Nord border
    if(r == 0 && cp == 2){
      if( color_crd != 0 )
        cost += 2;
    }
    //Sud border
    if(r == st.getHeight()-1 && cp == 0){
      if( color_crd != 0 )
        cost += 2;
    }
    //Ovest border
    if(c == 0 && cp == 1){
      if( color_crd != 0 )
        cost += 2;
    }
    //Est border
    if(c == st.getWidth()-1 && cp == 3){
      if( color_crd != 0 )
        cost += 2;
    }
        
    //Inner borders
    if(r != st.getHeight()-1 && cp == 0){
      IDO ido_sud = st.getIDOAt( pair<unsigned,unsigned>(r+1,c) );
      Color color_sud = st.getColor(ido_sud,2);
      if( color_crd != color_sud )
        cost++;
    }
    else if(c != 0 && cp == 1){
      IDO ido_ovest = st.getIDOAt( pair<unsigned,unsigned>(r,c-1) );
      Color color_ovest = st.getColor(ido_ovest,3);
      if( color_crd != color_ovest )
        cost++;
    }
    else if(r != 0 && cp == 2){
      IDO ido_nord = st.getIDOAt( pair<unsigned,unsigned>(r-1,c) );
      Color color_nord = st.getColor(ido_nord,0);
      if( color_crd != color_nord )
        cost++;
    }
    else if(c != st.getWidth()-1 && cp == 3){
      IDO ido_est = st.getIDOAt( pair<unsigned,unsigned>(r,c+1) );
      Color color_est = st.getColor(ido_est,1);
      if( color_crd != color_est )
        cost++;
    }
  }
  return cost;
}






/*
* Implementation of the "Fisher-Yates Algorithm". It generates a unbiased permutation of the vector "hat".
*/
vector<unsigned> FisherYatesShuffle(unsigned sz) {
  vector<unsigned> hat = vector<unsigned>(sz);
  for(unsigned c = 0; c < sz; c++){
    hat.at(c) = c;
  }
  unsigned j;
  for(unsigned i = sz-1; i > 0; i--){
    j = Random::Int(0,i);
    swap(hat.at(i), hat.at(j));
  }
  return hat;
}



