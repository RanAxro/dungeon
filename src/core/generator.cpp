#include "include/generator.hpp"


int Room::cx() const{
  return (x1 + x2) / 2;
}
int Room::cy() const{
  return (y1 + y2) / 2;
}
int Room::area() const{
  return (x2 - x1) * (y2 - y1);
}


// Disjoint Set Union

DSU::DSU(int n) : p(n){
  iota(p.begin(), p.end(), 0);
}
int DSU::find(int x){
  return p[x] == x ? x : p[x] = find(p[x]);
}
bool DSU::unite(int x, int y){
  x = find(x),
  y = find(y);
  if(x == y) return false;
  p[y] = x;
  return true;
}



// generate map

vector<vector<int>> generateMap(int width, int height, int minRoom, int maxRoom){

  // 1. 初始化全墙
  vector<vector<int>> M(height, vector<int>(width, Tile::tWall));
  auto inside = [&](int x, int y){
    return x >= 0 && x < width && y >= 0 && y < height;
  };

  // 2. 随机参数
  std::mt19937 rng(std::random_device{}());
  const int roomTries = 50;

  // 3. 生成不重叠房间
  vector<Room> rooms;
  auto collide = [&](const Room &r){
    for(auto &R : rooms)
      if(!(r.x2 <= R.x1 || r.x1 >= R.x2 || r.y2 <= R.y1 || r.y1 >= R.y2))
        return true;
    return false;
  };
  for(int t = 0; t < roomTries; ++t){
    int w = uniform_int_distribution<int>(minRoom, maxRoom)(rng);
    int h = uniform_int_distribution<int>(minRoom, maxRoom)(rng);
    int x = uniform_int_distribution<int>(1, width - w - 1)(rng);
    int y = uniform_int_distribution<int>(1, height - h - 1)(rng);
    Room r{x, y, x + w, y + h};
    if(!collide(r)){
      rooms.push_back(r);
      // 挖成地板
      for(int yy = r.y1; yy < r.y2; ++yy)
        for(int xx = r.x1; xx < r.x2; ++xx)
          M[yy][xx] = Tile::tFloor;
    }
  }
  if (rooms.empty()) return M; // 容错

  // 4. 连接房间（Kruskal）
  vector<tuple<int, int, int>> edges; // (dist, i, j)
  for(int i = 0; i < (int)rooms.size(); ++i)
    for(int j = i + 1; j < (int)rooms.size(); ++j){
      int dx = rooms[i].cx() - rooms[j].cx();
      int dy = rooms[i].cy() - rooms[j].cy();
      int d = abs(dx) + abs(dy);
      edges.emplace_back(d, i, j);
    }
  sort(edges.begin(), edges.end());
  DSU dsu(rooms.size());
  auto digCorridor = [&](int x1, int y1, int x2, int y2){
    // 先横后竖
    int x = x1;
    while(x != x2){
      if(inside(x, y1)) M[y1][x] = Tile::tFloor;
      x += (x < x2 ? 1 : -1);
    }
    int y = y1;
    while(y != y2){
      if(inside(x2, y)) M[y][x2] = Tile::tFloor;
      y += (y < y2 ? 1 : -1);
    }
    if(inside(x2, y2)) M[y2][x2] = Tile::tFloor;
  };
  for(auto [d, i, j] : edges){
    if(dsu.unite(i, j))
      digCorridor(rooms[i].cx(), rooms[i].cy(), rooms[j].cx(), rooms[j].cy());
  }

  // 5. 随机出生房
  uniform_int_distribution<int> roomDist(0, (int)rooms.size() - 1);
  int birthIdx = roomDist(rng);
  Room &birth = rooms[birthIdx];

  // 6. 随机出口房（可与出生房相同，但位置不同）
  int exitIdx = roomDist(rng);
  Room &exitRoom = rooms[exitIdx];

  // 7. 放置元素
  auto freePos = [&](const Room &r){
    vector<pair<int, int>> cand;
    for(int y = r.y1; y < r.y2; ++y)
      for(int x = r.x1; x < r.x2; ++x)
        if(M[y][x] == Tile::tFloor) cand.emplace_back(x, y);
    return cand;
  };
  // 7.1 出生点
  auto birthPos = freePos(birth);
  if(!birthPos.empty()){
    auto [x, y] = birthPos[rng() % birthPos.size()];
    M[y][x] = Tile::tBirth;
  }
  // 7.2 出口
  auto exitPos = freePos(exitRoom);
  if(!exitPos.empty()){
    auto [x, y] = exitPos[rng() % exitPos.size()];
    M[y][x] = Tile::tExit;
  }
  // 7.3 宝藏 & BOSS
  for(int i = 0; i < (int)rooms.size(); ++i){
    if(i == birthIdx) continue; // 出生房不放宝藏/BOSS
    auto cand = freePos(rooms[i]);
    if(cand.empty()) continue;
    shuffle(cand.begin(), cand.end(), rng);

    // 最多 2 宝藏
    int nt = min(2, (int)cand.size());
    for(int t = 0; t < nt; ++t){
      auto [x, y] = cand.back(); cand.pop_back();
      M[y][x] = Tile::tTreasure;
    }
    // 最多 1 BOSS
    if(!cand.empty() && uniform_int_distribution<int>(0, 3)(rng) == 0){
      auto [x, y] = cand.back(); cand.pop_back();
      M[y][x] = Tile::tBoss;
    }
  }

  return M;
}