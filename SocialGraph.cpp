/*-------------------------------------------------------------------------
  SocialGraph.cpp

  - Implementation of all functions mentioned in .h file
------------------------------------------------------------------------*/
#include "SocialGraph.h"
#include <algorithm>
#include <queue>
#include <vector>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;
/*-----------------------------------------------------------------------
    Add a new person to the social network.

    Precondition:  name is the unique identifier for the person.
    Postcondition: A new node with the given name is added to the graph.
-----------------------------------------------------------------------*/
void SocialGraph::addPerson(const string& name) {
    Node newNode(name);
    if (!nodeExists(newNode)) {
        nodes.push_back(newNode);
    }
}

/*-----------------------------------------------------------------------
    Remove a person from the social network.

    Precondition:  name is the unique identifier for the person to remove.
    Postcondition: The node and all its connections are removed from the graph.
                  Returns true if removal was successful, false otherwise.
-----------------------------------------------------------------------*/
bool SocialGraph::removePerson(const string& name) {
    Node nodeToRemove(name);
    if (!nodeExists(nodeToRemove)) {
        return false;
    }

    // Remove all edges connected to this node by filtering edges where the node appears
    edgeList.erase(
        remove_if(edgeList.begin(), edgeList.end(),
            [&nodeToRemove](const Edge& edge) {
                return edge.getFirstNode() == nodeToRemove || edge.getSecondNode() == nodeToRemove;
            }),
        edgeList.end()
    );

    // Remove the node
    nodes.erase(
        remove(nodes.begin(), nodes.end(), nodeToRemove),
        nodes.end()
    );

    return true;
}

/*-----------------------------------------------------------------------
    Make two people friends.

    Precondition:  name1 and name2 are valid names of people in the graph.
    Postcondition: An edge is created between the two nodes if they exist.
-----------------------------------------------------------------------*/
void SocialGraph::addFriend(const string& name1, const string& name2) {
    Node node1(name1), node2(name2);

    // Check both exist and aren't the same node
    if (nodeExists(node1) && nodeExists(node2) && name1 != name2) {
        // Check if friendship already exists
        bool alreadyFriends = false;
        for (const Edge& edge : edgeList) {
            if (edge.connects(node1, node2)) {
                alreadyFriends = true;
                break;
            }
        }

        if (!alreadyFriends) {
            // Add new edge
            edgeList.push_back(Edge(node1, node2));
        }
    }
}

/*-----------------------------------------------------------------------
    Remove friendship between two people.

    Precondition:  name1 and name2 are valid names of people in the graph.
    Postcondition: The edge between the two nodes is removed if it exists.
-----------------------------------------------------------------------*/
void SocialGraph::removeFriend(const string& name1, const string& name2) {
    Node node1(name1), node2(name2);
    // Removes the edge connecting n1 and n2 by filtering
    edgeList.erase(
        remove_if(edgeList.begin(), edgeList.end(),
            [&node1, &node2](const Edge& edge) {
                return edge.connects(node1, node2);
            }),
        edgeList.end()
    );
}


/*-----------------------------------------------------------------------
    Count mutual friends between two nodes.

    Precondition:  Both a and b are valid nodes in the graph.
    Postcondition: Returns the number of mutual friends between a and b.
-----------------------------------------------------------------------*/
int SocialGraph::countMutualFriends(const Node& a, const Node& b) const {
    // Using Helper Function
    vector<Node> aFriends = getFriends(a);
    vector<Node> bFriends = getFriends(b);

    int count = 0;
    // Nested loop to compare all friends
    for (const Node& aFriend : aFriends) {
        for (const Node& bFriend : bFriends) {
            if (aFriend == bFriend) {
                count++;
                break;
            }
        }
    }
    return count;
}

/*-----------------------------------------------------------------------
    Check if two people are friends.

    Precondition:  name1 and name2 are valid names of people in the graph.
    Postcondition: Returns true if the two nodes are connected, false otherwise.
-----------------------------------------------------------------------*/
bool SocialGraph::areConnected(const string& name1, const string& name2) const {
    Node node1(name1), node2(name2);

    // for each edge in edgelist, check if these nodes are connected
    for (const Edge& edge : edgeList) {
        if (edge.connects(node1, node2)) {
            return true;
        }
    }
    return false;
}

/*------------------------------------------------------------------------------------------
    Recommend friends for a person based on mutual friends.

    Precondition:  name is a valid name in the graph, k is the number of recommendations.
    Postcondition: Returns a vector of names of top k recommended friends.
-------------------------------------------------------------------------------------------*/
vector<string> SocialGraph::recommendFriends(const string& name, int k) const {
    vector<string> recommendations;
    Node source(name);
    if (!nodeExists(source)) return recommendations;

    vector<Node> currentFriends = getFriends(source);
    vector<pair<Node, int>> potentialFriends;

    // Find all non-friends with mutual friend counts
    for (const Node& node : nodes) {
        if (node == source) continue;

        // Checks if 'node' is already a friend
        bool isAlreadyFriend = false;
        for (const Node& friendNode : currentFriends) {
            if (node == friendNode) {
                isAlreadyFriend = true;
                break;
            }
        }
        // If 'node' is not a friend, calculate mutual friend count
        if (!isAlreadyFriend) {
            int mutualCount = countMutualFriends(source, node);
            if (mutualCount > 0) {
                potentialFriends.emplace_back(node, mutualCount);
            }
        }
    }

    // Sort by mutual friend count (descending)
    sort(potentialFriends.begin(), potentialFriends.end(),
        [](const pair<Node, int>& a, const pair<Node, int>& b) {
            return a.second > b.second;
        });

    // incase nb of recomm asked is not enough for potential friends
    int limit = (k < potentialFriends.size()) ? k : potentialFriends.size();
    // Get top k recommendations
    for (int i = 0; i < limit; i++) {
        recommendations.push_back(potentialFriends[i].first.getName());
    }

    return recommendations;
}

/*-----------------------------------------------------------------------
    Find the shortest path between two people using BFS.

    Precondition:  from and to are valid names of people in the graph.
    Postcondition: Returns a vector of names representing the shortest path.
-----------------------------------------------------------------------*/
vector<string> SocialGraph::shortestPath(const string& from, const string& to) const {
    vector<string> path;
    Node start(from), end(to);
    if (!nodeExists(start) || !nodeExists(end)) return path;

    // Find indices of start and end nodes
    int start_index = -1, end_index = -1;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i] == start) start_index = i;
        if (nodes[i] == end) end_index = i;
        if (start_index != -1 && end_index != -1) break;
    }

    // Initialize parent and distance vectors
    vector<int> parent(nodes.size(), -1);
    vector<int> distance(nodes.size(), INT_MAX);
    parent[start_index] = -1; // -1 will mark the start node
    distance[start_index] = 0;

    queue<int> q;
    q.push(start_index);

    bool found = false;
    while (!q.empty() && !found) {
        int current = q.front();
        q.pop();

        if (current == end_index) {
            found = true;
            break;
        }

        vector<Node> neighbors = getFriends(nodes[current]);
        for (int i = 0; i < neighbors.size(); i++) {
            // Find index of neighbor
            int neighbor_index = -1;
            for (int j = 0; j < nodes.size(); j++) {
                if (nodes[j] == neighbors[i]) {
                    neighbor_index = j;
                    break;
                }
            }

            if (distance[neighbor_index] == INT_MAX) {
                distance[neighbor_index] = distance[current] + 1;
                parent[neighbor_index] = current;
                q.push(neighbor_index);
            }
        }
    }

    // Reconstruct path if found
    if (found) {
        vector<int> index_path;
        for (int v = end_index; v != -1; v = parent[v]) {
            index_path.push_back(v);
        }
        reverse(index_path.begin(), index_path.end());

        for (int i = 0; i < index_path.size(); i++) {
            path.push_back(nodes[index_path[i]].getName());
        }
    }

    return path;
}

/*--------------------------------------------------------------------------------------------------------
    Find the shortest path between two people while avoiding specific nodes using BFS.

    Precondition:  from and to are valid names, blacklist contains nodes to avoid.
    Postcondition: Returns a vector of names representing the shortest path avoiding blacklisted nodes.
----------------------------------------------------------------------------------------------------------*/
vector<string> SocialGraph::shortestPathAvoiding(const string& from, const string& to, const vector<string>& blacklist) const {
    vector<string> path;
    Node start(from), end(to);
    if (!nodeExists(start) || !nodeExists(end)) return path;

    // Find indices of start and end nodes
    int start_index = -1, end_index = -1;
    for (int i = 0; i < nodes.size(); i++) {
        if (nodes[i] == start) start_index = i;
        if (nodes[i] == end) end_index = i;
        if (start_index != -1 && end_index != -1) break;
    }

    // Convert blacklist to node indices
    vector<int> blacklistIndices;
    for (const string& name : blacklist) {
        Node blackNode(name);
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i] == blackNode) {
                blacklistIndices.push_back(i);
                break;
            }
        }
    }

    // Initialize parent and visited vectors
    vector<int> parent(nodes.size(), -1);
    vector<bool> visited(nodes.size(), false);
    parent[start_index] = -1; // -1 marks the start node
    visited[start_index] = true;

    queue<int> q;
    q.push(start_index);

    bool found = false;
    while (!q.empty() && !found) {
        int current = q.front();
        q.pop();

        if (current == end_index) {
            found = true;
            break;
        }

        vector<Node> neighbors = getFriends(nodes[current]);
        for (int i = 0; i < neighbors.size(); i++) {
            // Find index of neighbor
            int neighbor_index = -1;
            for (int j = 0; j < nodes.size(); j++) {
                if (nodes[j] == neighbors[i]) {
                    neighbor_index = j;
                    break;
                }
            }

            // Skip if blacklisted or already visited
            bool isBlacklisted = false;
            for (int blackIndex : blacklistIndices) {
                if (neighbor_index == blackIndex) {
                    isBlacklisted = true;
                    break;
                }
            }

            if (!isBlacklisted && !visited[neighbor_index]) {
                // Mark neighbor as visited
                visited[neighbor_index] = true;
                // Store current node as parent
                parent[neighbor_index] = current;
                // Add neighbor to queue
                q.push(neighbor_index);
            }
        }
    }

    // Reconstruct path if found
    if (found) {
        vector<int> index_path;
        for (int v = end_index; v != -1; v = parent[v]) {
            index_path.push_back(v);
        }
        reverse(index_path.begin(), index_path.end());

        for (int i = 0; i < index_path.size(); i++) {
            path.push_back(nodes[index_path[i]].getName());
        }
    }

    return path;
}

/*-----------------------------------------------------------------------
    Check if a node exists in the graph.

    Precondition:  node is the node to check.
    Postcondition: Returns true if node exists in the graph, false otherwise.
-----------------------------------------------------------------------*/
bool SocialGraph::nodeExists(const Node& node) const {
    return find(nodes.begin(), nodes.end(), node) != nodes.end();
}

/*-----------------------------------------------------------------------
    Get all friends of a given node.

    Precondition:  node is a valid node in the graph.
    Postcondition: Returns a vector of nodes that are friends with the given node.
-----------------------------------------------------------------------*/
vector<SocialGraph::Node> SocialGraph::getFriends(const Node& node) const {
    vector<Node> friends;
    for (const Edge& edge : edgeList) {
        if (edge.getFirstNode() == node) {
            friends.push_back(edge.getSecondNode());
        }
        else if (edge.getSecondNode() == node) {
            friends.push_back(edge.getFirstNode());
        }
    }
    return friends;
}



/*-----------------------------------------------------------------------
    Load graph data from a file.

    Precondition:  edgeListFile is the name of a valid file.
    Postcondition: Graph is populated with data from the file.
                  Returns true if successful, false otherwise.
-----------------------------------------------------------------------*/
bool SocialGraph::loadFromFile(const string& edgeListFile) {
    ifstream inFile(edgeListFile);
    if (!inFile) {
        cerr << "Error: Could not open file: " << edgeListFile << endl;
        return false;
    }

    // Clear existing graph data before loading new data
    nodes.clear();
    edgeList.clear();

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        // Format: "source: friend1 friend2 friend3"
        int colonPos = line.find(':');

        // Extract and trim source name (handles leading/trailing whitespace)
        string sourceName = line.substr(0, colonPos);
        sourceName.erase(0, sourceName.find_first_not_of(" \t"));
        sourceName.erase(sourceName.find_last_not_of(" \t") + 1);

        addPerson(sourceName);

        // Process all friends listed after the colon
        istringstream iss(line.substr(colonPos + 1));
        string neighbor;
        while (iss >> neighbor) {
            addPerson(neighbor);
            addFriend(sourceName, neighbor);  // Create undirected connection
        }
    }

    return true;
}


/*-----------------------------------------------------------------------
    Save graph data to a file.

    Precondition:  edgeListFile is the name of the file to create/overwrite.
    Postcondition: Graph data is written to the file.
                  Returns true if successful, false otherwise.
-----------------------------------------------------------------------*/
bool SocialGraph::saveToFile(const string& edgeListFile) const {
    ofstream outFile(edgeListFile);
    if (!outFile) {
        cerr << "Error: Could not open file for writing: " << edgeListFile << endl;
        return false;
    }

    // Save in format "source: n1 n2 n3"
    vector<Node> nodes = getNodes();
    for (const Node& current : nodes) {
        outFile << current.getName() << ": ";

        // Loop through each friend to write their name to file
        vector<Node> friends = getFriends(current);
        for (int j = 0; j < friends.size(); j++) {
            outFile << friends[j].getName();
            if (j != friends.size() - 1) {
                // Space between names, no trailing space
                outFile << " ";  
            }
        }
        outFile << endl;
    }

    return true;
}