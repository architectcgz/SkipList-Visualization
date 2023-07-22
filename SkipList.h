
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
    int speed=50;//��ʾ�ٶ�Ĭ��Ϊ50
    sf::RenderWindow* window;
    Node*head,*tail;//�ֱ��ʾ�����ͷ�ͱ�β
    int heightOfSkipList=0;//��ǰ����ĸ߶�
    float probability=0.5f;//���������εĸ��� Ĭ��Ϊ0.5
    const int NEG_INF=INT_MIN, POS_INF=INT_MAX;

    // ** ���������롢ɾ�������躯��**//
    bool addLevelOrNot() const;
    void canIncreaseLevel(int level);
    void addEmptyLevel();
    Node*insertAfterAbove(Node*position,Node*q,int key);
    void setBeforeAndAfterReferences(Node*q,Node*newNode);
    void setAboveAndBelowReferences(Node*&position,int key,Node*&newNode,Node*&nodeBeforeNewNode);
    void removeOneNode(Node*&nodeToRemove);
    //** ��Ҫ����  ** //

    void initialize(float probability);

    void printSkipList();//ͼ�λ���ӡSkipList
    void setSpeed(int speed);//������ʾ�ٶ�
    void initialize();//��ʼ��SkipList
    void initializeSFML();//ͼ�λ�
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
