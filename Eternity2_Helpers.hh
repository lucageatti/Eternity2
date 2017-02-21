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
 * Singleton Move Neighborhood Explorer:
 ***************************************************************************/
class SingletonMoveDeltaCostComponent
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove>
{
public:
  SingletonMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove>(in,cc,"SingletonMoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_SingletonMove& mv) const;
protected:
  int deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) const;
};


class SingletonMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove> 
{
public:
  SingletonMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_SingletonMove>(pin, psm, "SingletonMoveNeighborhoodExplorer") {} 
  bool FeasibleMove(const Eternity2_State&, const Eternity2_SingletonMove&) const;  
  void MakeMove(Eternity2_State&,const Eternity2_SingletonMove&) const;
  bool NextMove(const Eternity2_State&,Eternity2_SingletonMove&) const;
  EvaluatedMove<Eternity2_SingletonMove, DefaultCostStructure<int>> SelectBest(const Eternity2_State& st, size_t& explored, const MoveAcceptor& AcceptMove, const std::vector<double>& weights) const throw (EmptyNeighborhood);
  void RandomMove(const Eternity2_State&, Eternity2_SingletonMove&) const throw(EmptyNeighborhood);        
  void FirstMove(const Eternity2_State&,Eternity2_SingletonMove&) const throw(EmptyNeighborhood);
protected:
  bool incrementOrientation(Eternity2_SingletonMove& mv) const;
  bool incrementPermutation(Eternity2_SingletonMove& mv) const;
  vector<vector<pair<int,Orientation>>> createGraph(const Eternity2_State&,Eternity2_SingletonMove&) const;
  void updateCoords(Eternity2_State& st) const;
  void forceUpdate(const Eternity2_State& st) const;
  void createMove(Eternity2_SingletonMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>>& graph) const;
};




/***************************************************************************
 * Even-chessboard Neighborhood Explorer:
 ***************************************************************************/

class EvenChessboardMoveDeltaCostComponent
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_EvenChessboardMove>
{
public:
  EvenChessboardMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_EvenChessboardMove>(in,cc,"SingletonMoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_EvenChessboardMove& mv) const;
protected:
  int deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) const;
};


class EvenChessboardMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_EvenChessboardMove> 
{
public:
  EvenChessboardMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_EvenChessboardMove>(pin, psm, "EvenChessboardMoveNeighborhoodExplorer") {} 
  bool FeasibleMove(const Eternity2_State&, const Eternity2_EvenChessboardMove&) const;  
  void MakeMove(Eternity2_State&,const Eternity2_EvenChessboardMove&) const;
  bool NextMove(const Eternity2_State&,Eternity2_EvenChessboardMove&) const;
  void BestMove(const Eternity2_State&,Eternity2_EvenChessboardMove&) const;
  void RandomMove(const Eternity2_State&, Eternity2_EvenChessboardMove&) const throw(EmptyNeighborhood);        
  void FirstMove(const Eternity2_State&,Eternity2_EvenChessboardMove&) const throw(EmptyNeighborhood);
protected:
  bool incrementOrientation(Eternity2_EvenChessboardMove& mv) const;
  bool incrementPermutation(Eternity2_EvenChessboardMove& mv) const;
  vector<vector<pair<int,Orientation>>> createGraph(const Eternity2_State&,Eternity2_EvenChessboardMove&) const;
  void forceUpdate(const Eternity2_State& st) const;
  void createMove(Eternity2_EvenChessboardMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>> graph) const;
};




/***************************************************************************
 * Odd-chessboard Neighborhood Explorer:
 ***************************************************************************/

class OddChessboardMoveDeltaCostComponent
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_OddChessboardMove>
{
public:
  OddChessboardMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_OddChessboardMove>(in,cc,"SingletonMoveDeltaCostComponent1") 
  {}
  int ComputeDeltaCost(const Eternity2_State& st, const Eternity2_OddChessboardMove& mv) const;
protected:
  int deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st) const;
};


class OddChessboardMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_OddChessboardMove> 
{
public:
  OddChessboardMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
    : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_OddChessboardMove>(pin, psm, "OddChessboardMoveNeighborhoodExplorer") {} 
  bool FeasibleMove(const Eternity2_State&, const Eternity2_OddChessboardMove&) const;  
  void MakeMove(Eternity2_State&,const Eternity2_OddChessboardMove&) const;
  bool NextMove(const Eternity2_State&,Eternity2_OddChessboardMove&) const;
  void BestMove(const Eternity2_State&,Eternity2_OddChessboardMove&) const;
  void RandomMove(const Eternity2_State&, Eternity2_OddChessboardMove&) const throw(EmptyNeighborhood);        
  void FirstMove(const Eternity2_State&,Eternity2_OddChessboardMove&) const throw(EmptyNeighborhood);
protected:
  bool incrementOrientation(Eternity2_OddChessboardMove& mv) const;
  bool incrementPermutation(Eternity2_OddChessboardMove& mv) const;
  vector<vector<pair<int,Orientation>>> createGraph(const Eternity2_State&,Eternity2_OddChessboardMove&) const;
  void forceUpdate(const Eternity2_State& st) const;
  void createMove(Eternity2_OddChessboardMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>> graph) const;
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

int computeTTSDeltaCost(const Eternity2_State& st, const tuple<tileMove,tileMove,tileMove,int>& single_move, bool delta = true);

class ThreeTileStreakMoveNeighborhoodExplorer
  : public NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_ThreeTileStreakMove> 
{
  public:
    ThreeTileStreakMoveNeighborhoodExplorer(const Eternity2_Input & pin, StateManager<Eternity2_Input,Eternity2_State>& psm)  
      : NeighborhoodExplorer<Eternity2_Input,Eternity2_State,Eternity2_ThreeTileStreakMove>(pin, psm, "ThreeTileStreakMoveNeighborhoodExplorer") {} 
    bool FeasibleMove(const Eternity2_State&, const Eternity2_ThreeTileStreakMove&) const;  
    void MakeMove(Eternity2_State&,const Eternity2_ThreeTileStreakMove&) const;             
    bool NextMove(const Eternity2_State&,Eternity2_ThreeTileStreakMove&) const;
    EvaluatedMove<Eternity2_ThreeTileStreakMove, DefaultCostStructure<int>> SelectBest(const Eternity2_State& st, size_t& explored, const MoveAcceptor& AcceptMove, const std::vector<double>& weights) const throw (EmptyNeighborhood);
    void RandomMove(const Eternity2_State&, Eternity2_ThreeTileStreakMove&) const throw(EmptyNeighborhood);
    void FirstMove(const Eternity2_State&,Eternity2_ThreeTileStreakMove&) const throw(EmptyNeighborhood);  
    
  protected:
    bool incrementOrientation(Eternity2_ThreeTileStreakMove& mv) const;
    bool incrementPermutation(Eternity2_ThreeTileStreakMove& mv) const;
    vector<vector<pair<int,Orientation>>> createGraph(const Eternity2_State&,Eternity2_ThreeTileStreakMove&) const;
    void updateCoords(Eternity2_State& st) const;
    void forceUpdate(const Eternity2_State& st) const;
};




/**************************************************************************************************
***************************************************************************************************
* L Move
*
* @author Elia
**************************************************************************************************
*************************************************************************************************/

class Eternity2_LMoveDeltaCostComponent
  : public DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_LMove>
{
public:
  Eternity2_LMoveDeltaCostComponent(const Eternity2_Input & in, Eternity2_CostComponent& cc) 
    : DeltaCostComponent<Eternity2_Input,Eternity2_State,Eternity2_LMove>(in,cc,"Eternity2_LMoveDeltaCostComponent") 
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
  void BestMove(const Eternity2_State& st, Eternity2_LMove& mv) const;
protected:
  vector<vector<pair<int,Orientation>>> createGraph(const Eternity2_State&, Eternity2_LMove&) const;
  void updateCoords(Eternity2_State& st) const;
  void forceUpdate(const Eternity2_State& st) const;
  void createMove(Eternity2_LMove& mv, vector<int>& match, vector<vector<pair<int,Orientation>>> graph) const;
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
int deltaSingleTileCost(IDO ido, Coord crd, const Eternity2_State& st);

//Hungarian Algorithm
vector<int> hungarianAlgorithm(vector<vector<pair<int,Orientation>>>& m);
void maxMatch2minCover(vector<vector<pair<int,Orientation>>>& m, vector<int>& match, vector<bool>& coveredLines);
void AlternatingPaths(vector<vector<pair<int,Orientation>>>& m, int x, vector<DFSColor>& colors, vector<int>& match, vector<bool>& z, bool parity);
bool isPerfectMatching(vector<int>& match);
void findMaxMatch(vector<vector<pair<int,Orientation>>>& m, vector<int>& match, vector<int>& inverse_match);
bool findFreeNode(vector<int>& match, vector<bool>& s, int& free_node);
void DFS(vector<bool>& s, vector<vector<bool>>& a, vector<int>& match, vector<int>& inverse_match, int x);
void DFS_Visit(int x, bool parity, vector<DFSColor>& colors, vector<int>& pi, vector<vector<bool>>& a, vector<int>& match);
bool extractAP(vector<int>& pi, int x, vector<int>& match, vector<int>& inverse_match);



#endif


