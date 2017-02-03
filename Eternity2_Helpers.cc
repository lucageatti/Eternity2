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
      st.insertTile(rdm_permutation.at(counter),c,d);
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
  IDO aux;
  for(unsigned i = sz-1; i > 0; i--){
    j = Random::Int(0,i);
    aux = hat.at(i);
    aux.second = Random::Int(0,3);
    hat.at(i) = hat.at(j);
    hat.at(j) = aux;
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
      out.insertTile(st.getIDOAt(r,c), r, c);
    }
  }
}







/*****************************************************************************
 * Eternity2_Move Neighborhood Explorer Methods
 *****************************************************************************/

// initial move builder
void Eternity2_MoveNeighborhoodExplorer::RandomMove(const Eternity2_State& st, Eternity2_Move& mv) const  throw(EmptyNeighborhood)
{
  // insert the code that writes a random move on mv in state st
	throw logic_error("Eternity2_MoveNeighborhoodExplorer::RandomMove not implemented yet");	
} 

// check move feasibility
bool Eternity2_MoveNeighborhoodExplorer::FeasibleMove(const Eternity2_State& st, const Eternity2_Move& mv) const
{
  // Insert the code that check is move mv is legal in state st 
  // (return true if legal, false otherwise)
	throw logic_error("Eternity2_MoveNeighborhoodExplorer::FeasibleMove not implemented yet");	
  return true;
} 

// update the state according to the move 
void Eternity2_MoveNeighborhoodExplorer::MakeMove(Eternity2_State& st, const Eternity2_Move& mv) const
{
  // Insert the code that modify the state st based on the application of move mv
	throw logic_error("Eternity2_MoveNeighborhoodExplorer::MakeMove not implemented yet");	
}  

void Eternity2_MoveNeighborhoodExplorer::FirstMove(const Eternity2_State& st, Eternity2_Move& mv) const  throw(EmptyNeighborhood)
{
  // Insert the code the generate the first move in the neighborhood and store it in mv
	throw logic_error("Eternity2_MoveNeighborhoodExplorer::FirstMove not implemented yet");	
}

bool Eternity2_MoveNeighborhoodExplorer::NextMove(const Eternity2_State& st, Eternity2_Move& mv) const
{
  // Insert the code that generate the move that follows mv in the neighborhood, and writes
  // it back in mv. Return false if mv is already the last move. 
	throw logic_error("Eternity2_MoveNeighborhoodExplorer::NextMove not implemented yet");	
  return true;
}

int Eternity2_MoveDeltaCostComponent1::ComputeDeltaCost(const Eternity2_State& st, const Eternity2_Move& mv) const
{
  int cost = 0;
  // Insert the code that computes the delta cost of component 1 for move mv in state st
	throw logic_error("Eternity2_MoveDeltaCostComponent1::ComputeDeltaCost not implemented yet");	
  return cost;
}


