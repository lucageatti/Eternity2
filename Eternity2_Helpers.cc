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

/*
* Translates a output object into a state object
*/
void Eternity2_OutputManager::InputState(Eternity2_State& st, const Eternity2_Output& out) const 
{
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
 * SingletonMove Delta Cost Component Methods
 *****************************************************************************/

/*
* Computes the delta-cost of the move "mv" component by component.
*/
int SingletonMoveDeltaCostComponent::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_SingletonMove& mv) const
{
  int cost = 0;
  vector<Coord> coords = mv.getCoordinates();
  for(unsigned c = 0; c < coords.size(); c++){
    Coord xy = coords.at(c);
    cost -= deltaSingleTileCost(st.getIDOAt(xy), xy, st); //before
    
    IDO new_ido = pair<ID,Orientation>( st.getIDOAt(coords.at(mv.getIndexAt(c))).first, mv.getOrientationAt(c));
    cost += deltaSingleTileCost(new_ido, xy, st); //after
  }
  return cost;
}


/*
* Computes the cost of a single tile, given its orientation and a state.
*/
int SingletonMoveDeltaCostComponent::deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) const {
  unsigned cost = 0;
  unsigned r = crd.first;
  unsigned c = crd.second;
  for(unsigned cp = 0; cp < 4; cp++){
    Color color_crd = st.getColor(ido,cp);
    //Nord border
    if(r == 0 && cp == 2){
      if( color_crd != 0 )
        cost++;
    }
    //Sud border
    if(r == st.getHeight()-1 && cp == 0){
      if( color_crd != 0 )
        cost++;
    }
    //Ovest border
    if(c == 0 && cp == 1){
      if( color_crd != 0 )
        cost++;
    }
    //Est border
    if(c == st.getWidth()-1 && cp == 3){
      if( color_crd != 0 )
        cost++;
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






/*****************************************************************************
 * SingletonMove Neighborhood Explorer Methods
 *****************************************************************************/

/*
* The move is always feasible.
*/
bool SingletonMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_SingletonMove& mv) const
{
  return true;
}


/*
* Update the state according to the move.
*/
void SingletonMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_SingletonMove& mv) const
{
  updateCoords(st);
  vector<Coord> coords = mv.getCoordinates();
  vector<tileMove> changes = vector<tileMove>(coords.size());
  for(unsigned c = 0; c < coords.size(); c++){
    changes[c] = make_pair(make_pair(st.getIDOAt( coords[mv.getIndexAt(c)] ).first , mv.getOrientationAt(c)) , coords[c]);
  }
  for(unsigned c = 0; c < changes.size(); c++){
    st.insertTile(changes[c].first, changes[c].second);
  }
}


/*
* It computes the next move, computing the next permutation of the indexes 0, 1, ..., N.
*/
bool SingletonMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_SingletonMove& mv) const
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
* It exploits the "Hungarian Algorithm" to compute the perfect matching and to create the best move.
*/
EvaluatedMove<Eternity2_SingletonMove, DefaultCostStructure<int>> SingletonMoveNeighborhoodExplorer::SelectBest(const Eternity2_State& st, size_t& explored, const MoveAcceptor& AcceptMove, const std::vector<double>& weights) const throw (EmptyNeighborhood){
  //Creation of the move
  Eternity2_SingletonMove mv = Eternity2_SingletonMove();
  mv.setCoordinates(st.random_singleton);
  //forcing the update
  forceUpdate(st);
  //creating the graph
  vector<vector<pair<int,Orientation>>> graph = createGraph(st, mv);

  //calling the hungarian algorithm
  vector<int> match = hungarianAlgorithm(graph);
  //creating the move
  createMove(mv, match, graph);
  //Creation of Evaluated Move
  return EvaluatedMove<Eternity2_SingletonMove, DefaultCostStructure<int>>(mv, DefaultCostStructure<int>());
}

/*
* Creates a random move: this is done exploiting the "Fisher-Yates Algorithm" for compute a random permutation.
*/
void SingletonMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_SingletonMove& mv) const  throw(EmptyNeighborhood)
{
    mv.setCoordinates(st.random_singleton);
    mv.createPermutationVector(st.random_singleton.size());
    
    vector<unsigned> rdm_perm = FisherYatesShuffle(st.random_singleton.size());
    
    for(unsigned c = 0; c < mv.getSize(); c++){
      mv.setIndex(c, rdm_perm.at(c));
      mv.setOrientation(c, Random::Int(0,3));
    }

    st.singleton_counter++;
}


/*
* It creates the first move.
*/
void SingletonMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_SingletonMove& mv) const  throw(EmptyNeighborhood)
{
  mv.setCoordinates(st.random_singleton);
  mv.createPermutationVector(st.random_singleton.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, c);
    mv.setOrientation(c,0);
  }
  st.singleton_counter++;
}


/*
* Increments the orientation: it makes an increment of a number in base 4.
*/
bool SingletonMoveNeighborhoodExplorer::incrementOrientation(Eternity2_SingletonMove& mv) const {
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
bool SingletonMoveNeighborhoodExplorer::incrementPermutation(Eternity2_SingletonMove& mv) const {
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
* 
*/
vector<vector<pair<int,Orientation>>> SingletonMoveNeighborhoodExplorer::createGraph(const Eternity2_State& st, Eternity2_SingletonMove& mv) const{
  pair<int,Orientation> best_weight;
  vector<Coord> mv_coords = mv.getCoordinates();
  //declaring the matrix
  vector<vector<pair<int,Orientation>>> graph(mv_coords.size());
  //filling the matrix
  for(int r = 0; r < graph.size(); ++r){
    graph[r] = vector<pair<int,Orientation>>(graph.size());
    IDO old_tile = st.getIDOAt(mv_coords[r]);
    for(int c = 0; c < graph.size(); ++c){
      old_tile.second = 0;
      best_weight.first = deltaSingleTileCost(old_tile, mv_coords[c], st);
      best_weight.second = 0;
      int new_stl;
      for(int i = 1; i < 4; i++){
        old_tile.second = i;
        new_stl = deltaSingleTileCost(old_tile, mv_coords[c], st);
        if( new_stl < best_weight.first ){ 
          best_weight.first = new_stl;
          best_weight.second = i;
        }
      }
      graph[r][c] = best_weight;
    }
  }
  return graph;
}


/*
* 
*/
void SingletonMoveNeighborhoodExplorer::updateCoords(Eternity2_State& st) const {
  if( st.singleton_counter % 10 == 0 )
    st.singletonRandomCoords();
}


/*
*
*/
void SingletonMoveNeighborhoodExplorer::forceUpdate(const Eternity2_State& st) const {
  st.singleton_counter = 0;
}


/*
*
*/
void SingletonMoveNeighborhoodExplorer::createMove(Eternity2_SingletonMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>>& graph) const {
  mv.createPermutationVector(match.size());
  for(int i = 0; i < match.size(); ++i){
    mv.setIndex(match[i], i);
    mv.setOrientation(match[i], graph[i][match[i]].second);
  }
}











/*****************************************************************************
 * EvenChessBoardMove Delta Cost Component Methods
 *****************************************************************************/

/*
* Computes the delta-cost of the move "mv" component by component.
*/
int EvenChessboardMoveDeltaCostComponent::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_EvenChessboardMove& mv) const
{
  int cost = 0;
  vector<Coord> coords = mv.getCoordinates();
  for(unsigned c = 0; c < coords.size(); c++){
    Coord xy = coords.at(c);
    cost -= deltaSingleTileCost(st.getIDOAt(xy), xy, st); //before
    
    IDO new_ido = pair<ID,Orientation>( st.getIDOAt(coords.at(mv.getIndexAt(c))).first, mv.getOrientationAt(c));
    cost += deltaSingleTileCost(new_ido, xy, st); //after
  }
  return cost;
}


/*
* Computes the cost of a single tile, given its orientation and a state.
*/
int EvenChessboardMoveDeltaCostComponent::deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) const {
  unsigned cost = 0;
  unsigned r = crd.first;
  unsigned c = crd.second;
  for(unsigned cp = 0; cp < 4; cp++){
    Color color_crd = st.getColor(ido,cp);
    //Nord border
    if(r == 0 && cp == 2){
      if( color_crd != 0 )
        cost++;
    }
    //Sud border
    if(r == st.getHeight()-1 && cp == 0){
      if( color_crd != 0 )
        cost++;
    }
    //Ovest border
    if(c == 0 && cp == 1){
      if( color_crd != 0 )
        cost++;
    }
    //Est border
    if(c == st.getWidth()-1 && cp == 3){
      if( color_crd != 0 )
        cost++;
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





/*****************************************************************************
 * EvenChessBoard Neighborhood Explorer Methods
 *****************************************************************************/

/*
* The move is always feasible.
*/
bool EvenChessboardMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_EvenChessboardMove& mv) const
{
  return true;
}


/*
* Update the state according to the move.
*/
void EvenChessboardMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_EvenChessboardMove& mv) const
{
  vector<Coord> coords = mv.getCoordinates();
  vector<tileMove> changes = vector<tileMove>(coords.size());
  for(unsigned c = 0; c < coords.size(); c++){
    changes[c] = make_pair(make_pair(st.getIDOAt( coords[mv.getIndexAt(c)] ).first , mv.getOrientationAt(c)) , coords[c]);
  }
  for(unsigned c = 0; c < changes.size(); c++){
    st.insertTile(changes[c].first, changes[c].second);
  }
}



/*
* It computes the next move, computing the next permutation of the indexes 0, 1, ..., N.
*/
bool EvenChessboardMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_EvenChessboardMove& mv) const
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
* It exploits the "Hungarian Algorithm" to compute the perfect matching and to create the best move.
*/
EvaluatedMove<Eternity2_EvenChessboardMove, DefaultCostStructure<int>> EvenChessboardMoveNeighborhoodExplorer::SelectBest(const Eternity2_State& st, size_t& explored, const MoveAcceptor& AcceptMove, const std::vector<double>& weights) const throw (EmptyNeighborhood){
  //Creation of the move
  Eternity2_EvenChessboardMove mv = Eternity2_EvenChessboardMove();
  mv.setCoordinates(st.even_chessboard);
  //forcing the update
  //forceUpdate(st);
  //creating the graph
  vector<vector<pair<int,Orientation>>> graph = createGraph(st, mv);

  //calling the hungarian algorithm
  vector<int> match = hungarianAlgorithm(graph);
  //creating the move
  createMove(mv, match, graph);
  //Creation of Evaluated Move
  return EvaluatedMove<Eternity2_EvenChessboardMove, DefaultCostStructure<int>>(mv, DefaultCostStructure<int>());
}


/*
* Creates a random move: this is done exploiting the "Fisher-Yates Algorithm" for compute a random permutation.
* It modifies the vectors "permutation" and "aux_perm" of the object "mv".
*/
void EvenChessboardMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_EvenChessboardMove& mv) const  throw(EmptyNeighborhood)
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
void EvenChessboardMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_EvenChessboardMove& mv) const  throw(EmptyNeighborhood)
{
  mv.setCoordinates(st.even_chessboard);
  mv.createPermutationVector(st.even_chessboard.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, c);
    mv.setOrientation(c,0);
  }
}


/*
* Increments the orientation: it makes an increment of a number in base 4.
*/
bool EvenChessboardMoveNeighborhoodExplorer::incrementOrientation(Eternity2_EvenChessboardMove& mv) const {
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
bool EvenChessboardMoveNeighborhoodExplorer::incrementPermutation(Eternity2_EvenChessboardMove& mv) const {
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
* 
*/
vector<vector<pair<int,Orientation>>> EvenChessboardMoveNeighborhoodExplorer::createGraph(const Eternity2_State& st, Eternity2_EvenChessboardMove& mv) const{
  pair<int,Orientation> best_weight;
  vector<Coord> mv_coords = mv.getCoordinates();
  //declaring the matrix
  vector<vector<pair<int,Orientation>>> graph(mv_coords.size());
  //filling the matrix
  for(int r = 0; r < graph.size(); ++r){
    graph[r] = vector<pair<int,Orientation>>(graph.size());
    IDO old_tile = st.getIDOAt(mv_coords[r]);
    for(int c = 0; c < graph.size(); ++c){
      old_tile.second = 0;
      best_weight.first = deltaSingleTileCost(old_tile, mv_coords[c], st);
      best_weight.second = 0;
      int new_stl;
      for(int i = 1; i < 4; i++){
        old_tile.second = i;
        new_stl = deltaSingleTileCost(old_tile, mv_coords[c], st);
        if( new_stl < best_weight.first ){ 
          best_weight.first = new_stl;
          best_weight.second = i;
        }
      }
      graph[r][c] = best_weight;
    }
  }
  return graph;
}




/*
*
*/
void EvenChessboardMoveNeighborhoodExplorer::createMove(Eternity2_EvenChessboardMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>>& graph) const {
  mv.createPermutationVector(match.size());
  for(int i = 0; i < match.size(); ++i){
    mv.setIndex(match[i], i);
    mv.setOrientation(match[i], graph[i][match[i]].second);
  }
}












/*****************************************************************************
 * OddChessBoardMove Delta Cost Component Methods
 *****************************************************************************/

/*
* Computes the delta-cost of the move "mv" component by component.
*/
int OddChessboardMoveDeltaCostComponent::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_OddChessboardMove& mv) const
{
  int cost = 0;
  vector<Coord> coords = mv.getCoordinates();
  for(unsigned c = 0; c < coords.size(); c++){
    Coord xy = coords.at(c);
    cost -= deltaSingleTileCost(st.getIDOAt(xy), xy, st); //before
    
    IDO new_ido = pair<ID,Orientation>( st.getIDOAt(coords.at(mv.getIndexAt(c))).first, mv.getOrientationAt(c));
    cost += deltaSingleTileCost(new_ido, xy, st); //after
  }
  return cost;
}


/*
* Computes the cost of a single tile, given its orientation and a state.
*/
int OddChessboardMoveDeltaCostComponent::deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) const {
  unsigned cost = 0;
  unsigned r = crd.first;
  unsigned c = crd.second;
  for(unsigned cp = 0; cp < 4; cp++){
    Color color_crd = st.getColor(ido,cp);
    //Nord border
    if(r == 0 && cp == 2){
      if( color_crd != 0 )
        cost++;
    }
    //Sud border
    if(r == st.getHeight()-1 && cp == 0){
      if( color_crd != 0 )
        cost++;
    }
    //Ovest border
    if(c == 0 && cp == 1){
      if( color_crd != 0 )
        cost++;
    }
    //Est border
    if(c == st.getWidth()-1 && cp == 3){
      if( color_crd != 0 )
        cost++;
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







/*****************************************************************************
 * OddChessBoard Neighborhood Explorer Methods
 *****************************************************************************/

/*
* The move is always feasible.
*/
bool OddChessboardMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_OddChessboardMove& mv) const
{
  return true;
}


/*
* Update the state according to the move.
*/
void OddChessboardMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_OddChessboardMove& mv) const
{
  vector<Coord> coords = mv.getCoordinates();
  vector<tileMove> changes = vector<tileMove>(coords.size());
  for(unsigned c = 0; c < coords.size(); c++){
    changes[c] = make_pair(make_pair(st.getIDOAt( coords[mv.getIndexAt(c)] ).first , mv.getOrientationAt(c)) , coords[c]);
  }
  for(unsigned c = 0; c < changes.size(); c++){
    st.insertTile(changes[c].first, changes[c].second);
  }
}


/*
* It computes the next move, computing the next permutation of the indexes 0, 1, ..., N.
*/
bool OddChessboardMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_OddChessboardMove& mv) const
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
* It exploits the "Hungarian Algorithm" to compute the perfect matching and to create the best move.
*/
EvaluatedMove<Eternity2_OddChessboardMove, DefaultCostStructure<int>> OddChessboardMoveNeighborhoodExplorer::SelectBest(const Eternity2_State& st, size_t& explored, const MoveAcceptor& AcceptMove, const std::vector<double>& weights) const throw (EmptyNeighborhood){
  //Creation of the move
  Eternity2_OddChessboardMove mv = Eternity2_OddChessboardMove();
  mv.setCoordinates(st.odd_chessboard);
  //forcing the update
  //forceUpdate(st);
  //creating the graph
  vector<vector<pair<int,Orientation>>> graph = createGraph(st, mv);

  //calling the hungarian algorithm
  vector<int> match = hungarianAlgorithm(graph);
  //creating the move
  createMove(mv, match, graph);
  //Creation of Evaluated Move
  return EvaluatedMove<Eternity2_OddChessboardMove, DefaultCostStructure<int>>(mv, DefaultCostStructure<int>());
}


/*
* Creates a random move: this is done exploiting the "Fisher-Yates Algorithm" for compute a random permutation.
* It modifies the vectors "permutation" and "aux_perm" of the object "mv".
*/
void OddChessboardMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_OddChessboardMove& mv) const  throw(EmptyNeighborhood)
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
void OddChessboardMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_OddChessboardMove& mv) const  throw(EmptyNeighborhood)
{
  mv.setCoordinates(st.odd_chessboard);
  mv.createPermutationVector(st.odd_chessboard.size());

  for(unsigned c = 0; c < mv.getSize(); c++){
    mv.setIndex(c, c);
    mv.setOrientation(c,0);
  }
}


/*
* Increments the orientation: it makes an increment of a number in base 4.
*/
bool OddChessboardMoveNeighborhoodExplorer::incrementOrientation(Eternity2_OddChessboardMove& mv) const {
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
bool OddChessboardMoveNeighborhoodExplorer::incrementPermutation(Eternity2_OddChessboardMove& mv) const {
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
* 
*/
vector<vector<pair<int,Orientation>>> OddChessboardMoveNeighborhoodExplorer::createGraph(const Eternity2_State& st, Eternity2_OddChessboardMove& mv) const{
  pair<int,Orientation> best_weight;
  vector<Coord> mv_coords = mv.getCoordinates();
  //declaring the matrix
  vector<vector<pair<int,Orientation>>> graph(mv_coords.size());
  //filling the matrix
  for(int r = 0; r < graph.size(); ++r){
    graph[r] = vector<pair<int,Orientation>>(graph.size());
    IDO old_tile = st.getIDOAt(mv_coords[r]);
    for(int c = 0; c < graph.size(); ++c){
      old_tile.second = 0;
      best_weight.first = deltaSingleTileCost(old_tile, mv_coords[c], st);
      best_weight.second = 0;
      int new_stl;
      for(int i = 1; i < 4; i++){
        old_tile.second = i;
        new_stl = deltaSingleTileCost(old_tile, mv_coords[c], st);
        if( new_stl < best_weight.first ){ 
          best_weight.first = new_stl;
          best_weight.second = i;
        }
      }
      graph[r][c] = best_weight;
    }
  }
  return graph;
}



/*
*
*/
void OddChessboardMoveNeighborhoodExplorer::createMove(Eternity2_OddChessboardMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>>& graph) const {
  mv.createPermutationVector(match.size());
  for(int i = 0; i < match.size(); ++i){
    mv.setIndex(match[i], i);
    mv.setOrientation(match[i], graph[i][match[i]].second);
  }
}













/***************************************************************************
 * ThreeTileStreak Move Neighborhood Explorer:
 ***************************************************************************/


// Random move (permutation of the clusters of tiles): unbiased random permutation
// is achieved using Fisher-Yates shuffle algorithm.
void ThreeTileStreakMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_ThreeTileStreakMove& mv) const throw(EmptyNeighborhood)
{
    // Setting (state) coordinates in the current move instance;
    mv.setCoordinates(st.random_tts);
    int i,r;
    vector<pair<unsigned,int>> perm(st.random_tts.size());

    // Calling Fisher-Yates shuffle algorithm
    vector<unsigned> rand_perm = FisherYatesShuffle(st.random_tts.size());

    for (i = 0; i < st.random_tts.size(); ++i)
    {
      // Generating Direct/Inverse tts orientation
      r = Random::Int(0,1);
      perm[i] = make_pair(rand_perm[i],r);
    }

    mv.setPermutation(perm);

    // Increasing tts counter to refresh the random coordinate selection
    st.tts_counter++;
} 

// First permutation of the three-tile streaks, corresponding to the sequence 1,2,...,n, with standard orientation (0).
void ThreeTileStreakMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_ThreeTileStreakMove& mv) const  throw(EmptyNeighborhood)
{
    // Setting (state) coordinates in the current move instance;
    mv.setCoordinates(st.random_tts);
    vector<pair<unsigned,int>> perm = vector<pair<unsigned,int>>(st.random_tts.size());

    for (int i = 0; i < st.random_tts.size(); ++i)
    {
      perm[i] = make_pair(i,0);
    }

    mv.setPermutation(perm);

    // Increasing tts counter to refresh the random coordinate selection
    st.tts_counter++;
}

// Next permutation of the streaks. Orientations are incremented first, then comes the actuall permutation of the tiles
bool ThreeTileStreakMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_ThreeTileStreakMove& mv) const
{
    int s = st.random_tts.size();
    int j;
    int i = s-1;

    // Increasing orientation (basically incrementing a binary number)
    while (i >= 0 && mv.getTTSOrientation(i) == 1)
    {
        mv.setTTSOrientation(i,0);
        --i;
    }

    // Incrementing permutation (if not reached the end) using 
    // the algorithm discussed during class
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
//
//            -------                              -------
//            |     |                              |     |
//            |  1  |                              |  1  |
//            |     |                              |     |
//            -------                              -------
//            |     |          direct (0)          |     |
//            |  c  |         ----------->         |  c  |
//            |     |                              |     |
//            -------                              -------
//            |     |                              |     |
//            |  2  |                              |  2  |
//            |     |                              |     |
//            -------                              -------
//          vertical (1)                         vertical (1)
//
//            -------                              -------
//            |     |                              |     |
//            |  1  |                              |  2  |
//            |     |                              |     |
//            -------                              -------
//            |     |          inverse (1)         |     |
//            |  c  |         ----------->         |  c  |
//            |     |                              |     |
//            -------                              -------
//            |     |                              |     |
//            |  2  |                              |  1  |
//            |     |                              |     |
//            -------                              -------
//          vertical (1)                         vertical (1)
//
//                                                 -------
//                                                 |     |
//                                                 |  1  |
//                                                 |     |
//      -------------------                        -------
//      |     |     |     |    direct (0)          |     |
//      |  1  |  c  |  2  |   ----------->         |  c  |
//      |     |     |     |                        |     |
//      -------------------                        -------
//                                                 |     |
//                                                 |  2  |
//                                                 |     |
//                                                 -------
//         horizontal (0)                        vertical (1)
//
//                                                 -------
//                                                 |     |
//                                                 |  2  |
//                                                 |     |
//      -------------------                        -------
//      |     |     |     |    inverse (1)         |     |
//      |  1  |  c  |  2  |   ----------->         |  c  |
//      |     |     |     |                        |     |
//      -------------------                        -------
//                                                 |     |
//                                                 |  1  |
//                                                 |     |
//                                                 -------
//         horizontal (0)                        vertical (1)
//
//            -------
//            |     |
//            |  1  |
//            |     |
//            -------                        -------------------
//            |     |          direct (0)    |     |     |     |
//            |  c  |         ----------->   |  1  |  c  |  2  |
//            |     |                        |     |     |     |
//            -------                        -------------------
//            |     |
//            |  2  |
//            |     |
//            -------
//          vertical (1)                        horizontal (0)
//
//            -------
//            |     |
//            |  2  |
//            |     |
//            -------                        -------------------
//            |     |          inverse (1)   |     |     |     |
//            |  c  |         ----------->   |  1  |  c  |  2  |
//            |     |                        |     |     |     |
//            -------                        -------------------
//            |     |
//            |  1  |
//            |     |
//            -------
//          vertical (1)                        horizontal (0)
//

// 'MakeMove' will convert tts moves in simple tile moves,
// keeping the relation between tiles from the same tts selection.
void ThreeTileStreakMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_ThreeTileStreakMove& mv) const
{
    int i;
    auto perm = mv.getPermutation();
    vector<tuple<tileMove,tileMove,tileMove,int>> changes(perm.size());

    // Disassembling the tts moves
    for (i = 0; i < perm.size(); ++i)
    {
        changes[i] = mv.computeSimpleMove(st,perm[i],i);
    }

    // Appling move changes
    for (i = 0; i < changes.size(); ++i)
    {
        st.insertTile(std::get<0>(changes[i]).first,std::get<0>(changes[i]).second);
        st.insertTile(std::get<1>(changes[i]).first,std::get<1>(changes[i]).second);
        st.insertTile(std::get<2>(changes[i]).first,std::get<2>(changes[i]).second);
    }

    // Updates coordinates if needed.
    updateCoords(st);
}

// 'SelectBest' definition. This should be 'BestMove' but, due to same recent
// framework changes we had to use 'SelectBest' as a placeholder.
EvaluatedMove<Eternity2_ThreeTileStreakMove, DefaultCostStructure<int>> ThreeTileStreakMoveNeighborhoodExplorer::SelectBest(const Eternity2_State& st, size_t& explored, const MoveAcceptor& AcceptMove, const std::vector<double>& weights) const throw (EmptyNeighborhood)
{
    Eternity2_ThreeTileStreakMove mv = Eternity2_ThreeTileStreakMove();

    // Setting (state) coordinates in the current move instance;
    mv.setCoordinates(st.random_tts);
    
    unsigned i;
    vector<pair<unsigned,int>> perm(st.random_tts.size());

    // Creating the graph
    vector<vector<pair<int,Orientation>>> graph = createGraph(st, mv);
    
    // Calling the hungarian algorithm
    //cout << "Calling the hungarian algorithm..." << endl;
    vector<int> match = hungarianAlgorithm(graph);
    
    // Creating the move
    //cout << "Creating the move..." << endl;
    for (i = 0; i < match.size(); ++i)
    {
      perm[match[i]] = make_pair(i,graph[i][match[i]].second);
    }
    mv.setPermutation(perm);
    cout << mv << endl;
    
    // Forces the coordinates update during the next 'MakeMove'
    forceUpdate(st);

    return EvaluatedMove<Eternity2_ThreeTileStreakMove, DefaultCostStructure<int>>(mv, DefaultCostStructure<int>());
}

// Hungarian algorithm auxiliary method.
// Creates a recombination graph as explained in the report
vector<vector<pair<int,Orientation>>> ThreeTileStreakMoveNeighborhoodExplorer::createGraph(const Eternity2_State& st ,Eternity2_ThreeTileStreakMove& mv) const
{
    int i,j;
    int cost_0,cost_1;
    int size = st.random_tts.size();
    vector<vector<pair<int,Orientation>>> graph(size,vector<pair<int,Orientation>>(size));

    for (i = 0; i < size; ++i)
    {
        for (j = 0; j < size; ++j)
        {
            // Takes the best among the two possible Direct/Inverse orientations
            cost_0 = computeTTSDeltaCost(st, mv.computeSimpleMove(st, make_pair((unsigned int)i,0), j), false);
            cost_1 = computeTTSDeltaCost(st, mv.computeSimpleMove(st, make_pair((unsigned int)i,1), j), false);
            
            graph[i][j] = (cost_0 < cost_1) ? make_pair(cost_0,0) : make_pair(cost_1,1);
        }
    }

    return graph;
}

// Forces the coordinates update during the next 'MakeMove'
void ThreeTileStreakMoveNeighborhoodExplorer::forceUpdate(const Eternity2_State& st) const {
  st.tts_counter = 0;
}

// Updates coordinates if needed.
void ThreeTileStreakMoveNeighborhoodExplorer::updateCoords(Eternity2_State& st) const
{
    if( !(st.tts_counter %= 10) )
      st.ttsRandomCoords();
}


// Checks for color violation, given a 'tileMove' and a cardinal point.
int checkColor(const Eternity2_State& st, tileMove m, CardinalPoint cp, bool delta = true)
{
    int cost = 0;
    int adj_color = 0; // "= 0" to avoid warnings
    Coord c;

    switch (cp)
    {
      case NORTH: 
          if (m.second.first == 0)
          {
              adj_color = 0;
          
          } else {

              c = make_pair(m.second.first - 1,m.second.second);
              adj_color = st.getColor(st.getIDOAt(c),SOUTH);
          }
          break;

      case SOUTH: 
          if (m.second.first + 1 >= st.getHeight())
          {
              adj_color = 0;
          
          } else {

              c = make_pair(m.second.first + 1,m.second.second);
              adj_color = st.getColor(st.getIDOAt(c),NORTH);
          }
          break;

      case WEST:
          if (m.second.second == 0)
          {
              adj_color = 0;
          
          } else {

              c = make_pair(m.second.first,m.second.second - 1);
              adj_color = st.getColor(st.getIDOAt(c),EAST);
          }
          break;

      case EAST: 
          if (m.second.second + 1 >= st.getWidth())
          {
              adj_color = 0;
          
          } else {

              c = make_pair(m.second.first,m.second.second + 1);
              adj_color = st.getColor(st.getIDOAt(c),WEST);
          }
          break;

      default:
          std::cerr << "Invalid Cardinal Point: " << cp;
    }

    cost += (st.getColor(m.first,cp) != adj_color);
    if (delta) cost -= (st.getColor(st.getIDOAt(m.second),cp) != adj_color);

    return cost;
}


// Computes the delta-cost of a TTS move. The method first converts tts moves into
// simple tile-wise moves, and then checks all the cardinal points involved (without
// checking internal violations).
int ThreeTileStreakMoveDeltaCostComponent::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_ThreeTileStreakMove& mv) const
{
    int i,cost = 0;
    vector<pair<unsigned,int>> perm = mv.getPermutation();

    for (i = 0; i < perm.size(); ++i)
    {
        cost += computeTTSDeltaCost(st,mv.computeSimpleMove(st,perm[i],i));
    }

  return cost;
}

// Computes delta cost, keeping in mind the orientation of the tts selection.
int computeTTSDeltaCost(const Eternity2_State& st, const tuple<tileMove,tileMove,tileMove,int>& single_move, bool delta)
{
    int cost = 0;

    if(std::get<3>(single_move))
    {
        // Streak vertically oriented

        ////// Upper tile //////
        // NORTH
        cost += checkColor(st, std::get<0>(single_move), NORTH, delta);
        // WEST
        cost += checkColor(st, std::get<0>(single_move), WEST, delta);
        // EAST
        cost += checkColor(st, std::get<0>(single_move), EAST, delta);
        
        ////// Middle tile //////
        // WEST
        cost += checkColor(st, std::get<1>(single_move), WEST, delta);
        // EAST
        cost += checkColor(st, std::get<1>(single_move), EAST, delta);

        ////// Lower tile //////
        // WEST
        cost += checkColor(st, std::get<2>(single_move), WEST, delta);
        // EAST
        cost += checkColor(st, std::get<2>(single_move), EAST, delta);
        // SOUTH
        cost += checkColor(st, std::get<2>(single_move), SOUTH, delta);
        
      } else {

        // Streak horizontally oriented

        ////// Left Tile //////
        // NORTH
        cost += checkColor(st, std::get<0>(single_move), NORTH, delta);
        // WEST
        cost += checkColor(st, std::get<0>(single_move), WEST, delta);
        // SOUTH
        cost += checkColor(st, std::get<0>(single_move), SOUTH, delta);
                  
        ////// Middle Tile //////
        // NORTH
        cost += checkColor(st, std::get<1>(single_move), NORTH, delta);
        // SOUTH
        cost += checkColor(st, std::get<1>(single_move), SOUTH, delta);
        
        ////// Right Tile //////
        // NORTH
        cost += checkColor(st, std::get<2>(single_move), NORTH, delta);
        // EAST
        cost += checkColor(st, std::get<2>(single_move), EAST, delta);
        // SOUTH
        cost += checkColor(st, std::get<2>(single_move), SOUTH, delta);
      }

      return cost;
}


/**************************************************************************************************
***************************************************************************************************
* L Move
*
* @author Elia
**************************************************************************************************
*************************************************************************************************/

// initial move builder
void Eternity2_LMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_LMove& mv) const  throw(EmptyNeighborhood)
{
  cout << "<RandomMove>" << endl;
  // insert the code that writes a random move on mv in state st
  // shuffle partition
  mv.setCoordinates(st.random_L);
  mv.ellSelection = FisherYatesShuffle(mv.ellList.size());
  st.L_counter++;
  cout << "</RandomMove>" << endl;
} 

// check move feasibility
bool Eternity2_LMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_LMove& mv) const
{
  // Insert the code that check is move mv is legal in state st 
  // (return true if legal, false otherwise)
  
  //if(mv.ellSelection.size() < 2) return false;

  // Check that elements are not repeated
  vector<unsigned> temp = vector<unsigned>(mv.ellSelection.size());
  for(unsigned i = 0; i < temp.size(); i++){
	  unsigned sel = mv.ellSelection.at(i);
	  if(temp.at(sel) != 0){
		  return false;
	  }
	  temp.at(sel) = sel;
  }
  
  // Maybe we should check that the generated L-partition is good. (but not here)
  return true;
} 

// update the state according to the move 
void Eternity2_LMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_LMove& mv) const
{
  // Insert the code that modifies the state st based on the application of move mv
  // If ellSelection[i]=j, then I move Lj in place of Li.

  //unsigned cols = st.getWidth();
  cout << "<MakeMove>" << endl;
  vector<pair<IDO,Coord>> changes;
  for(unsigned i = 0; i < mv.ellSelection.size(); i++){
    unsigned from = mv.ellSelection[i];
	  unsigned i1 = mv.ellList.at(i).first.first;
    unsigned j1 = mv.ellList.at(i).first.second;
    unsigned i2 = mv.ellList.at(from).first.first;
    unsigned j2 = mv.ellList.at(from).first.second;

    if (!(i1 < st.getHeight()-1 && i2 < st.getHeight()-1 && j1 < st.getWidth()-1 && j2 < st.getWidth()-1))
    {
      continue;
    }

    // Backup IDOs
    IDO eLstIDO[]= { 
        st.getIDOAt(pair<unsigned,unsigned>(i1,j1)), 
        st.getIDOAt(pair<unsigned,unsigned>(i1,j1+1)),
        st.getIDOAt(pair<unsigned,unsigned>(i1+1,j1+1)), 
        st.getIDOAt(pair<unsigned,unsigned>(i1+1,j1)) 
    };
    IDO eSelIDO[]= { 
        st.getIDOAt(pair<unsigned,unsigned>(i2,j2)), 
        st.getIDOAt(pair<unsigned,unsigned>(i2,j2+1)),
        st.getIDOAt(pair<unsigned,unsigned>(i2+1,j2+1)),
        st.getIDOAt(pair<unsigned,unsigned>(i2+1,j2)) 
    };
    // Backup coordinates
    Coord eLstCoord[] = { 
        pair<unsigned,unsigned>(i1,j1), 
        pair<unsigned,unsigned>(i1,j1+1), 
        pair<unsigned,unsigned>(i1+1,j1+1), 
        pair<unsigned,unsigned>(i1+1,j1) 
    };
    Coord eSelCoord[] = { 
        pair<unsigned,unsigned>(i2,j2), 
        pair<unsigned,unsigned>(i2,j2+1), 
        pair<unsigned,unsigned>(i2+1,j2+1), 
        pair<unsigned,unsigned>(i2+1,j2) 
    };
    cout << "mkmv 1" << endl;
    // Calculate the rotation  needed to fit ellList[i] in place of ellList[from]
    // Positive rotation = counter-clockwise rotation of the L
    int rot = mv.ellList.at(i).second - mv.ellList.at(from).second;

    // First off map the first L into the second 
    // This means that, for each cell of ellList[i], I store the index of the cell of
    // ellList[from] that is moved in its place.
    unsigned map[4];
    for (unsigned i = 0; i < 4; ++i)
    {
      int val = st.strangeMod(i-rot,4);
      map[i] = (unsigned int)val;
    }

	  cout << "mkmv 2" << endl;

	  // Store the changes needed for the swap
	  /* Swap Example:
	   *
	   *   | |#| = |0|1| --> |#| | = |3|0| 
	   *   |#|#| = |3|2| --> |#|#| = |2|1|
	   *
	   * I do a clockwise rotation of 1 (respectively -1), also rotating each individual cell.
	   */
	  for(unsigned j = 0; j<3; j++){
      // Single cell orientation is clockwise
		  int newRot = st.strangeMod(eSelIDO[map[j]].second-rot,4);
      
		  changes.push_back( pair<IDO,Coord>( pair<int,int>(eSelIDO[map[j]].first, newRot), eLstCoord[j] ) );
	  }

  }

  cout << "mkmv 3" << endl;

  // Apply changes
  for (int i = 0; i < changes.size(); ++i)
  {
    st.insertTile( changes[i].first, changes[i].second );
  }

  cout << "mkmv 4" << endl;

  updateCoords(st);
  cout << "</MakeMove>" << endl;
}  


void Eternity2_LMoveNeighborhoodExplorer::updateCoords(Eternity2_State& st) const {
  if( st.L_counter % 10 == 0 )
    st.LRandomCoords();
}



void Eternity2_LMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_LMove& mv) const  throw(EmptyNeighborhood)
{
  // Insert the code the generate the first move in the neighborhood and store it in mv
  // The list of ells in the partition is produced.
  //cout << "<FirstMove>" << endl;
  mv.setCoordinates(st.random_L);
  //unsigned id = 0;
  for(unsigned i=0; i<mv.ellList.size(); i++){
	  mv.ellSelection.push_back(i);
  }
  //cout << "</FirstMove>" << endl;
}

bool Eternity2_LMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_LMove& mv) const
{
  //cout << "<NextMove>" << endl;
  // Insert the code that generate the move that follows mv in the neighborhood, and writes
  // it back in mv. Return false if mv is already the last move.
  unsigned size = mv.ellSelection.size();
  unsigned i = size;
  // Find the pivot, i.e. the element before the longest non-increasing suffix
  unsigned pivot = i--;
  while( i > 0 && pivot >= size){
	  if(mv.ellSelection.at(i) > mv.ellSelection.at(i-1))
		  pivot = i-1;
	  i--;
  }
  
  if(pivot >= size){
    //cout << "</FirstMove>" << endl;
    return false;
  } 
  
  // Find the rightmost element greater than the pivot
  i = size-1;
  unsigned succ = size;
  while(i > pivot && succ >= size){
	  if(mv.ellSelection.at(i) > mv.ellSelection.at(pivot))
		  succ = i;
	  i--;
  }
  
  //if(succ >= size) return false;
  
  // Exchange the pivot with the successor
  unsigned temp = mv.ellSelection.at(succ);
  mv.ellSelection.at(succ) = mv.ellSelection.at(pivot);
  mv.ellSelection.at(pivot) = temp;
  
  // Invert the suffix delimited by the pivot
  for(i = 1; i <= (size-pivot)/2; i++){
	  temp = mv.ellSelection.at(pivot + i);
	  mv.ellSelection.at(pivot+i) = mv.ellSelection.at(size-i);
	  mv.ellSelection.at(size-i) = temp;
  }
  
  //cout << "</FirstMove>" << endl;
  return true;
}


int Eternity2_LMoveDeltaCostComponent::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_LMove& mv) const
{
  cout << "<DeltaCost>" << endl;
  int cost = 0;
  int originalCost = 0;
  int swappedCost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st

  // Same code as MakeMove, but I calculate the deltacost instead of updating the state
  for(unsigned i = 0; i < mv.ellSelection.size(); i++)
  { 
	  // Compute the cost of moving ellList[ellSelection[i]] in place of ellList[i]

    unsigned i1 = mv.ellList.at(i).first.first;
    unsigned j1 = mv.ellList.at(i).first.second;
    unsigned i2 = mv.ellList.at(mv.ellSelection.at(i)).first.first;
    unsigned j2 = mv.ellList.at(mv.ellSelection.at(i)).first.second;

    if (!(i1 < st.getHeight()-1 && i2 < st.getHeight()-1 && j1 < st.getWidth()-1 && j2 < st.getWidth()-1))
    {
      continue;
    }


    // Store the IDOs of the single cells
    IDO eLstIDO[]= { 
        st.getIDOAt(pair<unsigned,unsigned>(i1,j1)), 
        st.getIDOAt(pair<unsigned,unsigned>(i1,j1+1)),
        st.getIDOAt(pair<unsigned,unsigned>(i1+1,j1+1)), 
        st.getIDOAt(pair<unsigned,unsigned>(i1+1,j1)) 
    };
    IDO eSelIDO[]= { 
        st.getIDOAt(pair<unsigned,unsigned>(i2,j2)), 
        st.getIDOAt(pair<unsigned,unsigned>(i2,j2+1)),
        st.getIDOAt(pair<unsigned,unsigned>(i2+1,j2+1)),
        st.getIDOAt(pair<unsigned,unsigned>(i2+1,j2)) 
    };

	  // Store the coordinates of the single cells
	  Coord eLstCoord[] = { 
        pair<unsigned,unsigned>(i1,j1), 
        pair<unsigned,unsigned>(i1,j1+1), 
  		  pair<unsigned,unsigned>(i1+1,j1+1), 
        pair<unsigned,unsigned>(i1+1,j1) 
    };
	  Coord eSelCoord[] = { 
        pair<unsigned,unsigned>(i2,j2), 
        pair<unsigned,unsigned>(i2,j2+1), 
		    pair<unsigned,unsigned>(i2+1,j2+1), 
        pair<unsigned,unsigned>(i2+1,j2) 
    };

	  // Calculate the rotation  needed to fit ellList[i] in place of ellList[ellSelection[i]]
    // Positive rotation = counter-clockwise rotation of the L
	  int rot = mv.ellList.at(i).second - mv.ellList.at(mv.ellSelection.at(i)).second;
	  
    // Original cost
    for(unsigned j = 0; j < 3; j++)
    {
		  originalCost += singleTileCost(eLstIDO[j], eLstCoord[j], st);
	  }

    // Now compute the swapped cost
    
    // First off map the first L into the second 
    // This means that, for each cell of ellList[i], I store the index of the cell of
    // ellList[ellSelection[i]] that is moved in its place.

	  unsigned map[4];
    for (unsigned i = 0; i < 4; ++i)
    {
      int val = st.strangeMod(i-rot,4);
      map[i] = (unsigned int)val;
    }

    // Now compute the cost after the swap is (supposedly) made 
    // This is done by checking for violations
    for(unsigned k = 0; k < 4; k++)
    {
      // Rename the orientation of the moved L for convenience
      // Also indicates where the "hole" in the L is
      unsigned newOrient = mv.ellList.at(i).second;

      if(k != newOrient) // No point in checking for mistmatches in the hole
      {
        // DOWN
        if( ((newOrient==0 || newOrient==1) && (k==2 || k==3)) || (newOrient==2 && (k==1 || k==3)) || (newOrient==3 && (k==0 || k==2)) )
        {
          // South color of the cell that maps into k
          unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(0-rot,4));
          unsigned c2; // North color of the cell below k
          // Check that we don't go over the border
          if(eLstCoord[k].first+1 < st.getHeight())
          {
            c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first+1, eLstCoord[k].second)),2);  
          } else c2 = 0; // Grey
          if(c1!=c2) swappedCost++; // Mismatch
        }

        // LEFT
        if( (newOrient==0 && (k==1 || k==3)) || ((newOrient==1 || newOrient==2) && (k==0 || k==3)) || (newOrient==3 && (k==0 || k==2)) )
        {
          unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(1-rot,4));
          unsigned c2;
          if(eLstCoord[k].second > 0)
          {
            c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first, eLstCoord[k].second-1)),3);  
          } else c2 = 0;
          if(c1!=c2) swappedCost++;
        }

        // UP
        if( (newOrient==0 && (k==1 || k==3)) || (newOrient==1 && (k==0 || k==2)) || ((newOrient==2 || newOrient==3) && (k==0 || k==1)) )
        {
          unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(2-rot,4));
          unsigned c2;
          if(eLstCoord[k].first > 0)
          {
            c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first-1, eLstCoord[k].second)),0);  
          } else c2 = 0;
          if(c1!=c2) swappedCost++;
        }

        // RIGHT
        if( ((newOrient==0 || newOrient==3) && (k==1 || k==2)) || (newOrient==1 && (k==0 || k==2)) || (newOrient==2 && (k==1 || k==3)) )
        {
          unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(3-rot,4));
          unsigned c2;
          if(eLstCoord[k].second+1 < st.getWidth())
          {
            c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first, eLstCoord[k].second+1)),1);  
          } else c2 = 0;
          if(c1!=c2) swappedCost++;
        }
      } // Done computing swappedCost
    }
  }
  
  cost = swappedCost - originalCost;
  cout << "</Deltacost>" << endl;
  return cost;
}

vector<vector<pair<int,Orientation>>> Eternity2_LMoveNeighborhoodExplorer::createGraph(const Eternity2_State& st, Eternity2_LMove& mv) const
{
  unsigned size = mv.ellSelection.size();
  vector<vector<pair<int,Orientation>>> graph = vector<vector<pair<int,Orientation>>>(size);

  // Empty graph if the move is empty
  if(size == 0) return graph;

  for (int i = 0; i < size; ++i) // For each L
  {
    unsigned i1 = mv.ellList.at(i).first.first;
    unsigned j1 = mv.ellList.at(i).first.second;

    if (!(i1 < st.getHeight()-1 && j1 < st.getWidth()-1))
    {
      continue;
    }

    // Store the IDOs of the single cells
    IDO eLstIDO[]= { 
        st.getIDOAt(pair<unsigned,unsigned>(i1,j1)), 
        st.getIDOAt(pair<unsigned,unsigned>(i1,j1+1)),
        st.getIDOAt(pair<unsigned,unsigned>(i1+1,j1+1)), 
        st.getIDOAt(pair<unsigned,unsigned>(i1+1,j1)) 
    };
    // Store the coordinates of the single cells
    Coord eLstCoord[] = { 
        pair<unsigned,unsigned>(i1,j1), 
        pair<unsigned,unsigned>(i1,j1+1), 
        pair<unsigned,unsigned>(i1+1,j1+1), 
        pair<unsigned,unsigned>(i1+1,j1) 
    };
    
    graph[i] = vector<pair<int,Orientation>>(size);
    
    for (int j = 0; j < size; ++j)
    {      
      // Compute the "one-directional" delta cost to all other ells (i.e. don't subtract the original cost)
      if(j==i)
      {
        // Original cost
        int originalCost = 0;
        for(unsigned k = 0; k < 3; j++)
        {
          originalCost += singleTileCost(eLstIDO[k], eLstCoord[k], st);
        }
        graph[i][j] = pair<int,Orientation>(originalCost,mv.ellList[i].second);
      }else
      {
        // Swapped cost
        int swappedCost = 0;
        unsigned i2 = mv.ellList.at(j).first.first;
        unsigned j2 = mv.ellList.at(j).first.second;

        if (!(i2 < st.getHeight()-1 && j2 < st.getWidth()-1))
        {
          continue;
        }

        IDO eSelIDO[]= { 
          st.getIDOAt(pair<unsigned,unsigned>(i2,j2)), 
          st.getIDOAt(pair<unsigned,unsigned>(i2,j2+1)),
          st.getIDOAt(pair<unsigned,unsigned>(i2+1,j2+1)),
          st.getIDOAt(pair<unsigned,unsigned>(i2+1,j2)) 
        };
        Coord eSelCoord[] = { 
          pair<unsigned,unsigned>(i2,j2), 
          pair<unsigned,unsigned>(i2,j2+1), 
          pair<unsigned,unsigned>(i2+1,j2+1), 
          pair<unsigned,unsigned>(i2+1,j2) 
        };
        // Calculate the rotation  needed to fit ellList[i] in place of ellList[ellSelection[i]]
        // Positive rotation = counter-clockwise rotation of the L
        int rot = mv.ellList.at(i).second - mv.ellList.at(j).second;
   
        // First off map the first L into the second 
        // This means that, for each cell of ellList[i], I store the index of the cell of
        // ellList[ellSelection[i]] that is moved in its place.
        unsigned map[4];
        for (unsigned k = 0; k < 4; ++i)
        {
          int val = st.strangeMod(k-rot,4);
          map[k] = (unsigned int)val;
        }
    
        // Now compute the cost after the swap is (supposedly) made 
        // This is done by checking for violations
        for(unsigned k = 0; k < 4; k++)
        {
          // Rename the orientation of the moved L for convenience
          // Also indicates where the "hole" in the L is
          unsigned newOrient = mv.ellList.at(i).second;

          if(k != newOrient) // No point in checking for mistmatches in the hole
          {
            // DOWN
            if( ((newOrient==0 || newOrient==1) && (k==2 || k==3)) || (newOrient==2 && (k==1 || k==3)) || (newOrient==3 && (k==0 || k==2)) )
            {
              // South color of the cell that maps into k
              unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(0-rot,4));
              unsigned c2; // North color of the cell below k
              // Check that we don't go over the border
              if(eLstCoord[k].first+1 < st.getHeight())
              {
                c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first+1, eLstCoord[k].second)),2);  
              } else c2 = 0; // Grey
              if(c1!=c2) swappedCost++; // Mismatch
            }
  
            // LEFT
            if( (newOrient==0 && (k==1 || k==3)) || ((newOrient==1 || newOrient==2) && (k==0 || k==3)) || (newOrient==3 && (k==0 || k==2)) )
            {
              unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(1-rot,4));
              unsigned c2;
              if(eLstCoord[k].second > 0)
              {
                c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first, eLstCoord[k].second-1)),3);  
              } else c2 = 0;
              if(c1!=c2) swappedCost++;
            }
  
            // UP
            if( (newOrient==0 && (k==1 || k==3)) || (newOrient==1 && (k==0 || k==2)) || ((newOrient==2 || newOrient==3) && (k==0 || k==1)) )
            {
              unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(2-rot,4));
              unsigned c2;
              if(eLstCoord[k].first > 0)
              {
                c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first-1, eLstCoord[k].second)),0);  
              } else c2 = 0;
              if(c1!=c2) swappedCost++;
            }
  
            // RIGHT
            if( ((newOrient==0 || newOrient==3) && (k==1 || k==2)) || (newOrient==1 && (k==0 || k==2)) || (newOrient==2 && (k==1 || k==3)) )
            {
              unsigned c1 = st.getColor(eSelIDO[map[k]], st.strangeMod(3-rot,4));
              unsigned c2;
              if(eLstCoord[k].second+1 < st.getWidth())
              {
                c2 = st.getColor(st.getIDOAt(pair<unsigned,unsigned>(eLstCoord[k].first, eLstCoord[k].second+1)),1);  
              } else c2 = 0;
              if(c1!=c2) swappedCost++;
            }
          }
        } // end foreach cell of an L

        graph[i][j] = pair<int,Orientation>(swappedCost,mv.ellList[j].second); 

      }// end swapped cost computation 
    }// end foreach possible swap
  } // end foreach L
  return graph;
}



void Eternity2_LMoveNeighborhoodExplorer::BestMove(const Eternity2_State& st, Eternity2_LMove& mv) const
{
  forceUpdate(st);
  //creating the graph
  vector<vector<pair<int,Orientation>>> graph = createGraph(st, mv);
  //calling the hungarian algorithm
  vector<int> match = hungarianAlgorithm(graph);
  //creating the move
  createMove(mv, match, graph);
}


void Eternity2_LMoveNeighborhoodExplorer::forceUpdate(const Eternity2_State& st) const {
  st.L_counter = 0;
}


void Eternity2_LMoveNeighborhoodExplorer::createMove(Eternity2_LMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>> graph) const {
  for(int i = 0; i < match.size(); ++i){
    mv.ellSelection[i] = match[i];
  }
}







/***************************************************************************
 * Shared general purpose methods
 ***************************************************************************/

/*
* Computes the cost of a single tile, given its orientation and a state.
*/
int deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) {
  unsigned cost = 0;
  unsigned r = crd.first;
  unsigned c = crd.second;
  for(unsigned cp = 0; cp < 4; cp++){
    Color color_crd = st.getColor(ido,cp);
    //Nord border
    if(r == 0 && cp == 2){
      if( color_crd != 0 )
        cost++;
    }
    //Sud border
    if(r == st.getHeight()-1 && cp == 0){
      if( color_crd != 0 )
        cost++;
    }
    //Ovest border
    if(c == 0 && cp == 1){
      if( color_crd != 0 )
        cost++;
    }
    //Est border
    if(c == st.getWidth()-1 && cp == 3){
      if( color_crd != 0 )
        cost++;
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

/*
* Implementation of the "Augmenting Path Algorithm" (see the original specification and our report)
*/

vector<int> hungarianAlgorithm(vector<vector<pair<int,Orientation>>>& m)
{  
  //modifing the rows of the matrix
  int min,i,j;
  for(i=0; i < m.size(); ++i)
  {
      min = 0;

      for(j=1; j < m.size(); ++j)
        min = (m[i][j].first < m[i][min].first) ? j : min;

      min = m[i][min].first;

      for(j=0; j < m.size(); ++j)
        m[i][j].first -= min;
  }

  //modifing the columns of the matrix
  for(j=0; j < m.size(); ++j)
  {
      min = 0;
      
      for(i=0; i < m.size(); ++i)
        min = (m[i][j].first < m[min][j].first) ? i : min;

      min = m[min][j].first;

      for(i=0; i < m.size(); ++i)
        m[i][j].first -= min;
  }

  vector<int> match = vector<int>(m.size(), -1);
  vector<int> inverse_match = vector<int>(m.size(), -1);
  
  findMaxMatch(m, match, inverse_match);

  while( not isPerfectMatching(match) )
  {
      vector<bool> coveredLines = vector<bool>(2*m.size(), 0);

      // Converting maximum matching 'match' into minimum vertex cover
      // using Konig's theorem.
      maxMatch2minCover(m, match, coveredLines);

      //finding the minimum uncovered entry
      int min_entry = std::numeric_limits<int>::max();
      for(int r = 0; r < m.size(); ++r)
        if( not coveredLines[r] )
          for(int c = 0; c < m.size(); ++c)
            if( not coveredLines[c + m.size()] )
              min_entry = std::min(min_entry, m[r][c].first);

      //modifing rows and columns
      for(int r=0; r < m.size(); ++r){
        for(int c=0; c < m.size(); ++c){
          if( not coveredLines[r] )
            m[r][c].first -= min_entry;
          if( coveredLines[c + m.size()] )
            m[r][c].first += min_entry;
        }
      }

      match = vector<int>(m.size(), -1);
      inverse_match = vector<int>(m.size(), -1);

      findMaxMatch(m, match, inverse_match);

  } //END_WHILE

  return match;
}

// Implementation of Konig's theorem.
void maxMatch2minCover(vector<vector<pair<int,Orientation>>>& m, vector<int>& match, vector<bool>& coveredLines)
{
    int i;
    vector<bool> z(2*m.size(),0);
    vector<DFSColor> colors(2*m.size(), WHITE);

    for (i = 0; i < m.size(); ++i)
    {
        if( match[i] == -1 && colors[i] == WHITE)
        {
            AlternatingPaths(m,i,colors,match,z,1);
        }
    }

    for (i = 0; i < m.size(); ++i)
    {
        coveredLines[i] = not z[i];
    }

    for (i = m.size(); i < 2*m.size(); ++i)
    {
        coveredLines[i] = z[i];
    }
}

void AlternatingPaths(vector<vector<pair<int,Orientation>>>& m, int x, vector<DFSColor>& colors, vector<int>& match, vector<bool>& z, bool parity)
{
  if(parity)
  {
      z[x] = 1;
      colors[x] = GREY;
    
      for(int i=0; i < match.size(); ++i)
      {
          if( m[x][i].first == 0  &&  colors[i+match.size()] == WHITE  &&  match[x] != i )
            AlternatingPaths(m, i, colors, match, z, not parity);
      }

      colors[x] = BLACK;

  } else {

      z[x+match.size()] = 1;
      colors[x+match.size()] = GREY;
    
      for(int i=0; i < match.size(); ++i)
      {
          if( m[i][x].first == 0  &&  colors[i] == WHITE  &&  match[i] == x )
            AlternatingPaths(m, i, colors, match, z, not parity);
      }

      colors[x+match.size()] = BLACK;
  }
}

bool isPerfectMatching(vector<int>& match){
  int i;
  for(i = 0; i < match.size() && match[i] != -1; ++i);
  return (i == match.size());
}


void findMaxMatch(vector<vector<pair<int,Orientation>>>& m, vector<int>& match, vector<int>& inverse_match){
  vector<bool> s = vector<bool>(m.size(), 1);
  vector<vector<bool>> a = vector<vector<bool>> (m.size());
  //filling the "a" matrix
  for(int i=0; i < m.size(); ++i){
    a[i] = vector<bool>(m.size());
    for(int j=0; j < m.size(); ++j){
      a[i][j] = (m[i][j].first == 0) ? 1 : 0;
    }
  }
  int free_node;
  while( findFreeNode(match, s, free_node) )
    DFS(s, a, match, inverse_match, free_node);
}



bool findFreeNode(vector<int>& match, vector<bool>& s, int& free_node){
  for(int i=0; i < match.size(); ++i){
    if( match[i] == -1  &&  s[i] ){
      free_node = i;
      return true;
    }
  }
  return false;
}



void DFS(vector<bool>& s, vector<vector<bool>>& a, vector<int>& match, vector<int>& inverse_match, int x)
{
  //initializing the colors
  vector<DFSColor> colors = vector<DFSColor>(2 * match.size(), WHITE);
  //initializing the precedences
  vector<int> pi = vector<int>(2 * match.size(), -1);

  DFS_Visit(x, 1, colors, pi, a, match);

  if( not extractAP(pi, x, match, inverse_match) )
  {
    s[x] = 0;
    for(int i=0; i < match.size(); ++i)
      if( pi[i] != -1 )
        a[i][pi[i]] = 0;
    for(int i = match.size(); i < 2 * match.size(); ++i)
      if( pi[i] != -1 )
        a[pi[i]][i] = 0;
  }
}



void DFS_Visit(int x, bool parity, vector<DFSColor>& colors, vector<int>& pi, vector<vector<bool>>& a, vector<int>& match)
{
  if(parity)
  {
      colors[x] = GREY;
    
      for(int i=0; i < match.size(); ++i)
      {
          if( a[x][i]  &&  colors[i+match.size()] == WHITE  &&  match[x] != i )
          {
              pi[i+match.size()] = x;
              DFS_Visit(i, not parity, colors, pi, a, match);
          }
      }

      colors[x] = BLACK;

  } else {

      colors[x+match.size()] = GREY;
    
      for(int i=0; i < match.size(); ++i)
      {
          if( a[i][x]  &&  colors[i] == WHITE  &&  match[i] == x )
          {
              pi[i] = x+match.size();
              DFS_Visit(i, not parity, colors, pi, a, match);
          }
      }

      colors[x+match.size()] = BLACK;
  }
}



bool extractAP(vector<int>& pi, int x, vector<int>& match, vector<int>& inverse_match)
{
  int i;

  for(i = match.size(); i < 2 * match.size() && ( inverse_match[i-match.size()] != -1 || pi[i] == -1 ); ++i);

  if( i == pi.size() )
  {
      return false;
  
  } else {

    while( i > -1 && pi[i] != -1)
    {
      match[pi[i]] = i-match.size();
      inverse_match[i-match.size()] = pi[i];
      i = pi[pi[i]];
    }
  }
  
  return true;
}

