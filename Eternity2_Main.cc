#include "Eternity2_Helpers.hh"
#include "easylocal/helpers/multimodalneighborhoodexplorer.hh"


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
  Parameter<double> p1("p1", "Ratio of first move", main_parameters);
  Parameter<double> p2("p2", "Ratio of first move", main_parameters);
  Parameter<double> p3("p3", "Ratio of first move", main_parameters);
  p1 = p2 = p3 = 1/3;

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

  //3-modal neighborhood explorers:
    // 1. singleton + odd-chessboard + even-chessboard
    SetUnionNeighborhoodExplorer<Eternity2_Input, Eternity2_State, DefaultCostStructure<int>, decltype(singleton_nhe), decltype(even_chess_nhe), decltype(odd_chess_nhe)> 
      seo_nhe(in, Eternity2_sm, "Singleton+Even+Odd", singleton_nhe, even_chess_nhe, odd_chess_nhe, {p1, p2, p3});
    // 2. singleton + tts + l_move
    SetUnionNeighborhoodExplorer<Eternity2_Input, Eternity2_State, DefaultCostStructure<int>, decltype(singleton_nhe), decltype(tts_nhe), decltype(ell_nhe)> 
      stl_nhe(in, Eternity2_sm, "Singleton+TTS+LMove", singleton_nhe, tts_nhe, ell_nhe, {p1, p2, p3});
      
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
  //SA
  SimulatedAnnealing<Eternity2_Input, Eternity2_State, decltype(seo_nhe)::MoveType> seo_sa(in, Eternity2_sm, seo_nhe, "SEO_SA"); 
  SimulatedAnnealing<Eternity2_Input, Eternity2_State, decltype(stl_nhe)::MoveType> stl_sa(in, Eternity2_sm, stl_nhe, "STL_SA"); 
  //SD
  SteepestDescent<Eternity2_Input, Eternity2_State, Eternity2_SingletonMove> singleton_sd(in, Eternity2_sm, singleton_nhe, "SING_SD");
  SteepestDescent<Eternity2_Input, Eternity2_State, Eternity2_ThreeTileStreakMove> tts_sd(in, Eternity2_sm, tts_nhe, "TTS_SD");
  SteepestDescent<Eternity2_Input, Eternity2_State, Eternity2_LMove> l_sd(in, Eternity2_sm, ell_nhe, "ELL_SD");

  // tester
  Tester<Eternity2_Input, Eternity2_Output, Eternity2_State> tester(in,Eternity2_sm,Eternity2_om);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_SingletonMove> singleton_move_test(in,Eternity2_sm,Eternity2_om,singleton_nhe, "Singleton Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_EvenChessboardMove> even_chessboard_move_test(in,Eternity2_sm,Eternity2_om,even_chess_nhe, "Even Chessboard Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_OddChessboardMove> odd_chessboard_move_test(in,Eternity2_sm,Eternity2_om,odd_chess_nhe, "Odd Chessboard Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_ThreeTileStreakMove> tts_move_test(in,Eternity2_sm,Eternity2_om,tts_nhe, "Three Tiles Streak Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, Eternity2_LMove> l_move_test(in,Eternity2_sm,Eternity2_om,ell_nhe, "L Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, decltype(seo_nhe)::MoveType> seo_move_test(in,Eternity2_sm,Eternity2_om,seo_nhe, "SEO Move", tester);
  MoveTester<Eternity2_Input, Eternity2_Output, Eternity2_State, decltype(stl_nhe)::MoveType> stl_move_test(in,Eternity2_sm,Eternity2_om,stl_nhe, "STL Move", tester);
  

  SimpleLocalSearch<Eternity2_Input, Eternity2_Output, Eternity2_State> simple_solver(in, Eternity2_sm, Eternity2_om, "simple solver");
  //MultiStartSearch<Eternity2_Input, Eternity2_Output, Eternity2_State> multi_start_solver(in, Eternity2_sm, Eternity2_om, "multi start solver");
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

      if (method == string("SEO_SA"))
        {
          simple_solver.SetRunner(seo_sa);
        }
      else if (method == string("STL_SA"))
        {
          simple_solver.SetRunner(stl_sa);
        }
      else if (method == string("SING_SD"))
        {
          simple_solver.SetRunner(singleton_sd);
        }
      else if (method == string("TTS_SD"))
        {
          simple_solver.SetRunner(tts_sd);
        }
      else if (method == string("ELL_SD"))
        {
          simple_solver.SetRunner(l_sd);
        }
      else
        {
          cerr << "Runners parameters not valid" << endl;
        }
      auto result = simple_solver.Solve();
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
