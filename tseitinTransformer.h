#ifndef TSEITIN_H
#define TSEITIN_H

#include <vector>
#include <map>
#include <string>
#include "treeNode.h"

class TseitinTransformer
{
  private:
    TreeNode *formulaRoot;
    std::vector<std::vector<int>> cnf; // represent CNF in vector of vector
    std::map<std::string, int> varIdTable; // stores the ID associated with the variable
    int varIdCounter = 1; // tracking how many ID have been used; -0 = 0; so start id with 1
    int transSubformula(TreeNode *subRoot); // recursive function called on each subformula
    int getID(TreeNode *node);//read only, do not change 

  public:
    TseitinTransformer(TreeNode *root);
    std::vector<std::vector<int>> transform();
    //std::string cnfString() const; // a debug helper function to check whehter the correctness of transformation
    unsigned int getVarNum() const;
    void vec_print(std::vector<int> a );
    void cnf_print(std::vector<std::vector<int>>);
    //void push_clausestocnf();
};

#endif
