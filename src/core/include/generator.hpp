#pragma once

#include <vector>
#include <tuple>
#include <algorithm>
#include <random>
#include <iostream>
#include <iomanip>
using namespace std;

enum Tile{
  tWall = 0,
  tFloor = 1,
  tBirth = 10,
  tExit = 11,
  tTreasure = 20,
  tBoss = 30
};

struct Room{
  int x1, y1, x2, y2;
  int cx() const;
  int cy() const;
  int area() const;
};

// Disjoint Set Union
struct DSU{
  vector<int> p;
  DSU(int n);
  int find(int x);
  bool unite(int x, int y);
};

// generate map
vector<vector<int>> generateMap(int width = 100, int height = 100, int minRoom = 4, int maxRoom = 8);