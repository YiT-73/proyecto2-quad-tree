#ifndef STRUCTURE_H
#define STRUCTURE_H

#include <sstream>
#include <vector>

using namespace std;

struct Particle {
  int id;
  double x, y;   // posición
  double vx, vy; // velocidad
  double radius; // radio angulo de giro
};

struct Rectangle {
  double x, y;
  double w, h;

  bool contains(const Particle &p) const;
  bool intersects(const Rectangle &other) const;
};

struct QueryStats {
  long long nodesVisited;
  long long particlesChecked;
  long long candidatesFound;
};

struct CollisionStats {
  int collisions;
  long long comparisons;
  long long nodesVisited;
  long long particlesChecked;
  long long candidatesFound;
};

class QuadNode {
public:
  Rectangle boundary;
  vector<Particle> particles;
  bool divided;

  QuadNode *northeast;
  QuadNode *northwest;
  QuadNode *southeast;
  QuadNode *southwest;

  QuadNode(Rectangle boundary);
  ~QuadNode();

  bool insert(const Particle &p, int capacity);
  void subdivide(int capacity);
  void queryRange(const Rectangle &range, vector<Particle> &found,
                  QueryStats &stats) const;
  void collectBoundaries(vector<Rectangle> &boundaries) const;
  void clear();
  void serialize(std::ostringstream &os) const;
};

class QuadTree {
private:
  QuadNode *root;
  int capacity;
  Rectangle boundary;

public:
  QuadTree(Rectangle boundary, int capacity);
  ~QuadTree();

  bool insert(const Particle &p);
  void queryRange(const Rectangle &range, vector<Particle> &found) const;
  void queryRange(const Rectangle &range, vector<Particle> &found,
                  QueryStats &stats) const;
  vector<Particle> queryNearPoint(double x, double y, double radius) const;
  vector<Particle> queryNearPoint(double x, double y, double radius,
                                  QueryStats &stats) const;
  void collectBoundaries(vector<Rectangle> &boundaries) const;
  void clear();
  void serialize(std::ostringstream &os) const;
};

void updateParticles(vector<Particle> &particles, double width, double height);

double distanceBetween(const Particle &a, const Particle &b);
bool areColliding(const Particle &a, const Particle &b);

int bruteForceCollisions(const vector<Particle> &particles);
int quadTreeCollisions(QuadTree &qt, const vector<Particle> &particles);
CollisionStats bruteForceCollisionStats(const vector<Particle> &particles);
CollisionStats quadTreeCollisionStats(QuadTree &qt,
                                      const vector<Particle> &particles);

#endif
