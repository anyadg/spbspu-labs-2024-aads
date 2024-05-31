#include <iostream>
#include <fstream>
#include <string>
#include "commands.hpp"
#include "BinarySearchTree.hpp"

int main(int argc, char* argv[])
{
  using namespace doroshenko;
  BST< int, std::string > tree;
  if (argc != 3)
  {
    std::cerr << "Wrong input\n";
    return 1;
  }
  std::ifstream input(argv[2]);
  if (!input)
  {
    std::cerr << "Wrong file\n";
    return 1;
  }
  std::string inputLine;
  std::getline(input, inputLine);
  strToTree(inputLine, tree);
  BST< std::string, std::function< void(std::ostream& out, BST< int, std::string >& tree) > > treeOfFuncs;
  treeOfFuncs.insert("ascending", ascending);
  treeOfFuncs.insert("descending", descending);
  treeOfFuncs.insert("breadth", breadth);
  std::string traverse = argv[1];
  if (treeOfFuncs.find(traverse) != treeOfFuncs.end())
  {
    if (tree.getSize() == 0)
    {
      outEmpty(std::cout);
      return 0;
    }
    try
    {
      treeOfFuncs.at(traverse).second(std::cout, tree);
    }
    catch (const std::exception& e)
    {
      std::cerr << e.what() << '\n';
      return 1;
    }
  }
  else
  {
    outInvCom(std::cerr);
  }
  return 0;
}