#include"Tree.h"


Tree::Tree() : root(nullptr)
{
}
Tree::~Tree()
{
    Allocation(root);
}


void Tree::Allocation(TreeNode*& node)
{
    if (node != nullptr) 
    {
        Allocation(node->left);
        Allocation(node->right);
        delete node;
        node = nullptr;
    }
}

TreeNode* Tree::downTree(TreeNode* node, string value)
{
    if (node == nullptr) 
    {
        return new TreeNode(value);
    }
    if (nullptr == node->left)
     {
        node->left = downTree(node->left, value);
    }
    else if (nullptr == node->right) 
    {
        node->right = downTree(node->right, value);
    }
    return node;
}


void Tree::TINSERT(string elem)
{
    root = downTree(root, elem);
}

void Tree::JoinTree(TreeNode* Myroot,TreeNode* newRoot) {
    
    if (Myroot->data == "res" && Myroot->left != nullptr)
    {
        JoinTree(Myroot->left, newRoot);
        return;
    }
    if (newRoot == nullptr) 
    {
        return; // Если дерево, которое нужно добавить, пустое, ничего не делаем
    }

    // Копируем текущий узел
    string val = newRoot->data;
    TreeNode* newNode = new TreeNode(val);

    // Если текущее дерево пустое, просто присваиваем корень
    if (Myroot == nullptr)
    {
        Myroot = newNode;
    } 
    else 
    {
        // Ищем место для добавления нового узла
        if (Myroot->left == nullptr) 
        {
            Myroot->left = newNode;
        } 
        else if (Myroot->right == nullptr) 
        {
            Myroot->right = newNode;
        } 
        else 
        {
            // Если оба поддерева заняты, добавляем в левое поддерево
            if (Myroot->left != nullptr) 
            {
                JoinTree(newNode,newRoot->left); // Рекурсивно добавляем в левое поддерево
            }
            if (Myroot->right != nullptr) 
            {
                JoinTree(newNode,newRoot->right); // Рекурсивно добавляем в правое поддерево
            }
        }
    }

    // Рекурсивно добавляем поддеревья
    JoinTree(newNode,newRoot->left);
    JoinTree(newNode,newRoot->right);
}



bool Tree::TSEARCH(string elem)
{
    return RightOrLeft(root, elem) != nullptr;
}

TreeNode* Tree:: RightOrLeft(TreeNode* node,string& value)
{
    if (node == nullptr || node->data == value) 
    {
        return node;
    }
    if (value < node->data) 
    {
        return RightOrLeft(node->left, value);
    }
    else 
    {
        return RightOrLeft(node->right, value);
    }
}







int Tree::countNodes(TreeNode* node) 
{
    if (node == nullptr)
    {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}



int Tree::height(TreeNode* node) 
{
    if (node == nullptr) 
    {
        return 0;
    }
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);
    if (leftHeight > rightHeight)
    {
        return leftHeight + 1;
    }
    return  rightHeight + 1;
}



void Tree::PRINT(TreeNode* root, int space)
{
    if (!root)
    {
        return;
    }  
    space += 2;
    PRINT(root->right, space);
    for (int i = 2; i < space; ++i)
        cout << "  ";
    cout << root->data << endl;
    PRINT(root->left, space);
}

// ��������������� ����� ��� ������ ���� �� ��������
TreeNode* Tree::findNode(TreeNode* node, const string& value)
{
    if (node == nullptr) {
        return nullptr;
    }
    if (node->data == value) {
        return node;
    }
    TreeNode* leftResult = findNode(node->left, value);
    if (leftResult != nullptr) {
        return leftResult;
    }
    return findNode(node->right, value);
}

DL<DL<string>> Tree::getLevels() 
{
    DL<DL<string>> levels;
    int h = height(root);
    for (int i = 1; i <= h; i++) 
    {
        DL<string> levelValues;
        LoadLevel(root, i, levelValues);
        levels.LDPUSHT(levelValues);
    }
    return levels;
}

void Tree::LoadLevel(TreeNode* node, int level, DL<string>& levelValues)
{
    if (node == nullptr) 
    {
        return;
    }
    if (level == 1) 
    {
        levelValues.LDPUSHT(node->data);
    }
    else if (level > 1)
    {
        LoadLevel(node->left, level - 1, levelValues);
        LoadLevel(node->right, level - 1, levelValues);
    }
}

 TreeNode* Tree::copyTree(TreeNode* node) 
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        TreeNode* newNode = new TreeNode(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);
        newNode->prev = copyTree(node->prev); 

        return newNode;
    }