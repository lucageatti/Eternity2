# Eternity2
A Local Search-based approach to the Eternity 2 puzzle. It makes use of the framework easylocal++ ([https://bitbucket.org/satt/easylocal-3]). Five types of moves are available:
* Singleton Move: random positions of the board are selected
* Even Chessboard Move: supposing to have a chessboard under the board, we take the positions corresponding to the white cells
* Odd Chessboard Move: like above, but taking the black cells
* L Move: we take as many L-blocks as posible from the board, where an L-block is something like this
	
| x | x | _ |
|---|---|---|
| x | _ | _ |
| _ | _ | _ |

EasyLocal++ provides several meta-euristics for running the local search, like Simulated Annealing and Tabu Search.

## Example
In the following example we use Simulated Annealing with the move "SEO", which is a 3-modal move, consisting in the combination of the Singleton, Even Chessboard and Odd Chessboard move.

	./Eternity2 --main::instance "path_to_the_instance" --main::seed 0 --main::method SA --SEO_SA::start_temperature 5000 --SEO_SA::min_temperature 1 --SEO_SA::cooling_rate 0.99 --SEO_SA::neighbors_sampled 1000 --SEO_SA::neighbors_accepted 1000 --main::insert_ratio 0.25
