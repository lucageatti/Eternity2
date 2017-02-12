// File Eternity2_Helpers.hh
#ifndef Eternity2_HELPERS_HH
#define Eternity2_HELPERS_HH

#include "Eternity2_Basics.hh"


using namespace EasyLocal::Core;





/***************************************************************************
 * State Manager 
 ***************************************************************************/

class Eternity2_CostComponent : public CostComponent<Eternity2_Input,Eternity2_State> 
{
public:
  Eternity2_CostComponent(const Eternity2_Input & in, int w, bool hard) : CostComponent<Eternity2_Input,Eternity2_State>(in,w,hard,"Eternity2_CostComponent") 
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
 * Generic Move Neighborhood Explorer:
 ***************************************************************************/
class GenericMoveDeltaCostComponent
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_GenericMove>
{
public:
  GenericMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_GenericMove>(in,cc,"GenericMoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_GenericMove& mv) const;
protected:
  int deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) const;
};


class GenericMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_GenericMove> 
{
public:
  GenericMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_GenericMove>(pin, psm, "GenericMoveNeighborhoodExplorer") {} 
  bool FeasibleMove(const Eternity2_State&, const Eternity2_GenericMove&) const;  
  void MakeMove(Eternity2_State&,const Eternity2_GenericMove&) const;
  bool NextMove(const Eternity2_State&,Eternity2_GenericMove&) const;
  void BestMove(const Eternity2_State&,Eternity2_GenericMove&) const;
  void RandomMove(const Eternity2_State&, Eternity2_GenericMove&) const throw(EmptyNeighborhood){}          
  void FirstMove(const Eternity2_State&,Eternity2_GenericMove&) const throw(EmptyNeighborhood){}
protected:
  bool incrementOrientation(Eternity2_GenericMove& mv) const;
  bool incrementPermutation(Eternity2_GenericMove& mv) const;
  vector<vector<pair<int,Orientation>>> createGraph(const Eternity2_State&,Eternity2_GenericMove&) const;
};





/***************************************************************************
 * Singleton-Move Neighborhood Explorer:
 ***************************************************************************/

class SingletonMoveDeltaCostComponent : public GenericMoveDeltaCostComponent
{
public:
  SingletonMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : GenericMoveDeltaCostComponent( in, cc) {}
};


class SingletonMoveNeighborhoodExplorer : public GenericMoveNeighborhoodExplorer
{
public:
  SingletonMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : GenericMoveNeighborhoodExplorer( pin, psm) { } 
  void RandomMove(const Eternity2_State&, Eternity2_GenericMove&) const throw(EmptyNeighborhood);          
  void FirstMove(const Eternity2_State&,Eternity2_GenericMove&) const throw(EmptyNeighborhood);
};





/***************************************************************************
 * Even-chessboard Neighborhood Explorer:
 ***************************************************************************/

class EvenChessboardMoveDeltaCostComponent : public GenericMoveDeltaCostComponent
{
public:
  EvenChessboardMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : GenericMoveDeltaCostComponent( in, cc) {}
};


class EvenChessboardMoveNeighborhoodExplorer : public GenericMoveNeighborhoodExplorer
{
public:
  EvenChessboardMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : GenericMoveNeighborhoodExplorer( pin, psm) {}
  void RandomMove(const Eternity2_State&, Eternity2_GenericMove&) const throw(EmptyNeighborhood);          
  void FirstMove(const Eternity2_State&,Eternity2_GenericMove&) const throw(EmptyNeighborhood);  
};





/***************************************************************************
 * Odd-chessboard Neighborhood Explorer:
 ***************************************************************************/

class OddChessboardMoveDeltaCostComponent : public GenericMoveDeltaCostComponent
{
public:
  OddChessboardMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : GenericMoveDeltaCostComponent( in, cc) {}
};


class OddChessboardMoveNeighborhoodExplorer : public GenericMoveNeighborhoodExplorer
{
public:
  OddChessboardMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : GenericMoveNeighborhoodExplorer(pin,  psm) {}
  void RandomMove(const Eternity2_State&, Eternity2_GenericMove&) const throw(EmptyNeighborhood);          
  void FirstMove(const Eternity2_State&, Eternity2_GenericMove&) const throw(EmptyNeighborhood);  
};








/***************************************************************************
 * ThreeTileStreak Move Neighborhood Explorer:
 ***************************************************************************/

class ThreeTileStreakMoveDeltaCostComponent
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_ThreeTileStreakMove>
{
  public:
    
    ThreeTileStreakMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_ThreeTileStreakMove>(in,cc,"ThreeTileStreakMoveDeltaCostComponent") 
    {}
    
    int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_ThreeTileStreakMove& mv) const;
};


class ThreeTileStreakMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_ThreeTileStreakMove> 
{
  public:
    ThreeTileStreakMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
      : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_ThreeTileStreakMove>(pin, psm, "ThreeTileStreakMoveNeighborhoodExplorer") {} 
    void RandomMove(const Eternity2_State&, Eternity2_ThreeTileStreakMove&) const throw(EmptyNeighborhood);          
    bool FeasibleMove(const Eternity2_State&, const Eternity2_ThreeTileStreakMove&) const;  
    void MakeMove(Eternity2_State&,const Eternity2_ThreeTileStreakMove&) const;             
    void FirstMove(const Eternity2_State&,Eternity2_ThreeTileStreakMove&) const throw(EmptyNeighborhood);  
    bool NextMove(const Eternity2_State&,Eternity2_ThreeTileStreakMove&) const;

  protected:
    bool incrementOrientation(Eternity2_ThreeTileStreakMove& mv) const;
    bool incrementPermutation(Eternity2_ThreeTileStreakMove& mv) const;
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



//Common methods and functions
vector<unsigned> FisherYatesShuffle(unsigned sz);
int singleTileCost(IDO ido, Coord crd, const Eternity2_State& st);

//Hungarian Algorithm
vector<int> hungarianAlgorithm(vector<vector<pair<int,Orientation>>> m);
bool isPerfectMatching(vector<int> match);
void findMaxMatch(vector<vector<pair<int,Orientation>>> m, vector<int>& match);
bool findFreeNode(vector<int> match, vector<bool> s, int& free_node);
void DFS(vector<bool> s, vector<vector<bool>> a, vector<int> match, int x);
void DFS_Visit(int x, bool parity, vector<DFSColor>& colors, vector<int>& pi, vector<vector<bool>> a, vector<int> match);
bool extractAP(vector<int> pi, int x, vector<int> match);



#endif
