#include "SkipList.h"


Node *SkipList::skipSearch(int key) {
    Node*current=head;//从头开始查找
    //从顶向下查找
    while(current->below!= nullptr){

        //如果一层查找完了，就向下一层查找
        current=current->below;
        cout<<"go to the next level"<<endl;
        //如果当前值小于要查找的值，继续查找
        while(key>=current->next->key){
            cout<<"Now Comparing "<<key<<" and "<< current->next->key<<endl;
            printSkipSearch(current->next->key);
            //与下一个结点比较
            current=current->next;
        }

        cout<<endl;
    }
    cout<<"arrive at the bottom level"<<endl;
    cout<<key<<" < "<<current->next->key<<" , here"<<endl;
    return current;
}

void SkipList::printSkipSearch(int key) {
    //注意清空上一次界面的图画
    window->clear(sf::Color::White);
    const int nodeWidth = 40;
    const int nodeHeight = 40;
    const int spacingX = 10;
    const int spacingY = 10;

    Node* starting = head;
    Node* highestLevel = starting;
    int level = heightOfSkipList;
    int offsetY = 0;

    sf::Font font;//设置字体 注意一定要设置，否则没法显示数据
    font.loadFromFile("D:\\Documents\\codes in Clion\\skiplist_SFML\\typewriter\\TYPEWR__.TTF");

    //按层打印
    while (highestLevel != nullptr) {
        //显示Level的图形
        sf::Text levelText;
        levelText.setFont(font);
        levelText.setCharacterSize(20);
        levelText.setString("Level: " + std::to_string(level));
        levelText.setFillColor(sf::Color::Black);
        levelText.setPosition(10, offsetY+17);
        offsetY+=10;//图形添加后在Y轴添加偏移
        window->draw(levelText);
        //下面是显示出当前行的数据
        int offsetX = 0;
        sf::Color color;
        while (starting != nullptr) {
            if(starting->key==key){
                color=sf::Color::Red;//将当前搜索的元素标红，以显示出搜索的过程
            }
            else{
                color=sf::Color::Black;
            }
            sf::RectangleShape shape(sf::Vector2f(nodeWidth, nodeHeight));
            shape.setFillColor(sf::Color::White);
            shape.setOutlineThickness(2);
            shape.setOutlineColor(color);
            shape.setPosition(offsetX + 10, offsetY + 30);

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(18);
            text.setFillColor(color);
            text.setStyle(sf::Text::Bold);
            //用于head和tail显示无穷
            if (starting->key == INT_MIN) {
                text.setString("-INF");
            } else if (starting->key == INT_MAX) {
                text.setString("INF");
            } else {
                text.setString(std::to_string(starting->key));
            }
            Sleep(101-speed);
            text.setPosition(offsetX + 15, offsetY + 30 + 15);
            window->draw(shape);
            window->draw(text);

            offsetX += nodeWidth + spacingX;
            starting = starting->next;
        }

        highestLevel = highestLevel->below;
        starting = highestLevel;
        level--;
        offsetY += nodeHeight + spacingY;
    }

    window->display();
}

Node *SkipList::skipInsert(int key) {
    Node *position = skipSearch(key);
    Node *q;
    int level = -1;
    int numberOfHeads = -1;
    if (position->key == key) {
        return position;
    }

    //用do while是因为默认插入结点至少要在最底层插入一个
    do {
        numberOfHeads++;
        level++;
        canIncreaseLevel(level);

        q = position;
        //试图向上增长一层
        while (position->above == nullptr) {
            position = position->prev;//向前查找插入的位置
        }
        position=position->above;
        q = insertAfterAbove(position, q, key);
    } while (addLevelOrNot());

    printSkipList();
    return q;
}



bool SkipList::addLevelOrNot() const {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<float> dis(0.0f, 1.0f);
    //如果随机生成的数小于probability,则添加新的一层
    if(dis(gen)<probability)
        return true;
    else
        return false;
}

void SkipList::canIncreaseLevel(int level) {
    if(level >= heightOfSkipList){
        heightOfSkipList++;
        addEmptyLevel();
    }
}

//添加一层空层
void SkipList::addEmptyLevel() {
    //生成新的空层的头结点为尾结点
    Node*newHeadNode = new Node(NEG_INF);
    Node*newTailNode = new Node(POS_INF);

    newHeadNode->next = newTailNode;
    newHeadNode->below = head;
    newTailNode->prev = newHeadNode;
    newTailNode->below = tail;

    head->above=newHeadNode;
    tail->above=newTailNode;

    head = newHeadNode;
    tail = newTailNode;
}

//在当前层的上面一层插入结点
Node *SkipList::insertAfterAbove(Node*position,Node*q,int key) {
    Node*newNode=new Node(key);
    Node*nodeBeforeNewNode=position->below->below;
    setBeforeAndAfterReferences(q,newNode);
    setAboveAndBelowReferences(position,key,newNode,nodeBeforeNewNode);

    return newNode;
}

//将新结点newNode插入到结点q之后 这样就将新结点插入到了最下面一层
//@
void SkipList::setBeforeAndAfterReferences(Node *q, Node *newNode) {
    newNode->next=q->next;
    newNode->prev=q;
    q->next->prev=newNode;
    q->next=newNode;
}

void SkipList::setAboveAndBelowReferences(Node *&position, int key, Node *&newNode, Node *&nodeBeforeNewNode) {
    if(nodeBeforeNewNode!= nullptr){
        while(true){
            if(nodeBeforeNewNode->next->key!=key){
                nodeBeforeNewNode=nodeBeforeNewNode->next;
            }
            else{
                break;
            }
        }

        newNode->below=nodeBeforeNewNode->next;
        nodeBeforeNewNode->next->above=newNode;
    }
    if(position!= nullptr){
        if(position->next->key==key){
            newNode->above=position->next;
        }
    }
}


Node *SkipList::remove(int key) {
    Node *nodeToRemove = skipSearch(key);
    if (nodeToRemove->key != key) {
        return nullptr;
    }
    removeOneNode(nodeToRemove);

    while(nodeToRemove!= nullptr){
        removeOneNode(nodeToRemove);
        if(nodeToRemove->above!= nullptr){
            nodeToRemove=nodeToRemove->above;
        }
        else{
            break;
        }
    }
    printSkipList();
    return nodeToRemove;
}

/*
 * @brief 删除一个结点
 * @param 要删除的结点
 * @return void
 */
void SkipList::removeOneNode(Node *&nodeToRemove) {
    Node *afterNodeToRemove = nodeToRemove->next;
    Node *beforeNodeToRemove = nodeToRemove->prev;
    beforeNodeToRemove->next = afterNodeToRemove;
    afterNodeToRemove->prev = beforeNodeToRemove;
}

/*
 * @brief 析构函数，删除操作过程中产生的结点
 * @param 无
 *
 */

SkipList::~SkipList() {
    queue<Node*> nodeQueue;
    nodeQueue.push(head);
    //将所有层的头结点都放入队列
    while(!head->above){
        nodeQueue.push(head->above);
    }
    while(!nodeQueue.empty()){
        Node*current=nodeQueue.front();
        Node*next=current->next;//删除这条链上所有的指针
        while(next){
            delete current;
            current=next;
            next=next->next;
        }
        delete current;
    }
    delete window;
}

/*
 * @brief 初始化SkipList
 * @param 空
 * @return 空
 */
void SkipList::initialize() {
    heightOfSkipList=0;
    head = new Node(NEG_INF); // 表头初始化为负无穷大
    tail = new Node(POS_INF); // 表尾初始化为正无穷大
    head->next = tail;
    tail->prev = head;
}
/*
 * @brief 图形化初始化
 * @param void
 * @return void
 */
void SkipList::initializeSFML() {
    heightOfSkipList=0;
    head = new Node(NEG_INF); // 表头初始化为负无穷大
    tail = new Node(POS_INF); // 表尾初始化为正无穷大
    head->next = tail;
    tail->prev = head;
    printSkipList();
}

/*
 * @brief 初始化SkipList的最大层次和随机生成一层的可能性
 * @param Probability SkipList随机生成一层的概率
 * @return void
 */
void SkipList::initialize( float Probability) {
    initialize();
    this->probability = Probability;
}

/*
 * @brief 显示操作菜单
 * @param void
 * @return void
 */
void menu(){
    cout<<"1、初始化SkipList"<<endl;
    cout<<"2、插入"<<endl;
    cout<<"3、搜索"<<endl;
    cout<<"4、删除"<<endl;
    cout<<"5、设置演示速度（1-100）"<<endl;
    cout<<"输入你的选择"<<endl;
}
/*
 * @brief 图形化操作接口
 * @param void
 * @return void
 */

void SkipList::operate() {
    menu();
    bool isInitialized=false;
    int select;
    while(true){
        cin>>select;
        switch (select) {
            case 1:{
                initializeSFML();
                isInitialized= true;
                break;
            }
            case 2:{
                if(!isInitialized){
                    cout<<"请先初始化!"<<endl;
                    break;
                }
                cout<<"输入插入的值:";
                int value;
                cin>>value;
                skipInsert(value);
                break;
            }
            case 3:{
                if(!isInitialized){
                    cout<<"请先初始化!"<<endl;
                    break;
                }
                cout<<"输入搜索的值:"<<endl;
                int value;
                cin>>value;
                skipSearch(value);
                break;
            }
            case 4:{
                if(!isInitialized){
                    cout<<"请先初始化!"<<endl;
                    break;
                }
                cout<<"输入删除的值:";
                int value;
                cin>>value;
                remove(value);
                break;
            }
            case 5:{
                cout<<"输入演示速度:";
                int s;
                while(cin>>s){
                    if(s<1||s>100){
                        cout<<"输入错误，请重新输入!"<<endl;
                    }
                    else{
                        break;
                    }
                }
                setSpeed(s);
                cout<<"设置成功!"<<endl;
                break;
            }
            default:{
                cout<<"输入错误,请重新输入:";
                break;
            }
        }
    }

}

/*
 * @brief 图形化操作构造函数
 * @param window 图形化主页面
 * @return void
 */
SkipList::SkipList(sf::RenderWindow *window) {
    this->window=window;
}

/*


/*
 * @brief 图形化显示SkipList
 * @param void
 * @return void
 */
void SkipList::printSkipList() {
    //注意清空上次的界面
    window->clear(sf::Color::White);
    //方块的宽、高以及在 X Y 方向上的间隔
    const int nodeWidth = 40;
    const int nodeHeight = 40;
    const int spacingX = 10;
    const int spacingY = 10;

    //从头还是打印
    Node* starting = head;
    Node* highestLevel = starting;
    int level = heightOfSkipList;
    int offsetY = 0;

    sf::Font font;
    font.loadFromFile("D:\\Documents\\codes in Clion\\skiplist1_1\\typewriter\\TYPEWR__.TTF");

    while (highestLevel != nullptr) {
        sf::Text levelText;
        levelText.setFont(font);
        levelText.setCharacterSize(20);
        levelText.setString("Level: " + std::to_string(level));
        levelText.setFillColor(sf::Color::Black);
        levelText.setPosition(10, offsetY+17);
        offsetY+=10;
        window->draw(levelText);

        int offsetX = 0;
        while (starting != nullptr) {
            sf::RectangleShape shape(sf::Vector2f(nodeWidth, nodeHeight));
            shape.setFillColor(sf::Color::White);
            shape.setOutlineThickness(2);
            shape.setOutlineColor(sf::Color::Black);
            shape.setPosition(offsetX + 10, offsetY + 30);

            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(18);
            text.setFillColor(sf::Color::Black);
            text.setStyle(sf::Text::Bold);

            if (starting->key == INT_MIN) {
                text.setString("-INF");
            } else if (starting->key == INT_MAX) {
                text.setString("INF");
            } else {
                text.setString(std::to_string(starting->key));
            }

            text.setPosition(offsetX + 15, offsetY + 30 + 15);
            Sleep(101-speed);
            window->draw(shape);
            window->draw(text);

            offsetX += nodeWidth + spacingX;
            starting = starting->next;
        }

        highestLevel = highestLevel->below;
        starting = highestLevel;
        level--;
        offsetY += nodeHeight + spacingY;
    }

    window->display();
}

void SkipList::setSpeed(int speed) {
    this->speed=speed;
}











