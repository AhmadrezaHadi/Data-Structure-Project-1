#include <iostream>
#include <string.h>
#include <time.h>
#include <afxres.h>

#define SIZE 1000

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

using namespace std;

template <class X>
class queue
{
    X *arr;
    int capacity;
    int front;
    int rear;
    int count;

public:
    explicit queue(int size = SIZE);      // constructor
    void dequeue();
    void enqueue(X);
    X peek();
    int size();
    bool isEmpty();
    bool isFull();
};
// Constructor to initialize queue
template <class X>
queue<X>::queue(int size)
{
    arr = new X[size];
    capacity = size;
    front = 0;
    rear = -1;
    count = 0;
}

template <class X>
void queue<X>::dequeue()
{
    if (isEmpty())
    {
        cout << "Queue is empty.\n";
        return;
    }

    front = (front + 1) % capacity;
    count--;
}

template <class X>
void queue<X>::enqueue(X item)
{
    if (isFull())
    {
        cout << "Queue is full.\n";
        return;
    }

    rear = (rear + 1) % capacity;
    arr[rear] = item;
    count++;
}

template <class X>
X queue<X>::peek() {
    if (isEmpty()) {
        cout << "Queue is empty.\n";
        return -1;
    }
    return arr[front];
}

template <class X>
int queue<X>::size() {
    return count;
}

template <class X>
bool queue<X>::isEmpty() {
    return (size() == 0);
}

template <class X>
bool queue<X>::isFull() {
    return (size() == capacity);
}

//---------------------------------------------------------------------------

template <class X>
class stack {
    X *arr;
    int top;
    int capacity;

public:
    explicit stack(int size = SIZE);   // constructor
    void push(X);
    X pop();
    X peek();
    int size();
    bool isEmpty();
    bool isFull();


    // destructor
    ~stack() {
        free(arr);
//        delete[] arr;
    }
};


template <class X>
stack<X>::stack(int size)
{
    arr = new X[size];
    capacity = size;
    top = -1;
}

template <class X>
void stack<X>::push(X x)
{
    if (isFull())
    {
        cout << "Stack is full.\n";
        return;
    }
    arr[++top] = x;
}

template <class X>
X stack<X>::pop()
{
    if (isEmpty()) {
        cout << "Stack is empty.\n";
        return 0;
    }

    return arr[top--];
}

template <class X>
X stack<X>::peek()
{
    if (!isEmpty())
        return arr[top];
}

template <class X>
int stack<X>::size()
{
    return top + 1;
}

template <class X>
bool stack<X>::isEmpty()
{
    return top == -1;
}

template <class X>
bool stack<X>::isFull()
{
    return top == capacity - 1;
}


//-------------------------------------Passengers----------------------------------------
class passenger {
public:
    stack<string> foods;
    string nationalCode;
    passenger* next;
    explicit passenger(string&);

};
passenger::passenger(string &nationalCode)  {
    this->next = nullptr;
    this->nationalCode = nationalCode;
}


class linkedList {
public:
    passenger* head;
    linkedList();
    void addPassenger(string&, stack<string>&);
    int count();
    void displayFoods(string&);

};
linkedList::linkedList() {
    head = nullptr;

}
void linkedList::addPassenger(string &nationalCode, stack<string> &foods) {
    if(head == nullptr) {
        head = new passenger(nationalCode);
        head->foods = foods;
    } else{
        passenger *copyHead = head;
        while(copyHead->next != nullptr)
            copyHead = copyHead->next;
        passenger* temp;
        temp = new passenger(nationalCode);
        temp->foods = foods;
        temp->next = nullptr;
        copyHead->next = temp;
    }
}
int linkedList::count() {
    int counter = 0;
    passenger * current = head;
    while(current) {
        counter++;
        current = current->next;
    }
    return counter;
}
void linkedList::displayFoods(string &nationalCode) {
    passenger* current = head;
    while (current->nationalCode != nationalCode)
        current = current->next;
    stack<string> temp = current->foods;
    cout << "Foods for passenger with National Code : " << nationalCode << endl;
    while (!temp.isEmpty()) {
        cout << "\t" << temp.pop() << endl;
    }
}

//---------------------------------------Stations--------------------------------------------

class station {
public:
    int number;
    linkedList firstTrain;
    linkedList secondTrain;

    station **forward;
    station(int, int);
};

station::station(int key, int level) {
    this->number = key;
    forward = new station*[level+1];
    memset(forward, 0, sizeof(station*)*(level+1));
};

class SkipList
{
    int MAXLVL;
    float P;
    int level;

    station *header;
public:
    SkipList(int, float);
    int randomLevel();
    station* createNode(int, int);
    void insertElement(int);
    station* findStation(int);
    void displayList();
};

SkipList::SkipList(int MAXLVL, float P)
{
    this->MAXLVL = MAXLVL;
    this->P = P;
    level = 0;

    header = new station(-1, MAXLVL);
};

int SkipList::randomLevel()
{
    float r = (float)rand()/RAND_MAX;
    int lvl = 0;
    while(r < P && lvl < MAXLVL)
    {
        lvl++;
        r = (float)rand()/RAND_MAX;
    }
    return lvl;
};

station* SkipList::createNode(int key, int level)
{
    auto *n = new station(key, level);
    return n;
};

void SkipList::insertElement(int key)
{
    station *current = header;
    station *update[MAXLVL+1];

    memset(update, 0, sizeof(station*)*(MAXLVL+1));

    for(int i = level; i >= 0; i--) {
        while(current->forward[i] != nullptr && current->forward[i]->number < key)
            current = current->forward[i];
        update[i] = current;
    }
    current = current->forward[0];
    if (current == nullptr || current->number != key) {
        int rlevel = randomLevel();

        if(rlevel > level) {
            for(int i=level+1;i<rlevel+1;i++)
                update[i] = header;

            level = rlevel;
        }
        station* temp = createNode(key, rlevel);

        for(int i=0; i <= rlevel; i++) {
            temp->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = temp;
        }
    }
};

station* SkipList::findStation(int key)
{
    station *current = header;

    for(int i = level; i >= 0; i--)
    {
        while(current->forward[i] && current->forward[i]->number < key)
            current = current->forward[i];

    }
    current = current->forward[0];
    if(current and current->number == key)
        return current;
};

void SkipList::displayList()
{
    cout<<"\n*****Stations List*****"<<"\n";
    for(int i=0; i <= level; i++)
    {
        station *node = header->forward[i];
        cout << "Level " << i << ": ";
        while(node != nullptr) {
            cout << node->number << " ";
            node = node->forward[i];
        }
        cout << "\n";
    }
};

int main() {
    srand((unsigned)time(0));
    int firstTrainCapacity = 0;
    int secondTrainCapacity = 0;
    int usedStations = 0;

    SkipList mainList(5, 0.5);
    for (int i = 0; i < 50; ++i) {
        mainList.insertElement(i + 1);
    }

    SetConsoleTextAttribute(hConsole, 11);
    cout << "\t\t\t\tWELCOME ^_____^" << endl;
    cout << "Enter the first train's capacity : ";
    cin >> firstTrainCapacity;
    cout << "Enter the second train's capacity : ";
    cin >> secondTrainCapacity;
    SetConsoleTextAttribute(hConsole, 13);
    cout << "\t\t\t\tFIRST TRAIN" << endl;
    cout << "In how many stations passengers have moved from the first train to the second train? : ";
    cin >> usedStations;
    for (int i = 0; i < usedStations; ++i) {
        cout << "Which station? : ";
        int currentStation;
        cin >> currentStation;
        cout << "How many passengers have moved ? : ";
        int movedPassengers;
        cin >> movedPassengers;
        for (int j = 0; j < movedPassengers ; ++j) {
            cout << "Enter the ID of the passenger : ";
            string id;
            cin >> id;
            cout << "how much food do you want to order? :";
            int numberOfOrders;
            cin >> numberOfOrders;
            cout << "Enter food name(s) : ";
            stack <string> orderedFoods;
            for (int k = 0; k < numberOfOrders; ++k) {
                string temp;
                cin >> temp;
                orderedFoods.push(temp);
            }
            mainList.findStation(currentStation)->firstTrain.addPassenger(id,orderedFoods);
            //mainList.findStation(currentStation)->firstTrain.displayFoods(id);
        }
    }

    SetConsoleTextAttribute(hConsole, 8);
    cout << "\t\t\t\tSECOND TRAIN" << endl;
    cout << "In how many stations passengers have moved from the second train to the first train? : ";
    cin >> usedStations;
    for (int i = 0; i < usedStations; ++i) {
        cout << "Which station? : ";
        int currentStation;
        cin >> currentStation;
        cout << "How many passengers have moved ? : ";
        int movedPassengers;
        cin >> movedPassengers;
        for (int j = 0; j < movedPassengers ; ++j) {
            cout << "Enter the ID of the passenger : ";
            string id;
            cin >> id;
            cout << "how much food do you want to order? :";
            int numberOfOrders;
            cin >> numberOfOrders;
            cout << "Enter food name(s) : ";
            stack <string> orderedFoods;
            for (int k = 0; k < numberOfOrders; ++k) {
                string temp;
                cin >> temp;
                orderedFoods.push(temp);
            }
            mainList.findStation(currentStation)->secondTrain.addPassenger(id,orderedFoods);
            //mainList.findStation(currentStation)->secondTrain.displayFoods(id);
        }
    }
    SetConsoleTextAttribute(hConsole, 15);
    cout << "What do you want to know now? -____-" << endl;
    SetConsoleTextAttribute(hConsole, 10);
    cout << "1.How many passengers have moved totally in station x ?"<< endl;
    SetConsoleTextAttribute(hConsole, 11);
    cout << "2.How many passengers is still in the train at the last station?" << endl;
    SetConsoleTextAttribute(hConsole, 12);
    cout << "3.List of passenger's orders" << endl;
    SetConsoleTextAttribute(hConsole, 13);
    cout << "4.Give everybody their orders in the first train" << endl;
    SetConsoleTextAttribute(hConsole, 14);
    cout << "5.Give everybody their orders in the second train" << endl;
    SetConsoleTextAttribute(hConsole, 15);
    cout << "6.Exit" << endl;
    int whichOne;
    while(true) {
        SetConsoleTextAttribute(hConsole, 15);
        cout << "Which one ? :";
        cin >> whichOne;
        if (whichOne == 1) {
            SetConsoleTextAttribute(hConsole, 10);
            cout << "Which station ? :";
            int currentstation;
            cin >> currentstation;
            cout << "From  the first train to the second one : "
                 << mainList.findStation(currentstation)->firstTrain.count()
                 << endl;
            cout << "From  the second train to the first one : "
                 << mainList.findStation(currentstation)->secondTrain.count() << endl;
            int sum;
            sum = mainList.findStation(currentstation)->firstTrain.count();
            sum += mainList.findStation(currentstation)->secondTrain.count();
            cout << "Total : " << sum << endl;
        }
        if (whichOne == 2) {
            SetConsoleTextAttribute(hConsole, 11);
            int sum1 = 0;
            int sum2 = 0;
            for (int i = 1; i < 51; ++i) {
                sum1 += mainList.findStation(i)->firstTrain.count();
                sum2 += mainList.findStation(i)->secondTrain.count();
            }
            int current1, current2;
            current1 = firstTrainCapacity - sum1 + sum2;
            current2 = secondTrainCapacity - sum2 + sum1;
            cout << current1 << " passengers are still in the first train." << endl;
            cout << current2 << " passengers are still in the second train." << endl;
        }
        if (whichOne == 3) {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Enter the passenger's ID : ";
            string id;
            cin >> id;
            int wantedi = 0;
            for (int i = 1; i < 51; ++i) {
                passenger *temp = mainList.findStation(i)->firstTrain.head;
                while (temp) {
                    if (temp->nationalCode == id) {
                        wantedi = i;
                    }
                    temp = temp->next;
                }
            }
            if (wantedi > 0)
                mainList.findStation(wantedi)->firstTrain.displayFoods(id);
            else {
                for (int i = 1; i < 51; ++i) {
                    passenger *temp = mainList.findStation(i)->secondTrain.head;
                    while (temp) {
                        if (temp->nationalCode == id) {
                            wantedi = i;
                        }
                        temp = temp->next;
                    }
                }
                mainList.findStation(wantedi)->secondTrain.displayFoods(id);
            }
        }
        if (whichOne == 4) {
            SetConsoleTextAttribute(hConsole, 13);
            for (int i = 1; i < 51 ; ++i) {
                passenger *temp = mainList.findStation(i)->firstTrain.head;
                while(temp) {
                    cout << "Foods for passenger with National Code : " << temp->nationalCode << endl;
                    while (!temp->foods.isEmpty()) {
                        cout << "\t" << temp->foods.pop() << endl;
                    }
                    temp = temp->next;
                }
            }

        }
        if (whichOne == 5) {
            SetConsoleTextAttribute(hConsole, 14);
            for (int i = 1; i < 51 ; ++i) {
                passenger *temp = mainList.findStation(i)->secondTrain.head;
                while(temp) {
                    cout << "Foods for passenger with National Code : " << temp->nationalCode << endl;
                    while (!temp->foods.isEmpty()) {
                        cout << "\t" << temp->foods.pop() << endl;
                    }
                    temp = temp->next;
                }
            }

        }
        if (whichOne == 6) {
            SetConsoleTextAttribute(hConsole, 12);
            cout << "Goodbye, ";
            SetConsoleTextAttribute(hConsole, 4);
            cout << "Have a good time!\n";
            break;
        }
    }
    system("pause");
}