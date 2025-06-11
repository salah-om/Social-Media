/******************************************************************************
 * Class: SocialGraph
 *
 * Description: Represents a social network using graph data structure to
 *              store people and their friendship connections.
 *
 * Member Variables:
 *    - nodes: Vector storing all people in the network (vertices)
 *    - edgeList: Vector storing all friendship connections (edges)
 *
 *****************************************************************************/

#ifndef SOCIALGRAPH_H
#define SOCIALGRAPH_H

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class SocialGraph {
public:
    /***** Node Class  *****/
    class Node {
        string name;
    public:
        /*** Constructer ***/ 
        Node(string name = "") : name(name) {}
        /*-------------------------------------------------------------------
          Construct a Node with given name.
          
          Precondition:  name is the identifier for the person.
          Postcondition: A Node object is created with the given name.
         ------------------------------------------------------------------*/

        /*** Getter **/
        string getName() const { return name; }
        /*-------------------------------------------------------------------
          Get the name of this node.
          
          Postcondition: The name string is returned.
         ------------------------------------------------------------------*/

        /*** Overloaded operator **/
        bool operator==(const Node& other) const {
            return name == other.name;
        }
        /*-------------------------------------------------------------------
          Compare two nodes for equality.
          
          Precondition:  other is the Node to compare with.
          Postcondition: Returns true if nodes have same name, false otherwise.
         ------------------------------------------------------------------*/
    };

    /***** Edge Class (Represents a friendship connection) *****/
    class Edge {
        Node firstNode;
        Node secondNode;
    public:
        /*** Constructer ***/
        Edge(Node n1, Node n2) : firstNode(n1), secondNode(n2) {}

        /*-------------------------------------------------------------------
          Construct an Edge between two nodes.
          
          Precondition:  n1 and n2 are valid Nodes.
          Postcondition: An Edge connecting n1 and n2 is created.
         ------------------------------------------------------------------*/

        /*** Getters **/
        const Node& getFirstNode() const { return firstNode; }
        /*-------------------------------------------------------------------
          Get the first node of this edge.
          
          Postcondition: Reference to the first Node is returned.
         ------------------------------------------------------------------*/

        const Node& getSecondNode() const { return secondNode; }
        /*-------------------------------------------------------------------
          Get the second node of this edge.
          
          Postcondition: Reference to the second Node is returned.
         ------------------------------------------------------------------*/

        bool connects(const Node& a, const Node& b) const {
            return (firstNode == a && secondNode == b) ||
                (firstNode == b && secondNode == a);
        }
        /*-------------------------------------------------------------------
          Check if this edge connects two given nodes.
          
          Precondition:  a and b are Nodes to check.
          Postcondition: Returns true if edge connects a and b (order doesn't
                         matter), false otherwise.
         ------------------------------------------------------------------*/
    };

    /***** Social Graph Operations *****/
    void addPerson(const string& name);
    /*-----------------------------------------------------------------------
      Add a new person to the social network.
      
      Precondition:  name is the unique identifier for the person.
      Postcondition: If not already present, a new node with given name is 
                     added to the graph.
     ----------------------------------------------------------------------*/

    bool removePerson(const string& name);
    /*-----------------------------------------------------------------------
      Remove a person from the social network.
      
      Precondition:  name identifies the person to remove.
      Postcondition: The node and all its connections are removed if found.
                     Returns true if successful, false otherwise.
     ----------------------------------------------------------------------*/

    void addFriend(const string& name1, const string& name2);
    /*-----------------------------------------------------------------------
      Create a friendship between two people.
      
      Precondition:  name1 and name2 are valid names in the graph.
      Postcondition: If both exist and not already friends, an edge is created
                     between them.
     ----------------------------------------------------------------------*/

    void removeFriend(const string& name1, const string& name2);
    /*-----------------------------------------------------------------------
      Remove friendship between two people.
      
      Precondition:  name1 and name2 are valid names in the graph.
      Postcondition: Any edge between them is removed if found.
     ----------------------------------------------------------------------*/

    bool areConnected(const string& name1, const string& name2) const;
    /*-----------------------------------------------------------------------
      Check if two people are friends.
      
      Precondition:  name1 and name2 are valid names in the graph.
      Postcondition: Returns true if an edge exists between them, false otherwise.
     ----------------------------------------------------------------------*/

    vector<string> recommendFriends(const string& name, int k) const;
    /*-----------------------------------------------------------------------
      Recommend friends based on mutual connections.
      
      Precondition:  name is a valid name in the graph; k is number of recommendations.
      Postcondition: Returns vector of top k recommended friend names sorted by
                     number of mutual friends.
     ----------------------------------------------------------------------*/

    vector<string> shortestPath(const string& from, const string& to) const;
    /*-----------------------------------------------------------------------
      Find shortest path between two people.
      
      Precondition:  from and to are valid names in the graph.
      Postcondition: Returns vector of names representing the shortest path,
                     empty if no path exists.
     ----------------------------------------------------------------------*/

    vector<string> shortestPathAvoiding(const string& from, const string& to,
                                      const vector<string>& blacklist) const;
    /*-----------------------------------------------------------------------
      Find shortest path while avoiding certain people.
      
      Precondition:  from and to are valid names; blacklist contains names to avoid.
      Postcondition: Returns shortest path that doesn't contain blacklisted names,
                     empty if no valid path exists.
     ----------------------------------------------------------------------*/

    vector<Node> getFriends(const Node& node) const;
    /*-----------------------------------------------------------------------
      Get all friends of a given person.
      
      Precondition:  node is a valid Node in the graph.
      Postcondition: Returns vector of Nodes that are friends with given node.
     ----------------------------------------------------------------------*/

    vector<Node> getNodes() const { return nodes; }
    /*-----------------------------------------------------------------------
      Get all people in the network.
      
      Postcondition: Returns vector of all Nodes in the graph.
     ----------------------------------------------------------------------*/

    vector<Edge> getEdgeList() const { return edgeList; }
    /*-----------------------------------------------------------------------
      Get all friendships in the network.
      
      Postcondition: Returns vector of all Edges in the graph.
     ----------------------------------------------------------------------*/

    bool loadFromFile(const string& edgeListFile);
    /*-----------------------------------------------------------------------
      Load network from a file.
      
      Precondition:  edgeListFile is the name of a valid file in format:
                     "A: B C D" (one line per person with friends list)
      Postcondition: Graph is populated with data from file. Returns true if
                     successful, false otherwise.
     ----------------------------------------------------------------------*/

    bool saveToFile(const string& edgeListFile) const;
    /*-----------------------------------------------------------------------
      Save network to a file.
      
      Precondition:  edgeListFile is the name of file to create.
      Postcondition: Graph is saved in same format as loadFromFile expects.
                     Returns true if successful, false otherwise.
     ----------------------------------------------------------------------*/

private:
    /***** Data Members *****/
    vector<Node> nodes;      // All people in the network
    vector<Edge> edgeList;   // All friendships in the network

    /***** Helper Functions *****/
    bool nodeExists(const Node& node) const;
    /*-----------------------------------------------------------------------
      Check if a node exists in the graph.
      
      Precondition:  node is the Node to check.
      Postcondition: Returns true if node exists in the graph, false otherwise.
     ----------------------------------------------------------------------*/

    int countMutualFriends(const Node& a, const Node& b) const;
    /*-----------------------------------------------------------------------
      Count mutual friends between two people.
      
      Precondition:  a and b are valid Nodes in the graph.
      Postcondition: Returns number of nodes connected to both a and b.
     ----------------------------------------------------------------------*/
};

#endif