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
 * Eternity2_Move Neighborhood Explorer:
 ***************************************************************************/

class Eternity2_MoveDeltaCostComponent1
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_Move>
{
public:
  Eternity2_MoveDeltaCostComponent1(const Eternity2_Input & in, Eternity2_CostComponent1& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_Move>(in,cc,"Eternity2_MoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_Move& mv) const;
};


class Eternity2_MoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_Move> 
{
public:
  Eternity2_MoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_Move>(pin, psm, "Eternity2_MoveNeighborhoodExplorer") {} 
  void RandomMove(const Eternity2_State&, Eternity2_Move&) const throw(EmptyNeighborhood);          
  bool FeasibleMove(const Eternity2_State&, const Eternity2_Move&) const;  
  void MakeMove(Eternity2_State&,const Eternity2_Move&) const;             
  void FirstMove(const Eternity2_State&,Eternity2_Move&) const throw(EmptyNeighborhood);  
  bool NextMove(const Eternity2_State&,Eternity2_Move&) const;   
protected:
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
