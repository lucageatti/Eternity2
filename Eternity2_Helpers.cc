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
int Eternity2_CostComponent1::ComputeCost(const Eternity2_State& st) const
{
  unsigned cost = 0;
  for(unsigned r = 0; r < st.getHeight(); r++){
    for(unsigned c = 0; c < st.getWidth(); c++){
      IDO ido_rc = st.getIDOAt(r,c);
      for(unsigned cp = 0; cp < 4; cp++){
        Color color_rc = st.getColor(ido_rc,cp);
        //Nord border
        if(r == 0 && cp == 2){
          if( color_rc != 0 )
            cost += 2;
        }
        //Sud border
        if(r == st.getHeight()-1 && cp == 0){
          if( color_rc != 0 )
            cost += 2;
        }
        //Ovest border
        if(c == 0 && cp == 1){
          if( color_rc != 0 )
            cost += 2;
        }
        //Est border
        if(c == st.getWidth()-1 && cp == 3){
          if( color_rc != 0 )
            cost += 2;
        }
        
        //Inner borders
        if(r != st.getHeight()-1 && cp == 0){
          IDO ido_sud = st.getIDOAt(r+1,c);
          Color color_sud = st.getColor(ido_sud,2);
          if( color_rc != color_sud )
            cost++;
        }
        else if(c != 0 && cp == 1){
          IDO ido_ovest = st.getIDOAt(r,c-1);
          Color color_ovest = st.getColor(ido_ovest,3);
          if( color_rc != color_ovest )
            cost++;
        }
        else if(r != 0 && cp == 2){
          IDO ido_nord = st.getIDOAt(r-1,c);
          Color color_nord = st.getColor(ido_nord,0);
          if( color_rc != color_nord )
            cost++;
        }
        else if(c != st.getWidth()-1 && cp == 3){
          IDO ido_est = st.getIDOAt(r,c+1);
          Color color_est = st.getColor(ido_est,1);
          if( color_rc != color_est )
            cost++;
        }

      }
    }
  }
  return cost/2;
}

/*
* ...
*/
void Eternity2_CostComponent1::PrintViolations(const Eternity2_State& st, ostream& os) const
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
      out.insertTile(st.getIDOAt(r,c), pair<unsigned,unsigned>(r,c));
    }
  }
}







/*****************************************************************************
 * Eternity2_SingletonMove Neighborhood Explorer Methods
 *****************************************************************************/

/*
* Creates a random move: this is done exploiting the "Fisher-Yates Algorithm" for compute a random permutation.
* It modifies the vectors "permutation" and "aux_perm" of the object "mv".
*/
void Eternity2_SingletonMoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_SingletonMove& mv) const  throw(EmptyNeighborhood)
{
  	vector<unsigned> aux_perm = FisherYatesShuffle(vector<unsigned>(0,mv.permutation.size()));
    mv.setAuxPerm(aux_perm);
    vector<IDO> first_permutation = mv.getFirstPermutation();
    vector<IDO> permutation = vector<IDO>(aux_perm.size());
    for(unsigned c = 0; c < aux_perm.size(); c++){
      permutation.at(c) = first_permutation.at(c);
      permutation.at(c).second = Random::Int(0,3);
    }
    mv.permutation = permutation;
} 


/*
* Implementation of the "Fisher-Yates Algorithm". It generates a unbiased permutation of the vector "hat".
*/
vector<unsigned> Eternity2_SingletonMoveNeighborhoodExplorer::FisherYatesShuffle(vector<unsigned> hat) const {
  unsigned sz = hat.size();
  unsigned j;
  for(unsigned i = sz-1; i > 0; i--){
    j = Random::Int(0,i);
    swap(hat.at(i), hat.at(j));
  }
  return hat;
}


/*
* Checks if ...
*/
bool Eternity2_SingletonMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_SingletonMove& mv) const
{
  // Insert the code that check is move mv is legal in state st 
  // (return true if legal, false otherwise)
	throw logic_error("Eternity2_SingletonMoveNeighborhoodExplorer::FeasibleMove not implemented yet");	
  return true;
} 


/*
* Update the state according to the move.
*/
void Eternity2_SingletonMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_SingletonMove& mv) const
{
  vector<Coord> coords = mv.getCoords();
  for(unsigned c = 0; c < mv.permutation.size(); c++){
    st.insertTile(mv.permutation.at(c), coords.at(c));
  }	
}  


/*
* It creates the first move, choosing from the state "st" the tiles (IDOs) in the coordinates written in the vector "coords".
* This is the original/first permutation, corresponding to 0, 1, ..., N and all the orientations set to 0.
*/
void Eternity2_SingletonMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_SingletonMove& mv) const  throw(EmptyNeighborhood)
{
  vector<Coord> mv_coords = mv.getCoords();
  mv.permutation = vector<IDO>(mv_coords.size());
  vector<unsigned> aux_perm = vector<unsigned>();
  for(unsigned c = 0; c < mv_coords.size(); c++){
    Coord xy = mv_coords.at(c);
    IDO ido_xy = st.getIDOAt(xy.first, xy.second);
    mv.permutation.at(c) = pair<ID,Orientation>(ido_xy.first, 0);
    aux_perm.push_back(c); //initial permutation: 0, 1, ..., N.
  }
  mv.setAuxPerm(aux_perm);
  mv.setFirstPermutation(mv.permutation);
}


/*
* It computes the next move, computing the next permutation of the indexes 0, 1, ..., N.
*/
bool Eternity2_SingletonMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_SingletonMove& mv) const
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
bool Eternity2_SingletonMoveNeighborhoodExplorer::incrementOrientation(Eternity2_SingletonMove& mv) const {
  bool end = false;
  for(unsigned c = 0; c < mv.permutation.size() && !end; c++){
    IDO ido_c = mv.permutation.at(c);
    if(ido_c.second < 3){
      mv.permutation.at(c).second++;
      for(unsigned d = c-1; d > -1; d--){
        mv.permutation.at(d).second = 0;
      }
      end = true;
    }
  }
  return end;
}


/*
* Increments the permutation
*/
bool Eternity2_SingletonMoveNeighborhoodExplorer::incrementPermutation(Eternity2_SingletonMove& mv) const {
  vector<unsigned> aux_perm = mv.getAuxPerm();
  unsigned j = aux_perm.size();
  unsigned i = aux_perm.size() - 1;

  while (i > 0 && aux_perm.at(i-1) >= aux_perm.at(i)) 
     i--; 
  if (i == 0) //All the elements of "aux_perm" are in decreasing order
    return false;

  // (i-1) is the first decreasing elements from the end
  while (aux_perm.at(j-1) <= aux_perm.at(i-1))
    j--;
  swap(aux_perm.at(i-1), aux_perm.at(j-1));
  i++;
  j = aux_perm.size();

  // revert the elements in the tail of the array
  while (i < j) 
    {
      swap(aux_perm.at(i-1), aux_perm.at(j-1));
      i++;
      j--;
    }
  mv.setAuxPerm(aux_perm);

  //Modify the vector "permutation" and set all the orientations to 0.
  vector<IDO> first_permutation = mv.getFirstPermutation();
  for(unsigned c = 0; c < aux_perm.size(); c++){
    mv.permutation.at( c ) = pair<ID,Orientation> (first_permutation.at( aux_perm.at(c) ) , 0);
  }
  return true;
}


int Eternity2_SingletonMoveDeltaCostComponent1::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_SingletonMove& mv) const
{
  int cost = 0;
  	
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
  // insert the code that writes a random move on mv in state st
  // shuffle partition
} 

// check move feasibility
bool Eternity2_LMoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_LMove& mv) const
{
  // Insert the code that check is move mv is legal in state st 
  // (return true if legal, false otherwise)
  
  // Check that elements are not repeated
  vector<IDO> temp = vector<IDO>(mv.EllSelection.size());
  for(unsigned i = 0; i < temp.size(); i++){
	  IDO ido = mv.EllSelection.at(i);
	  if(temp.at(ido.first) != null){
		  return false;
	  }
	  temp.at(ido.first) = ido;
  }
  
  // Maybe we should check that the generated L-partition is good.
  return true;
} 

// update the state according to the move 
void Eternity2_LMoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_LMove& mv) const
{
  // Insert the code that modify the state st based on the application of move mv
  unsigned cols = st.getWidth();
  for(unsigned i = 0; i < mv.EllSelection; i++){
	  // Swap EllList(i) with EllSelection(i) on the board
	  unsigned i1 = mv.EllList.at(i).first().first();
	  unsigned j1 = mv.EllList.at(i).first().second();
	  unsigned i2 = mv.EllList.at(mv.EllSelection.at(i).first()).first().first();
	  unsigned j2 = mv.EllList.at(mv.EllSelection.at(i).first()).first().second();
	  // Backup IDOs
	  IDO[] eLstIDO = { st.getIDOAt(i1,j1), st.getIDOAt(i1,j1+1),
		st.getIDOAt(i1+1,j1+1), st.getIDOAt(i1+1,j1) };
	  IDO[] eSelIDO = { st.getIDOAt(i2,j2), st.getIDOAt(i2,j2+1),
		st.getIDOAt(i2+1,j2+1),st.getIDOAt(i2+1,j2) };
	  // Backup coordinates
	  pair<unsigned,unsigned>[] eLstCoord = 
		{ pair<unsigned,unsigned>(i1,j1), pair<unsigned,unsigned>(i1,j1+1), 
		pair<unsigned,unsigned>(i1+1,j1+1), pair<unsigned,unsigned>(i1+1,j1) };
	  pair<unsigned,unsigned>[] eSelCoord = 
		{ pair<unsigned,unsigned>(i2,j2), pair<unsigned,unsigned>(i2,j2+1), 
		pair<unsigned,unsigned>(i2+1,j2+1), pair<unsigned,unsigned>(i2+1,j2) };
	  // Calculate the rotation needed
	  // OCCHIO AL MODULO NEGATIVO
	  int rot12 = mv.EllList.at(mv.EllSelection.at(i).first()).second() - mv.EllList.at(i).second();
	  //int rot21 = mv.EllList.at(i).second() - mv.EllList.at(mv.EllSelection.at(i).first()).second();
	  
	  // Do the swap
	  /* Swap Example:
	   *
	   *   | |#| = |0|1| --> |#| | = |3|0| 
	   *   |#|#| = |3|2| --> |#|#| = |2|1|
	   *
	   * I do a clockwise rotation of 1 (respectively -1), also rotating each individual cell.
	   */
	  for(unsigned j = 0; j<3; j++){
		  int k1 = (j+rot12)%4;
		  int k2 = (j-rot12)%4;
		  if(k1 < 0) k1*=-1;
		  if(k2 < 0) k2*=-1;
		  st.insertTile( pair<unsigned,unsigned>(eLstIDO[j].first(), eLstIDO[j].second()+rot12),
			pair<unsigned,unsigned>(eSelCoord[].first(k1), eSelCoord[k1].second()) );
		  st.insertTile( pair<unsigned,unsigned>(eSelIDO[j].first(), eSelIDO[j].second()-rot12),
			pair<unsigned,unsigned>(eSelCoord[k2].first(), eSelCoord[k2].second()) );  
	  }
  }
}  

void Eternity2_LMoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_LMove& mv) const  throw(EmptyNeighborhood)
{
  // Insert the code the generate the first move in the neighborhood and store it in mv
  // The list of ells in the partition is produced.
  mv.EllSelection = vector<IDO>(mv.ells);
  unsigned id = 0;
  for(unsigned i=0; i<st.getHeight(); i++){
	  for(unsigned j=0; j<st.getWidth(); j++){
		  unsigned ell = mv.FlatEllMatrix.at(i).at(j);
		  if(ell < mv.NO_ELL)
		  mv.EllSelection.at(id) = pair<unsigned,unsigned>(id,ell);
	  }
  }
}

bool Eternity2_LMoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_LMove& mv) const
{
  // Insert the code that generate the move that follows mv in the neighborhood, and writes
  // it back in mv. Return false if mv is already the last move.
  unsigned size = mv.EllSelection.size();
  unsigned i= size;
  // Find the pivot, i.e. the element before the longest non-increasing suffix
  unsigned pivot=i;
  while( i>0 && pivot >= size){
	  if(mv.EllSelection.at(i) > mv.EllSelection.at(i-1))
		  pivot = i-1;
	  i--;
  }
  
  if(pivot >= size) return false;
  
  // Find the rightmost element greater than the pivot
  i = size-1;
  unsigned succ = size;
  while(i > pivot && succ >= size){
	  if(mv.EllSelection.at(i) > mv.EllSelection.at(pivot))
		  succ = i;
	  i--;
  }
  
  //if(succ >= size) return false;
  
  // Exchange the pivot with the successor
  IDO temp = mv.EllSelection.at(succ);
  mv.EllSelection.at(succ) = mv.EllSelection.at(pivot);
  mv.EllSelection.at(pivot) = temp;
  
  // Invert the suffix delimited by the pivot
  for(i = 1; i <= (size-pivot)/2; i++){
	  temp = mv.EllSelection.at(pivot + i);
	  mv.EllSelection.at(pivot+i) = mv.EllSelection.at(size-i);
	  mv.EllSelection.at(size-i) = temp;
  }
  
  return true;
}

/*
* Partition the board into L-shaped clusters such that no two of them "touch" each
* other horizontally or vertically.
* This should be called only once as the partition is not modified by the move.
*/
vector<vector<unsigned>> EllGeneration(const Eternity2_State& st, const Eternity2_LMove& mv) const
{
	r = st.getHeight(); // Rows
	c = st.getWidth(); // Columns
	mv.ells = 0;
	mv.EllList = vector<pair<pair<unsigned,unsigned>,unsigned>>(mv.ells);
	
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
	vector<vector<unsigned>> partition = vector<<vector>unsigned>(r);
	for(int i = 0; i<r; i++){
		partition.at(i) = vector<unsigned>(c, mv.ANY_ELL);
	}
		
	while(/*!done*/ /*cc < c &&*/ cr < r){
		
		if(cc==0 && cr == 0){
			// Randomized start
			lo = Random::Int(0,3);
			partition.at(0).at(0) = lo;
		}else{ 
		
			// Use the computed constraints
			if(partition.at(cr).at(cc) == mv.ANY_ELL){
				// No constraints, use random
				lo = Random::Int(0,3);
				partition.at(cr).at(cc) = lo;				
			}else if(partition.at(cr).at(cc) < mv.NO_ELL){
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

		mv.ells++;
		mv.EllList.push_back(pair<unsigned,unsigned>,unsigned>(pair<unsigned,unsigned>(cr,cc),lo))
			
		/* Now we want to place constraints on which ells can be placed next
		* based on the ell placed this iteration. */
		// Compute the area to work on
		spaceL = cc;
		spaceR = c-cc-1;
		//spaceU = cr;
		spaceD = r-cr-1;
		unsigned i1 = std::max(2-spaceL, 0);
		unsigned j1 = 2;//2 - std::max(2,spaceU);
		unsigned i2 = std::min(4, 2+spaceR);
		unsigned j2 = std::min(4,2+spaceD);
		
		/* The placement matrix tells us which constraints to put around the 
		* last placed ell in a 5x5 radius.
		* I read the placement matrix from (i1,2) to (i2,j2) so constraints 
		* can be placed on which ells (if any) to place next.
		* Actually I can skip (i1,j1) to (2,2)*/
		for(unsigned i=3; i<=i2; i++){
			unsigned constraint = mv.readPlacementMatrix(2,i,lo);
			partition.at(2).at(i) = constraint;
			// Jump to the first position that allows and ell
			if(nextPosr == 0 && nextPosc == 0 && constraint != mv.NO_ELL){ 
				nextPosr = 2;
				nextPosc = i;
			}
		}
		for(unsigned j=j1; j<=j2; j++){
			for(unsigned i=i1; i<=i2; i++){
				unsigned constraint = mv.readPlacementMatrix(j,i);
				partition.at(j).at(i) = constraint;
				// Jump to the first position that allows and ell
				if(nextPosr == 0 && && nextPosc == 0 && constraint != mv.NO_ELL){ 
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

int Eternity2_LMoveDeltaCostComponent1::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_LMove& mv) const
{
  int cost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st
	throw logic_error("Eternity2_LMoveDeltaCostComponent1::ComputeDeltaCost not implemented yet");	
  return cost;
}
          
int Eternity2_LMoveDeltaCostComponent2::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_LMove& mv) const
{
  int cost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st
	throw logic_error("Eternity2_LMoveDeltaCostComponent2::ComputeDeltaCost not implemented yet");	
  return cost;
}


