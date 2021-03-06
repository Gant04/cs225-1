#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"

#include <string>
#include <iostream>

using std::pair;
using std::string;
using std::list;

/**
* @return The number of vertices in the Graph
*/
template <class V, class E>
unsigned int Graph<V,E>::numVertices() const {
  // TODO: Part 2
  return vertexMap.size();
}


/**
* The degree of the vertex. For directed: Sum of in-degree and out-degree
* @return Returns the degree of a given vertex.
* @param v Given vertex to return degree.
*/
template <class V, class E>
unsigned int Graph<V,E>::degree(const V & v) const {
  // TODO: Part 2
  unsigned retVal = 0;
  for (auto i : edgeList)
  {
    if (i.get().source() == v || i.get().dest() == v)
      retVal++;
  }
  return retVal;
}


/**
* Inserts a Vertex into the Graph.
* @param key The key of the Vertex to insert
* @return The inserted Vertex
*/
template <class V, class E>
V & Graph<V,E>::insertVertex(std::string key)
{
  // TODO: Part 2
  V & v = *(new V(key));
  vertexMap.insert(pair<string, V&>(key, v));

  list<edgeListIter> temp;
  adjList.insert(pair<string, list<edgeListIter>>(key, temp));
  return v;
}


/**
* Removes a given Vertex
* @param v The Vertex to remove
*/
template <class V, class E>
void Graph<V,E>::removeVertex(const std::string & key)
{
  // TODO: Part 2
  auto it = edgeList.begin();
  // cannot use for loop, otherwise mem error with iterator ++
  while (it != edgeList.end())
  {
    if (it->get().source().key() == key || it->get().dest().key() == key)
    {
      adjList.at(it->get().source().key()).remove(it);
      if (!edgeList.front().get().directed())
        adjList.at(it->get().dest().key()).remove(it);
      // delete it->get();
      it = edgeList.erase(it);
      continue;
    }
    it++;
  }
  // delete vertexMap.at(key).get();
  vertexMap.erase(key);
  adjList.erase(key);
}


/**
* Inserts an Edge into the Graph.
* @param v1 The source Vertex
* @param v2 The destination Vertex
* @return The inserted Edge
*/
template <class V, class E>
E & Graph<V,E>::insertEdge(const V & v1, const V & v2)
{
  // TODO: Part 2
  E & e = *(new E(v1, v2));
  edgeList.push_back(e);

  auto it = edgeList.begin();
  for (it = edgeList.begin(); it != edgeList.end(); it++)
  {
    if (it->get() == e)
      break;
  }

  adjList.at(v1.key()).push_back(it);

  if (!edgeList.front().get().directed())
    adjList.at(v2.key()).push_back(it);

  return e;
}


/**
* Removes an Edge from the Graph. Consider both the undirected and directed cases.
* min(degree(key1), degree(key2))
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const std::string key1, const std::string key2)
{
  // TODO: Part 2
  if (!edgeList.front().get().directed())
  {
    for (auto i = edgeList.begin(); i != edgeList.end(); i++)
    {
      if ((i->get().source() == key1 && i->get().dest() == key2) || (i->get().source() == key2 && i->get().dest() == key1))
      {
        adjList.at(key1).remove(i);
        adjList.at(key2).remove(i);
        // delete i->get();
        edgeList.erase(i);
        break;
      }
    }
  }
  else
  {
    for (auto i = edgeList.begin(); i != edgeList.end(); i++)
    {
      if ((i->get().source() == key1 && i->get().dest() == key2))
      {
        adjList.at(key1).remove(i);
        // delete i->get();
        edgeList.erase(i);
        break;
      }
    }
  }
}


/**
* Removes an Edge from the Graph given by the location of the given iterator into the edge list.
* @param it An iterator at the location of the Edge that
* you would like to remove
*/
template <class V, class E>
void Graph<V,E>::removeEdge(const edgeListIter & it)
{
  adjList.at(it->get().source().key()).remove(it);
  if (!edgeList.front().get().directed())
    adjList.at(it->get().dest().key()).remove(it);
  edgeList.erase(it);
  // TODO: Part 2
}


/**
* Return the list of incident edges from a given vertex.
* For the directed case, consider all edges that has the vertex as either a source or destination.
* @param key The key of the given vertex
* @return The list edges (by reference) that are adjacent to the given vertex
*/
template <class V, class E>
const std::list<std::reference_wrapper<E>> Graph<V,E>::incidentEdges(const std::string key) const {
  // TODO: Part 2
  std::list<std::reference_wrapper<E>> edges;
  for (auto i : edgeList)
  {
    if (i.get().source().key() == key || i.get().dest().key() == key)
      edges.push_back(i);
  }
  return edges;
}


/**
* Return whether the two vertices are adjacent to one another. Consider both the undirected and directed cases.
* When the graph is directed, v1 and v2 are only adjacent if there is an edge from v1 to v2.
* @param key1 The key of the source Vertex
* @param key2 The key of the destination Vertex
* @return True if v1 is adjacent to v2, False otherwise
*/
template <class V, class E>
bool Graph<V,E>::isAdjacent(const std::string key1, const std::string key2) const {
  // TODO: Part 2
  for (auto i : adjList.at(key1))
  {
    if (i->get().dest() == key2)
      return true;
  }
  return false;
}
