#include <string>
#include <iostream>

void printBT(const std::string& prefix, const node<value_type>* nodeV, bool isLeft) const
{
    std::cout << prefix;

    std::cout << (isLeft ? "├──" : "└──" );

    if (nodeV == _nil){
        std::cout <<"\033[0;36m"<< "nil" << "\033[0m"<<std::endl;
        return ;
    }
    // print the value of the node
    if (nodeV->color == 0)
        std::cout <<"\033[0;36m"<< nodeV->key.first<<"\033[0m"<<std::endl;
    else
        std::cout <<"\033[0;31m"<< nodeV->key.first << "\033[0m"<<std::endl;
    printBT( prefix + (isLeft ? "│   " : "    "), nodeV->left, true);
    printBT( prefix + (isLeft ? "│   " : "    "), nodeV->right, false);
}

void printTree() {
    printBT("", _root, false);
}