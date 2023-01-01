#include <memory>
#include <iostream>
#include <ostream>
//vector is included in .h file
#include "satSolver.h"
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

bool satCallingMiniSat(unsigned int numVar, std::vector<std::vector<int>> &cnf)
{
  std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver()); // from ppt
  std::vector<Minisat::Lit> literals_vec;
  literals_vec.reserve(numVar);//reduce copied
  //std::cout<<literals.size();
  unsigned int indx = 0;
  while(indx<numVar){
    literals_vec.emplace_back(Minisat::mkLit(solver->newVar()));
    ++indx;
  }
  // creating a vector for creating clauses of unknown literal size
  Minisat::vec<Minisat::Lit> clause_vec; // from ppt, someVector
  for (int cla = 0, literal_index; cla < (int)cnf.size(); cla++) //create new variable, time saver when put literal index here. 
  {
    for (literal_index = 0; literal_index < (int)cnf[cla].size(); literal_index++)//reset literal index value, but dont create new variable
    { 
      int cnf_element = cnf[cla][literal_index];//reused multiple times here
      if(cnf_element < 0){//was <=
        Minisat::Lit atom = literals_vec[-cnf_element - 1];
        clause_vec.push(~atom);
      }else if(cnf_element > 0 ){//>0
        Minisat::Lit atom = literals_vec[cnf_element - 1];
        //std::cout << literal << std::endl;
        clause_vec.push(atom);
      }else if(cnf_element == 0){
        throw"Error: cnf element value == 0";// 0 vs -0 is the same, but creats trouble
      }
    }
    solver->addClause(clause_vec);
    clause_vec.clear();// clean previous clause content
  }
  // std::cout<<"satSolver.cc "<<solver->solve()<<std::endl;
  return solver->solve(); // from ppt
}
