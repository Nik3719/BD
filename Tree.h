#pragma once
#include<string>
#include<iostream>
#include <iomanip>
#include"Tools.h"


using namespace std;

struct TreeNode
{
    string data; 
    TreeNode* left;   
    TreeNode* right; 
    TreeNode* prev;
    TreeNode(const string& value) : data(value), left(nullptr), right(nullptr), prev(nullptr) {};
    ~TreeNode() {}
};

struct Tree
{
    TreeNode* root;

    Tree();
    ~Tree();
    void Allocation(TreeNode* node);
    TreeNode* downTree(TreeNode* node, string value);
    void TINSERT(string elem);
    bool TSEARCH(string elem);
    TreeNode* RightOrLeft(TreeNode* node, string& value);
    int countNodes(TreeNode* node);

    int height(TreeNode* node);
    
    void PRINT(TreeNode* root, int space = 0);

    TreeNode* findNode(TreeNode* node, const string& value);

    DL<DL<string>> getLevels();
    void LoadLevel(TreeNode* node, int level, DL<string>& levelValues);

    void JoinTree(TreeNode* root,TreeNode* NewRoot);

    TreeNode* copyTree(TreeNode* node);
   
    Tree(const Tree& other) : root(copyTree(other.root)) {}
    
    Tree& operator=(const Tree& other)
    {
        if (this != &other) 
        {
            Allocation(root); // Free existing resources
            root = copyTree(other.root); // Copy the new tree
        }
        return *this;
    }


    

};