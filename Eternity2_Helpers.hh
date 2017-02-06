// File Eternity2_Helpers.hh
#ifndef Eternity2_HELPERS_HH
#define Eternity2_HELPERS_HH

#include "Eternity2_Basics.hh"
#include <easylocal/easylocal.hh>

using namespace EasyLocal::Core;




/***************************************************************************
 * State Manager 
 ***************************************************************************/

class Eternity2_CostComponent1 : public CostComponent<Eternity2_Input,Eternity2_State> 
{
public:
  Eternity2_CostComponent1(const Eternity2_Input & in, int w, bool hard) : CostComponent<Eternity2_Input,Eternity2_State>(in,w,hard,"Eternity2_CostComponent1") 
  {}
  int ComputeCost(const Eternity2_State& st) const;
  void PrintViolations(const Eternity2_State& st, ostream& os = cout) const;
};


class Eternity2_StateManager : public StateManager<Eternity2_Input,Eternity2_State> 
{
public:
  Eternity2_StateManager(const Eternity2_Input &);
  void RandomState(Eternity2_State&);   
  bool CheckConsistency(const Eternity2_State& st) const;
protected:
  const Eternity2_Input& in;
  vector<IDO> FisherYatesShuffleIDO(vector<IDO> hat) const;
}; 






/***************************************************************************
 * Eternity2_SingletonMove Neighborhood Explorer:
 ***************************************************************************/

class Eternity2_SingletonMoveDeltaCostComponent1
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove>
{
public:
  Eternity2_SingletonMoveDeltaCostComponent1(const Eternity2_Input & in, Eternity2_CostComponent1& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove>(in,cc,"Eternity2_SingletonMoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_SingletonMove& mv) const;
};


class Eternity2_SingletonMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove> 
{
public:
  Eternity2_SingletonMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove>(pin, psm, "Eternity2_SingletonMoveNeighborhoodExplorer") {} 
  void RandomMove(const Eternity2_State&, Eternity2_SingletonMove&) const throw(EmptyNeighborhood);          
  bool FeasibleMove(const Eternity2_State&, const Eternity2_SingletonMove&) const;  
  void MakeMove(Eternity2_State&,const Eternity2_SingletonMove&) const;             
  void FirstMove(const Eternity2_State&,Eternity2_SingletonMove&) const throw(EmptyNeighborhood);  
  bool NextMove(const Eternity2_State&,Eternity2_SingletonMove&) const;   
protected:
  bool incrementOrientation(Eternity2_SingletonMove& mv) const;
  bool incrementPermutation(Eternity2_SingletonMove& mv) const;
  vector<unsigned> FisherYatesShuffle(vector<unsigned> hat) const;
};






/***************************************************************************
 * Output Manager:
 ***************************************************************************/
class Eternity2_OutputManager
  : public OutputManager<Eternity2_Input,Eternity2_Output,Eternity2_State> 
{
public:
  Eternity2_OutputManager(const Eternity2_Input & pin)
    : OutputManager<Eternity2_Input,Eternity2_Output,Eternity2_State>(pin,"Eternity2OutputManager") {}
  void InputState(Eternity2_State&, const Eternity2_Output&) const;  
  void OutputState(const Eternity2_State&, Eternity2_Output&) const; 
}; 
#endif



/**************************************************************************************************
***************************************************************************************************
* L Move
*
* @author Elia
**************************************************************************************************
*************************************************************************************************/

class Eternity2_LMoveDeltaCostComponent1
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_LMove>
{
public:
  Eternity2_LMoveDeltaCostComponent1(const Eternity2_Input & in, Eternity2_CostComponent1& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_LMove>(in,cc,"Eternity2_LMoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_LMove& mv) const;
};

class Eternity2_LMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_LMove> 
{
public:
  Eternity2_LMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_LMove>(pin, psm, "Eternity2_LMoveNeighborhoodExplorer") {} 
  void RandomMove(const Eternity2_State&, Eternity2_LMove&) const throw(EmptyNeighborhood);          
  bool FeasibleMove(const Eternity2_State&, const Eternity2_LMove&) const;  
  void MakeMove(Eternity2_State&,const Eternity2_LMove&) const;             
  void FirstMove(const Eternity2_State&,Eternity2_LMove&) const throw(EmptyNeighborhood);  
  bool NextMove(const Eternity2_State&,Eternity2_LMove&) const;   
protected:
  vector<unsigned> EllGeneration(const Eternity2_State&) const;
};
