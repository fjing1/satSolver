#include <iostream>
#include <string>
#include <map>
#include "parser.h"
#include "treeNode.h"
#include "tseitinTransformer.h"
#include "satSolver.h"

void checkInput(const std::string &line)
{
  if (line.length() == 0)
  {
    throw "Error: invalid input, 0 length str";
  }
}
// The program shall continuously ask for new inputs from standard input and output to the standard output
// The program should terminate gracefully (and quietly) once it sees EOF
int main()
{
  std::string line;
  while (true) // continuously asking for new inputs from standard input
  {
    std::string line; // store each input line
    std::string formulaStr;
    std::getline(std::cin, line);
    if (std::cin.eof())
      break; // terminates mac ctrl + d
    try
    {
      checkInput(line);
    }
    catch (const char *error)
    {
      std::cout << error << std::endl;
      continue;
    }
    FormulaParser fparser1{line};
    TreeNode *treeRoot = nullptr;
    try
    {
      treeRoot = fparser1.getTreeRoot();
      //std::cout << "treeRoot printStr is " << treeRoot->printStr() << std::endl;
      TseitinTransformer *TTx1 = new TseitinTransformer(treeRoot);
      // std::cout<<"after new"<<std::endl;
      std::vector<std::vector<int>> cnf = TTx1->transform();
      bool sat_unsat_result = satCallingMiniSat(TTx1->getVarNum(), cnf);
      if (sat_unsat_result == true){
        std::cout<<"sat"<<std::endl;
      }else if(sat_unsat_result == false){
        std::cout<<"unsat"<<std::endl;
      }else{
        std::cout<<"Error: return from minisat is neither true nor false"<<std::endl;
      }
    }
    catch (const char *e)
    {
      // delete
      std::cout << e << std::endl;
      continue;
    }
  }
}
