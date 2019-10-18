/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "NimLearner.h"
#include <ctime>

using namespace std;

/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true)
{
    /* Your code goes here! */

    // initialize the vertices and edges
    for (int j = 1; j < 3; j++)
    {
      for (int i = (int)startingTokens; i >= 0; i--)
      {
        Vertex temp = "p";
        temp += to_string(j);
        temp += "-";
        temp += to_string(i);
        g_.insertVertex(temp);

        // sets the start vertex
        if (i == (int)startingTokens && j == 1)
          startingVertex_ = temp;

        // set edges
        if (i > 0)
        {
          Vertex temp_1 = "p";
          temp_1 += to_string(j%2 + 1);
          temp_1 += "-";
          temp_1 += to_string(i - 1);
          g_.insertEdge(temp, temp_1);
          g_.setEdgeWeight(temp, temp_1, 0);
        }
        if (i > 1)
        {
          Vertex temp_1 = "p";
          temp_1 += to_string(j%2 + 1);
          temp_1 += "-";
          temp_1 += to_string(i - 2);
          g_.insertEdge(temp, temp_1);
          g_.setEdgeWeight(temp, temp_1, 0);
        }
      }
    }
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const
{
  vector<Edge> path;
  /* Your code goes here! */
  Vertex curr = startingVertex_;
  while(1)
  {
    // end condition
    if (curr == "p1-0" || curr == "p2-0")
      break;

    int i = rand()%2 + 1; // random walk
    int a = stoi(curr.substr(3)) - i;
    a = a<0 ? 0:a;
    Vertex temp = "p" + to_string((int)curr[1]%2 + 1) + "-" + to_string(a);

    path.push_back(Edge(curr, temp));
    curr = temp;
  }
  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> & path)
{
  /* Your code goes here! */
  char a = path[path.size() - 1].dest[1]; // get winner
  if (a == '1') // player 2 wins
  {
    for (int i = 0; i < (int)path.size(); i++)
    {
      int temp = g_.getEdgeWeight(path[i].source, path[i].dest);
      if (i % 2) // edge from p2 to p1
        g_.setEdgeWeight(path[i].source, path[i].dest, temp + 1);
      else // edge from p1 to p2
        g_.setEdgeWeight(path[i].source, path[i].dest, temp - 1);
    }
  }
  else // player 1 wins
  {
    for (int i = 0; i < (int)path.size(); i++)
    {
      int temp = g_.getEdgeWeight(path[i].source, path[i].dest);
      if (i % 2) // edge from p2 to p1
        g_.setEdgeWeight(path[i].source, path[i].dest, temp - 1);
      else // edge from p1 to p2
        g_.setEdgeWeight(path[i].source, path[i].dest, temp + 1);
    }
  }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex & v : g_.getVertices()) {
    for (const Vertex & w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold)           { g_.setEdgeLabel(v, w, "WIN"); }
      else if (weight < -1 * threshold) { g_.setEdgeLabel(v, w, "LOSE"); }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph & NimLearner::getGraph() const {
  return g_;
}