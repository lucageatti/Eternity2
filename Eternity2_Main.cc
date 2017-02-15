#include "Eternity2_Helpers.hh"

using namespace EasyLocal::Debug;

int main(int argc, const char* argv[])
{
  
  ParameterBox main_parameters("main", "Main Program options");

  // The set of arguments added are the following:
  
  Parameter<string> instance("instance", "Input instance", main_parameters); 
  Parameter<int> seed("seed", "Random seed", main_parameters);
  Parameter<string> method("method", "Solution method (empty for tester)", main_parameters);   
  Parameter<string> init_state("init_state", "Initial state (to be read from file)", main_parameters);
  Parameter<unsigned> observer("observer", "Attach the observers", main_parameters);
  Parameter<string> output_file("output_file", "Write the output to a file (filename required)", main_parameters);
 
  // 3rd parameter: false = do not check unregistered parameters
  // 4th parameter: true = silent
  CommandLineParameters::Parse(argc, argv, false, true);  

  if (!instance.IsSet())
    {
      cout << "Error: --main::instance filename option must always be set" << endl;
      return 1;
    }
  Eternity2_Input in(instance);

  if (seed.IsSet())
    Random::Seed(seed);
  
  // cost components: second parameter is the cost, third is the type (true -> hard, false -> soft)
  Eternity2_CostComponent cc1(in, 1, true);

  SingletonMoveDeltaCostComponent singleton_move(in, cc1);
  EvenChessboardMoveDeltaCostComponent even_chessboard_move(in, cc1);
  OddChessboardMoveDeltaCostComponent odd_chessboard_move(in, cc1);
  ThreeTileStreakMoveDeltaCostComponent tts_move(in, cc1);
  Eternity2_LMoveDeltaCostComponent ell_move(in, cc1);

  // helpers
  Eternity2_StateManager Eternity2_sm(in);
  SingletonMoveNeighborhoodExplorer singleton_nhe(in, Eternity2_sm);
  EvenChessboardMoveNeighborhoodExplorer even_chess_nhe(in, Eternity2_sm);
  OddChessboardMoveNeighborhoodExplorer odd_chess_nhe(in, Eternity2_sm);
  ThreeTileStreakMoveNeighborhoodExplorer tts_nhe(in, Eternity2_sm);
  Eternity2_LMoveNeighborhoodExplorer ell_nhe(in, Eternity2_sm);

  Eternity2_OutputManager Eternity2_om(in);
  
  // All cost components must be added to the state manager
  Eternity2_sm.AddCostComponent(cc1);
  
  // All delta cost components must be added to the neighborhood explorer
  singleton_nhe.AddDeltaCostComponent(singleton_move);
  even_chess_nhe.AddDeltaCostComponent(even_chessboard_move);
  odd_chess_nhe.AddDeltaCostComponent(odd_chessboard_move);
  tts_nhe.AddDeltaCostComponent(tts_move);
  ell_nhe.AddDeltaCostComponent(ell_move);
  
  // runners
  HillClimbing<Eternity2_Input, Eternity2_State, Eternity2_GenericMove> Eternity2_hc(in, Eternity2_sm, singleton_nhe, "Eternity2_SingletonMoveHillClimbing");
  SteepestDescent<Eternity2_Input, Eternity2_State, Eternity2_GenericMove> Eternity2_sd(in, Eternity2_sm, singleton_nhe, "Eternity2_SingletonMoveSteepestDescent");
  SimulatedAnnealing<Eternity2_Input, Eternity2_State, Eternity2_GenericMove> Eternity2_sa(in, Eternity2_sm, singleton_nhe, "Eternity2_SingletonMoveSimulatedAnnealing");

  // tester
  Tester<Eternity2_Input, Eternity2_Output, Eternity2_State> tester(in,Eternity2_sm,Eternity2_om);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_GenericMove> singleton_move_test(in,Eternity2_sm,Eternity2_om,singleton_nhe, "Singleton Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_GenericMove> even_chessboard_move_test(in,Eternity2_sm,Eternity2_om,even_chess_nhe, "Even Chessboard Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_GenericMove> odd_chessboard_move_test(in,Eternity2_sm,Eternity2_om,odd_chess_nhe, "Odd Chessboard Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_ThreeTileStreakMove> tts_move_test(in,Eternity2_sm,Eternity2_om,tts_nhe, "Three Tiles Streak Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_LMove> l_move_test(in,Eternity2_sm,Eternity2_om,ell_nhe, "L Move", tester);
  

  SimpleLocalSearch<Eternity2_Input, Eternity2_Output, Eternity2_State> Eternity2_solver(in, Eternity2_sm, Eternity2_om, "Eternity2 solver");
  if (!CommandLineParameters::Parse(argc, argv, true, false))
    return 1;

  if (!method.IsSet())
    { // If no search method is set -> enter in the tester
      if (init_state.IsSet())
	    tester.RunMainMenu(init_state);
      else
	    tester.RunMainMenu();
    }
  else
    {

      if (method == string("SA"))
        {
          Eternity2_solver.SetRunner(Eternity2_sa);
        }
      else if (method == string("HC"))
        {
          Eternity2_solver.SetRunner(Eternity2_hc);
        }
      else // if (method.GetValue() == string("SD"))
        {
          Eternity2_solver.SetRunner(Eternity2_sd);
        }
      auto result = Eternity2_solver.Solve();
	  // result is a tuple: 0: solutio, 1: number of violations, 2: total cost, 3: computing time
      Eternity2_Output out = result.output;
      if (output_file.IsSet())
        { // write the output on the file passed in the command line
          ofstream os(static_cast<string>(output_file).c_str());
          os << out << endl;
          os << "Cost: " << result.cost.total << endl;
					os << "Time: " << result.running_time << "s " << endl;
          os.close();
        }
      else
        { // write the solution in the standard output
          cout << out << endl;
          cout << "Cost: " << result.cost.total << endl;
					cout << "Time: " << result.running_time << "s " << endl;					
        }
   }
  return 0;
}
