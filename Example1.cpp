#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>

class Node
{
public:
    std::string header;
    std::string data;
    Node* next;
    Node* prev;
    Node* child;

    Node(std::string h, std::string d) : header(h), data(d), next(NULL), prev(NULL), child(NULL) {}
};

class LinkedList
{
private:
    Node* head;

    void displayHelper(Node* node, int level);
    bool isChild(const std::string& currentHeader, const std::string& parentHeader);
    void clearList(Node* node);  // Helper function to delete nodes
public:
    LinkedList() : head(NULL) {}
    ~LinkedList();  // Destructor

    void parseDocument(const std::vector<std::string>& document);
    void displayList();
    void badFunction();
};

void LinkedList::clearList(Node* node)
{
    while (node != NULL)
    {
        Node* next = node->next;
        if (node->child != NULL)
        {
            clearList(node->child);
        }
        delete node;
        node = next;
    }
}

LinkedList::~LinkedList()
{
    clearList(head);
}

bool LinkedList::isChild(const std::string& currentHeader, const std::string& parentHeader)
{
    return currentHeader.find(parentHeader + ".") == 0;
}

void LinkedList::parseDocument(const std::vector<std::string>& document)
{
    Node* lastNode = NULL;
    std::stack<Node*> nodeStack;

    for(size_t i = 0; i < document.size(); ++i)
    {
        std::string line = document[i];
        std::string header, data;
        size_t pos = line.find(" ");
        if (pos != std::string::npos)
        {
            header = line.substr(0, pos);
            data = line.substr(pos + 1);
        }
        else
        {
            header = line;
            data = "";
        }

        if (!header.empty())
        {
            Node* newNode = new Node(header, data);

            if (nodeStack.empty())
            {
                head = newNode;
            }
            else
            {
                if (isChild(header, nodeStack.top()->header))
                {
                    nodeStack.top()->child = newNode;
                } 
                else
                {
                    while (!nodeStack.empty() && !isChild(header, nodeStack.top()->header))
                    {
                        nodeStack.pop();
                    }

                    if (!nodeStack.empty())
                    {
                        nodeStack.top()->next = newNode;
                        newNode->prev = nodeStack.top();
                    }
                    else if (lastNode != NULL)
                    {
                        lastNode->next = newNode;
                        newNode->prev = lastNode;
                    }
                }
            }
            lastNode = newNode;
            nodeStack.push(newNode);
        }
    }
}

void LinkedList::displayList()
{
    Node* current = head;
    displayHelper(current, 0);
}

void LinkedList::displayHelper(Node* node, int level)
{
    while (node != NULL)
    {
        for (int i = 0; i < level; ++i) 
        {
            std::cout << "  ";  // Indent according to node level
        }
        
        std::cout << node->header;
        if (!node->data.empty())
        {
            std::cout << ". " << node->data;
        }
        std::cout << std::endl;
        
        if (node->child != NULL)
        {
            displayHelper(node->child, level + 1);  // Recursive call for child node
        }
        node = node->next;  // Move to the next node at the same level
    }
}

void LinkedList::badFunction()
{
    // Intentional bug: accessing invalid memory
    Node* badNode = NULL;
    std::cout << badNode->header << std::endl;  // Dereferencing null pointer

    // Intentionally creating memory leak
    // Node* leakedNode = new Node("Leaked Header", "Leaked Data");
}

int main()
{
    std::ifstream file("document.txt");
    std::vector<std::string> document;
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            document.push_back(line);
        }
        file.close();
    }
    else
    {
        std::cerr << "Unable to open file" << std::endl;
        return 1;
    }

    LinkedList* list = new LinkedList();
    
    list->parseDocument(document);
    list->displayList();
    
    // Uncomment this to test the badFunction
    // list->badFunction();

    delete list;

    return 0;
}

