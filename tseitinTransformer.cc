#include <iostream>
#include "tseitinTransformer.h"

TseitinTransformer::TseitinTransformer(TreeNode *root) : formulaRoot{root} {}
// std::vector<std::vector<int>> cnf; initialized in .h

int TseitinTransformer::transSubformula(TreeNode *subRoot)
{
  // This is required to iterate through all the nodes of the sparse tree.
  // Map all the clauses(operation node) and literal(var nodes) to numbers
  // When a clause is found call the respective transformation functions
  // When a literal is identified return the varIdCounter(The mapped number) of
  // that literal
  // After the transformation of the clause happens return the varIdCounter of that
  // clause.
  // std::cout <<"subroot getContent:" <<subRoot->getContent() << std::endl;
  if (subRoot->getContent() == "*")
  { // find ID and put clauses into cnf, all done here
    int leftID = transSubformula(subRoot->getLeftChild());
    int rightID = transSubformula(subRoot->getRightChild());
    ++varIdCounter; // new formula
    // addAndEq(varIdCounter, and_leftchild_ID, and_rightchild_ID);
    // removed the addAndEq, use less code, the function is only used once and it here
    int curID = varIdCounter;
    std::vector<int> clause1;
    std::vector<int> clause2;
    std::vector<int> clause3;
    clause1.push_back(-curID);
    clause1.push_back(leftID);
    clause2.push_back(-curID);
    clause2.push_back(rightID);
    clause3.push_back(curID);
    clause3.push_back(-leftID);
    clause3.push_back(-rightID);
    cnf.push_back(clause1);
    cnf.push_back(clause2);
    cnf.push_back(clause3);

    // transSubformula(subRoot->getLeftChild());
    // transSubformula(subRoot->getRightChild());
    return varIdCounter;
  }
  // if(subRoot->getContent() == "-"){
  if (subRoot->getContent() == "+")
  {
    int leftID = transSubformula(subRoot->getLeftChild());
    int rightID = transSubformula(subRoot->getRightChild());
    ++varIdCounter;
    // addOrEq(varIdCounter, or_leftchild_id, or_rightchild_id);
    // removed the addOrEq actually reduced line of code, since the function
    // only been used once
    int curID = varIdCounter;
    std::vector<int> clause1;
    std::vector<int> clause2;
    std::vector<int> clause3;
    clause1.push_back(-curID);
    clause1.push_back(leftID);
    clause1.push_back(rightID);
    clause2.push_back(-leftID);
    clause2.push_back(curID);
    clause3.push_back(-rightID);
    clause3.push_back(curID);
    cnf.push_back(clause1);
    cnf.push_back(clause2);
    cnf.push_back(clause3);
    return varIdCounter;
  }
  if (subRoot->getContent() == "-")
  {
    // check the child? only when child has value, then we can assign it vector int
    int childID = transSubformula(subRoot->getLeftChild());
    ++varIdCounter;
    // p <-> - left => (-p v -left) ^ (p v left)
    int curID = varIdCounter;
  
    std::vector<int> clause1;
    std::vector<int> clause2;
    clause1.push_back(-curID);
    clause1.push_back(-childID);

    clause2.push_back(curID);
    clause2.push_back(childID);
    // std::cout<<"neg curID "<<curID<<" childID " << childID << std::endl;
    // vec_print(clause1);
    // vec_print(clause2);
    // std::cout<<"end of neg eq"<<std::endl;
    cnf.push_back(clause1);
    cnf.push_back(clause2);
    // transSubformula(subRoot->getLeftChild());
    return varIdCounter;
  }

  if (subRoot->getLeftChild() == nullptr && subRoot->getRightChild() == nullptr)
  {
    // this is the leaf node
    // std::cout<<"both child is nullptr, leaf node"<<std::endl;
    if (varIdTable.count(subRoot->getContent()))
    {
      // std::cout<<"leaf node: already node exist in map, value "<<subRoot->getContent()<<std::endl;
      // std::cout<<varIdTable.at(subRoot->getContent())<<std::endl;
      return varIdTable.at(subRoot->getContent());
    }
    else if (varIdTable.find(subRoot->getContent()) == varIdTable.end())
    {
      // make sure it is not exist
      // std::cout<<"leaf node "<<subRoot->getContent()<<" not exist"<<std::endl;
      // varIdCounter++;//not exists, updates the counter
      // std::cout<<"leaf node not exist varIDcounter is"<<varIdCounter<<std::endl;
      ++varIdCounter;
      varIdTable.insert({subRoot->getContent(), varIdCounter});
      // current counter assigned to it, unless see a new operator
    }
  }
  return varIdCounter;
}
int TseitinTransformer::getID(TreeNode *node)
{ // input node//add static all tsetitin share one getID
  // if VariableNode  check the table
  std::cout << "Error: getID is called" << std::endl;
  if (node->getContent() == "*" || node->getContent() == "+" || node->getContent() == "-")
  {
    // std::cout <<"varIdCounter "<< varIdCounter << std::endl;
    return varIdCounter;
  }
  else if (varIdTable.find(node->getContent()) == varIdTable.end())
  {
    // not found inthe table // assign value
    ++varIdCounter;
    varIdTable[node->getContent()] = varIdCounter;
    // increase the counter
    // varIdCounter++;
    return varIdCounter;
  }
  else if (varIdTable.count(node->getContent())) // key exists in the table
  {
    // varIdCounter++;
    // std::cout<<"found in the table"<<std::endl;
    return varIdTable[node->getContent()];
  }
}
void TseitinTransformer::vec_print(std::vector<int> a)
{
  // std::cout<<"the vector elemetns are: ";
  for (int i = 0; i < a.size(); i++)
  {
    std::cout << a.at(i) << ' ';
  }
}
void TseitinTransformer::cnf_print(std::vector<std::vector<int>>)
{
  for (int i = 0; i < cnf.size(); i++)
  {
    std::cout << "cnf[" << i << "] is";
    vec_print(cnf[i]);
    std::cout << std::endl;
  }
}
std::vector<std::vector<int>> TseitinTransformer::transform()
{
  // kick start function where the tranSubFormula is called on the root node
  // std::cout<<"transform begin"<<std::endl;
  cnf.push_back({transSubformula(formulaRoot)});
  // cnf_print(cnf);

  return cnf;
}

unsigned int TseitinTransformer::getVarNum() const
{
  // the total number of clauses and literal
  // why i need a fucntion to do this? 
  return varIdCounter;
  // this is done
}
