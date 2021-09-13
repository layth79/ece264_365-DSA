/*
ECE-264 Project 1
Layth Yassin
Professor Sable

This program takes a series of commands from a text file as input, in which the commands make the program create stacks and queues, 
push a value onto stacks and queues, and pop a value from stacks and queues. These stacks and queues will be stored in 
lists based on the data tyes stored in them (int, double, string). The results of these commands will be output to a text file.
*/

#include <iostream>
#include <fstream>
#include <list>
#include <string>

using namespace std;

//initialization of input/output file streams
ifstream inFile; 
ofstream outFile;

//creation of base class SimpleList
template<typename T>
class SimpleList {
private:
	//creation of nested class Node
	class Node 
	{
	public:
		T data;
		Node *next;
		Node(T item, Node *n = nullptr)
		{
			data = item;
			next = n;
		}
	};
	string name;
	Node *head;
	Node *tail;
	int size;	
protected:
	//functions to be used in implementation of push/pop
	void insertStart(T item);
	void insertEnd(T item);
	T removeStart();
public:
	//creation of pure virtual functions to force use in subclasses
	virtual void push(T a) = 0;
	virtual T pop() = 0;

	//function to return the name of a list
	string getName()
	{
		return name;
	}

	//function to determine if a list is empty
	bool empty()
	{
		if(size == 0)
			return true;
		else
			return false;
	}

	SimpleList(string n)
	{
		name = n;
		head = nullptr;
		tail = nullptr;
		size = 0;
	}
};

//implementation of insertStart to insert a new node at start of list
template<typename T>
void SimpleList<T>::insertStart(T data)
{
	Node *nd = new Node{data, head};
	head = nd;
	size++;
}

//implementation of insertEnd to insert a new node at end of list
template<typename T>
void SimpleList<T>::insertEnd(T data)
{
	Node *nd = new Node{data, tail};

	if (size > 0)
		tail->next = nd;

	tail = nd;
	++size;

	if (size == 1)
		head = tail;
}

//implementation of removeStart to remove a node from start of list
template<typename T>
T SimpleList<T>::removeStart()
{
	Node *temp;
	temp = head;
	T item = temp->data;
	head = head->next;
	
	if(size == 1)
		tail = nullptr;

	delete temp;
	
	size--;

	return item;
}

//creation of Stack subclass, implementation of push/pop functions 
template<typename T>
class Stack:public SimpleList<T> {
public:
	Stack(string &name) : SimpleList<T>{name}{};

	void push(T item)
	{
		SimpleList<T>::insertStart(item);
	}

	T pop()
	{
		return SimpleList<T>::removeStart();
	}
};

//creation of Queue subclass, implementation of push/pop functions
template<typename T>
class Queue:public SimpleList<T> {
public:
	Queue(string &name) : SimpleList<T>{name}{};

	void push(T item)
	{
		SimpleList<T>::insertEnd(item);
	}

	T pop()
	{
		return SimpleList<T>::removeStart();
	}
};

//function that determines if the name of a list already exists
template <typename T>
bool search(string &name, list<SimpleList<T> *> &lst)
{
	for (auto container : lst)
		if (container->getName() == name)
			return true; 

	return false; 
}

//function that determines whether list is a stack or queue; if true, stack; if false, queue
bool containerType(string &input)
{
	int len = input.length();

	if (input[len - 1] == 'k')
		return true;
	else
		return false;
}

//function that creates a stack/queue in the int list
void createListInt(string &type, string &name, list<SimpleList<int> *> &lst)
{
	if (!search(name, lst))
	{
		if (type == "stack")
			lst.push_front(new Stack<int>{name});
		else 
			lst.push_front(new Queue<int>{name});
	}
	else 
		outFile << "ERROR: This name already exists!" << endl;
}

//function that creates a stack/queue in the double list
void createListDouble(string &type, string &name, list<SimpleList<double> *> &lst)
{
	if (!search(name, lst))
	{
		if (type == "stack")
			lst.push_front(new Stack<double>{name});
		else 
			lst.push_front(new Queue<double>{name});
	}
	else 
		outFile << "ERROR: This name already exists!" << endl;
}

//function that creates a stack/queue in the string list
void createListString(string &type, string &name, list<SimpleList<string> *> &lst)
{
	if (!search(name, lst))
	{
		if (type == "stack")
			lst.push_front(new Stack<string>{name});
		else 
			lst.push_front(new Queue<string>{name});
	}
	else 
		outFile << "ERROR: This name already exists!" << endl;
}

//function to push an int value onto a stack/queue
void intPush(string &item, string &name, list<SimpleList<int> *> &lst)
{
	if(search(name, lst))
	{
		for (auto container : lst)
		{
			if (container->getName() == name)
			{
					container->push(stoi(item)); 
					break;
			}
		}

	}
	else 
		outFile << "ERROR: This name does not exist!" << endl;
}

//function to push a double onto a stack/queue
void doublePush(string &item, string &name, list<SimpleList<double> *> &lst)
{
	if(search(name, lst))
	{
		for (auto container : lst)
		{
			if (container->getName() == name)
			{
					container->push(stod(item)); 
					break;
			}
		}

	}
	else 
		outFile << "ERROR: This name does not exist!" << endl;
}

//function to push a string onto a stack/queue
void stringPush(string &item, string &name, list<SimpleList<string> *> &lst)
{
	if(search(name, lst))
	{
		for (auto container : lst)
		{
			if (container->getName() == name)
			{
					container->push(item); 
					break;
			}
		}

	}
	else 
		outFile << "ERROR: This name does not exist!" << endl;
}

//template function to pop a value from a stack/queue in the corresponding list
template<typename T>
void containerPop(string &name, list<SimpleList<T> *> &lst)
{
	if(search(name, lst))
	{
		for (auto container : lst)
		{
			if(container->getName() == name)
			{	
				if (container->empty())
				{
					outFile << "ERROR: This list is empty!" << endl; 
					break;
				} 
				else
				{
					outFile << "Value popped: " << container->pop() << endl;
					break; 
				}
			}
		}
	}
	else 
		outFile << "ERROR: This name does not exist!" << endl;
}

//processes commands from input file and performs proper action
void process(string &input, list<SimpleList<int> *> &lstI, list<SimpleList<double> *> &lstD, list<SimpleList<string> *> &lstS)
{
	string word1 = "", word2 = "", word3 = ""; //3 strings initialized to store maximum of 3 words per line
	int i, counter = 0; //counter intialized to keep track of the number of spaces in each line
	int a [2];
	int len = input.length();

	for(i = 0; i < len; i++) //loops through line of input to determine number of spaces, and the index of those spaces
	{
		if(input[i] == ' ')
		{
			a[counter] = i;
			counter++;
		}
	}

    string command;
	string containerName;
	string containerType;
	
	if(counter == 2) //tests if there are 2 spaces in the line of input
	{
		//using the locations of the spacesthat are stored in the array, words 1, 2, and 3 are determined and assigned properly
		word1 = input.substr(0, a[0]);
		word2 = input.substr(a[0] + 1, a[1] - a[0] - 1);
		word3 = input.substr(a[1] + 1, len - a[1] - 1);
		command = word1;
		containerName = word2;
		containerType = word3; 	//name of variable misleading in some cases; 
								//could represent whether it's a stack or queue, or represent the value being pushed

		if(command == "create")
		{
			switch(containerName[0]) //checks first letter of the name of the object to determine type
			{
				case 'i': createListInt(containerType, containerName, lstI);
				break;
				case 'd': createListDouble(containerType, containerName, lstD);
				break;
				case 's': createListString(containerType, containerName, lstS);
				break; 
			}
		}
		if(command == "push")
		{
			switch(containerName[0])
			{
				case 'i': intPush(containerType, containerName, lstI);
				break;
				case 'd': doublePush(containerType, containerName, lstD);
				break;
				case 's': stringPush(containerType, containerName, lstS);
				break;
			}
		}
	}

	else //this means there is one space, implying pop command
	{
		word1 = input.substr(0, a[0]);
		word2 = input.substr(a[0] + 1, len - a[0] - 1);
		command = word1;
		containerName = word2;
		switch(containerName[0])
		{
			case 'i': containerPop(containerName, lstI);
			break;
			case 'd': containerPop(containerName, lstD);
			break;
			case 's': containerPop(containerName, lstS);
			break;
		}
	}
}

int main()
{
	//creation of lists of types int, double, and string (will contain corresponding stacks/queues)
	list<SimpleList<int> *> listSLi; 
	list<SimpleList<double> *> listSLd;
	list<SimpleList<string> *> listSLs;

	string in, out, input;
	cout << "Enter name of input file: ";
	cin >> in;
	cout << "Enter name of output file: ";
	cin >> out;

	inFile.open(in);
	outFile.open(out);

	while(getline(inFile, input)) //reads input file line by line
	{
		outFile << "PROCESSING COMMAND: " << input << endl; //outputs proper action
		process(input, listSLi, listSLd, listSLs); //us of process function to process input file
	}

	return 0;
}
