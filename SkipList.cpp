#include "SkipList.h"


Node *SkipList::skipSearch(int key) {
    Node*current=head;//��ͷ��ʼ����
    //�Ӷ����²���
    while(current->below!= nullptr){

        //���һ��������ˣ�������һ�����
        current=current->below;
        cout<<"go to the next level"<<endl;
        //�����ǰֵС��Ҫ���ҵ�ֵ����������
        while(key>=current->next->key){
            cout<<"Now Comparing "<<key<<" and "<< current->next->key<<endl;
            printSkipSearch(current->next->key);
            //����һ�����Ƚ�
            current=current->next;
        }

        cout<<endl;
    }
    cout<<"arrive at the bottom level"<<endl;
    cout<<key<<" < "<<current->next->key<<" , here"<<endl;
    return current;
}

void SkipList::printSkipSearch(int key) {
    //ע�������һ�ν����ͼ��
    window->clear(sf::Color::White);
    const int nodeWidth = 40;
    const int nodeHeight = 40;
    const int spacingX = 10;
    const int spacingY = 10;

    Node* starting = head;
    Node* highestLevel = starting;
    int level = heightOfSkipList;
    int offsetY = 0;

    sf::Font font;//�������� ע��һ��Ҫ���ã�����û����ʾ����
    font.loadFromFile("D:\\Documents\\codes in Clion\\skiplist_SFML\\typewriter\\TYPEWR__.TTF");

    //�����ӡ
    while (highestLevel != nullptr) {
        //��ʾLevel��ͼ��
        sf::Text levelText;
        levelText.setFont(font);
        levelText.setCharacterSize(20);
        levelText.setString("Level: " + std::to_string(level));
        levelText.setFillColor(sf::Color::Black);
        levelText.setPosition(10, offsetY+17);
        offsetY+=10;//ͼ����Ӻ���Y�����ƫ��
        window->draw(levelText);
        //��������ʾ����ǰ�е�����
        int offsetX = 0;
        sf::Color color;
        while (starting != nullptr) {
            if(starting->key==key){
                color=sf::Color::Red;//����ǰ������Ԫ�ر�죬����ʾ�������Ĺ���
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
            //����head��tail��ʾ����
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

    //��do while����ΪĬ�ϲ���������Ҫ����ײ����һ��
    do {
        numberOfHeads++;
        level++;
        canIncreaseLevel(level);

        q = position;
        //��ͼ��������һ��
        while (position->above == nullptr) {
            position = position->prev;//��ǰ���Ҳ����λ��
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
    //���������ɵ���С��probability,������µ�һ��
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

//���һ��ղ�
void SkipList::addEmptyLevel() {
    //�����µĿղ��ͷ���Ϊβ���
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

//�ڵ�ǰ�������һ�������
Node *SkipList::insertAfterAbove(Node*position,Node*q,int key) {
    Node*newNode=new Node(key);
    Node*nodeBeforeNewNode=position->below->below;
    setBeforeAndAfterReferences(q,newNode);
    setAboveAndBelowReferences(position,key,newNode,nodeBeforeNewNode);

    return newNode;
}

//���½��newNode���뵽���q֮�� �����ͽ��½����뵽��������һ��
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
 * @brief ɾ��һ�����
 * @param Ҫɾ���Ľ��
 * @return void
 */
void SkipList::removeOneNode(Node *&nodeToRemove) {
    Node *afterNodeToRemove = nodeToRemove->next;
    Node *beforeNodeToRemove = nodeToRemove->prev;
    beforeNodeToRemove->next = afterNodeToRemove;
    afterNodeToRemove->prev = beforeNodeToRemove;
}

/*
 * @brief ����������ɾ�����������в����Ľ��
 * @param ��
 *
 */

SkipList::~SkipList() {
    queue<Node*> nodeQueue;
    nodeQueue.push(head);
    //�����в��ͷ��㶼�������
    while(!head->above){
        nodeQueue.push(head->above);
    }
    while(!nodeQueue.empty()){
        Node*current=nodeQueue.front();
        Node*next=current->next;//ɾ�������������е�ָ��
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
 * @brief ��ʼ��SkipList
 * @param ��
 * @return ��
 */
void SkipList::initialize() {
    heightOfSkipList=0;
    head = new Node(NEG_INF); // ��ͷ��ʼ��Ϊ�������
    tail = new Node(POS_INF); // ��β��ʼ��Ϊ�������
    head->next = tail;
    tail->prev = head;
}
/*
 * @brief ͼ�λ���ʼ��
 * @param void
 * @return void
 */
void SkipList::initializeSFML() {
    heightOfSkipList=0;
    head = new Node(NEG_INF); // ��ͷ��ʼ��Ϊ�������
    tail = new Node(POS_INF); // ��β��ʼ��Ϊ�������
    head->next = tail;
    tail->prev = head;
    printSkipList();
}

/*
 * @brief ��ʼ��SkipList������κ��������һ��Ŀ�����
 * @param Probability SkipList�������һ��ĸ���
 * @return void
 */
void SkipList::initialize( float Probability) {
    initialize();
    this->probability = Probability;
}

/*
 * @brief ��ʾ�����˵�
 * @param void
 * @return void
 */
void menu(){
    cout<<"1����ʼ��SkipList"<<endl;
    cout<<"2������"<<endl;
    cout<<"3������"<<endl;
    cout<<"4��ɾ��"<<endl;
    cout<<"5��������ʾ�ٶȣ�1-100��"<<endl;
    cout<<"�������ѡ��"<<endl;
}
/*
 * @brief ͼ�λ������ӿ�
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
                    cout<<"���ȳ�ʼ��!"<<endl;
                    break;
                }
                cout<<"��������ֵ:";
                int value;
                cin>>value;
                skipInsert(value);
                break;
            }
            case 3:{
                if(!isInitialized){
                    cout<<"���ȳ�ʼ��!"<<endl;
                    break;
                }
                cout<<"����������ֵ:"<<endl;
                int value;
                cin>>value;
                skipSearch(value);
                break;
            }
            case 4:{
                if(!isInitialized){
                    cout<<"���ȳ�ʼ��!"<<endl;
                    break;
                }
                cout<<"����ɾ����ֵ:";
                int value;
                cin>>value;
                remove(value);
                break;
            }
            case 5:{
                cout<<"������ʾ�ٶ�:";
                int s;
                while(cin>>s){
                    if(s<1||s>100){
                        cout<<"�����������������!"<<endl;
                    }
                    else{
                        break;
                    }
                }
                setSpeed(s);
                cout<<"���óɹ�!"<<endl;
                break;
            }
            default:{
                cout<<"�������,����������:";
                break;
            }
        }
    }

}

/*
 * @brief ͼ�λ��������캯��
 * @param window ͼ�λ���ҳ��
 * @return void
 */
SkipList::SkipList(sf::RenderWindow *window) {
    this->window=window;
}

/*


/*
 * @brief ͼ�λ���ʾSkipList
 * @param void
 * @return void
 */
void SkipList::printSkipList() {
    //ע������ϴεĽ���
    window->clear(sf::Color::White);
    //����Ŀ����Լ��� X Y �����ϵļ��
    const int nodeWidth = 40;
    const int nodeHeight = 40;
    const int spacingX = 10;
    const int spacingY = 10;

    //��ͷ���Ǵ�ӡ
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











