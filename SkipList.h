
#ifndef SKIPLIST_SFML_SKIPLIST_H
#define SKIPLIST_SFML_SKIPLIST_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include<windows.h>

using namespace std;
using namespace sf;
struct Node {
public:
    Node* above;
    Node* below;
    Node* next;
    Node* prev;
    int key;
    Node(int key) {
        this->key = key;
        this->above = nullptr;
        this->below = nullptr;
        this->next = nullptr;
        this->prev = nullptr;
    }
};


class SkipList {
private:
    int speed=50;//演示速度默认为50
    sf::RenderWindow* window;
    Node*head,*tail;//分别表示跳表表头和表尾
    int heightOfSkipList=0;//当前跳表的高度
    float probability=0.5f;//生成随机层次的概率 默认为0.5
    const int NEG_INF=INT_MIN, POS_INF=INT_MAX;

    // ** 搜索、插入、删除等所需函数**//
    bool addLevelOrNot() const;
    void canIncreaseLevel(int level);
    void addEmptyLevel();
    Node*insertAfterAbove(Node*position,Node*q,int key);
    void setBeforeAndAfterReferences(Node*q,Node*newNode);
    void setAboveAndBelowReferences(Node*&position,int key,Node*&newNode,Node*&nodeBeforeNewNode);
    void removeOneNode(Node*&nodeToRemove);
    //** 主要函数  ** //

    void initialize(float probability);

    void printSkipList();//图形化打印SkipList
    void setSpeed(int speed);//设置演示速度
    void initialize();//初始化SkipList
    void initializeSFML();//图形化
    Node*skipSearch(int key);
    void printSkipSearch(int key);
    Node*skipInsert(int key);
    Node*remove(int key);

public:
    explicit SkipList(sf::RenderWindow*window);
    SkipList()=default;
    ~SkipList();
    void operate();

};


#endif //SKIPLIST_SFML_SKIPLIST_H
