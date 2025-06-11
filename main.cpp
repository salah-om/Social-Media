/******************************************************************************

    Implementation of main.cpp:

    displayMenu: Displays the social network menu options.
    addPerson: Adds a person to the social graph.
    addFriendship: Adds a connection between two people.
    removeFriendship: Removes a connection between two people.
    removePerson: Removes a person from the social graph.
    checkConnection: Checks if two people are connected.
    getRecommendations: Gets friend recommendations for a person.
    findShortestPath: Finds shortest path between two people.
    findShortestPathAvoiding: Finds path while avoiding certain people.
    loadFromFile: Loads network data from a file.
    saveToFile: Saves network data to a file.
    displayAllPeople: Displays all people in the network.
    displayAllFriendships: Displays all friendships in the network.
    main: Main function that runs the social network program.

******************************************************************************/
#include "SocialGraph.h"
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

/*-------------------------------------------------------------------
  Display the main menu options for the social network application.

  Precondition:  None
  Postcondition: The menu options are displayed to the console.
-------------------------------------------------------------------*/
void displayMenu() {
    cout << "--------------------------";
    cout << "\nSocial Network Menu\n";
    cout << "--------------------------\n";
    cout << "1. Add a person\n";
    cout << "2. Add connection between two people\n";
    cout << "3. Remove connection between two people\n";
    cout << "4. Remove a person from the network\n";
    cout << "5. Check if two people are friends\n";
    cout << "6. Get friend recommendations for a person\n";
    cout << "7. Find shortest path between two people\n";
    cout << "8. Find shortest path avoiding certain people\n";
    cout << "9. Display all people in network\n";
    cout << "10. Display all friendships\n";
    cout << "11. Load network from file\n";
    cout << "12. Save network to a .txt file\n";
    cout << "0. Exit\n";
    cout << "---------------------------\n";
    cout << "Enter your choice: \n";
    cout << "---------------------------\n";
}

/*-------------------------------------------------------------------
  Add a new person to the social network.

  Precondition:  graph is a valid SocialGraph object.
  Postcondition: If successful, the person is added to the network
                 and a confirmation message is displayed.
-------------------------------------------------------------------*/
void addPerson(SocialGraph& graph) {
    string name;
    cout << "Enter person's name: ";
    cin >> name;
    graph.addPerson(name);
    cout << "Person '" << name << "' added to the network.\n";
}

/*-------------------------------------------------------------------
  Add a friendship connection between two people in the network.

  Precondition:  graph is a valid SocialGraph object.
                 name1 and name2 are valid names in the network.
  Postcondition: If successful, a friendship is established between
                 the two people and a confirmation message is displayed.
-------------------------------------------------------------------*/
void addFriendship(SocialGraph& graph) {
    string name1, name2;
    cout << "Enter first person's name: ";
    cin >> name1;
    cout << "Enter second person's name: ";
    cin >> name2;
    graph.addFriend(name1, name2);
    cout << "Friendship added between '" << name1 << "' and '" << name2 << "'.\n";
}

/*-------------------------------------------------------------------
  Remove a friendship connection between two people.

  Precondition:  graph is a valid SocialGraph object.
                 name1 and name2 are valid names in the network.
  Postcondition: If successful, the friendship is removed and
                 a confirmation message is displayed.
-------------------------------------------------------------------*/
void removeFriendship(SocialGraph& graph) {
    string name1, name2;
    cout << "Enter first person's name: ";
    cin >> name1;
    cout << "Enter second person's name: ";
    cin >> name2;
    graph.removeFriend(name1, name2);
    cout << "Friendship removed between '" << name1 << "' and '" << name2 << "'.\n";
}

/*-------------------------------------------------------------------
  Remove a person from the social network.

  Precondition:  graph is a valid SocialGraph object.
                 name is a valid name in the network.
  Postcondition: If successful, the person and all their connections
                 are removed from the network and a confirmation
                 message is displayed.
-------------------------------------------------------------------*/
void removePerson(SocialGraph& graph) {
    string name;
    cout << "Enter person's name to remove: ";
    cin >> name;
    if (graph.removePerson(name)) {
        cout << "Person '" << name << "' removed from the network.\n";
    }
    else {
        cout << "Person '" << name << "' not found in the network.\n";
    }
}

/*-------------------------------------------------------------------
  Check if two people in the network are friends.

  Precondition:  graph is a valid SocialGraph object.
                 name1 and name2 are valid names in the network.
  Postcondition: Displays whether the two people are connected
                 as friends.
-------------------------------------------------------------------*/
void checkConnection(SocialGraph& graph) {
    string name1, name2;
    cout << "Enter first person's name: ";
    cin >> name1;
    cout << "Enter second person's name: ";
    cin >> name2;
    if (graph.areConnected(name1, name2)) {
        cout << "'" << name1 << "' and '" << name2 << "' are friends.\n";
    }
    else {
        cout << "'" << name1 << "' and '" << name2 << "' are not friends.\n";
    }
}

/*-------------------------------------------------------------------
  Get friend recommendations for a person in the network.

  Precondition:  graph is a valid SocialGraph object.
                 name is a valid name in the network.
                 k is a positive integer for number of recommendations.
  Postcondition: Displays up to k recommended friends based on
                 network connections.
-------------------------------------------------------------------*/
void getRecommendations(SocialGraph& graph) {
    string name;
    int k;
    cout << "Enter person's name: ";
    cin >> name;
    cout << "Enter number of recommendations: ";
    cin >> k;
    vector<string> recs = graph.recommendFriends(name, k);
    if (recs.empty()) {
        cout << "No recommendations available for '" << name << "'.\n";
    }
    else {
        cout << "Recommended friends for '" << name << "':\n";
        for (const string& rec : recs) {
            cout << "- " << rec << endl;
        }
    }
}

/*-------------------------------------------------------------------
  Find the shortest path between two people in the network.

  Precondition:  graph is a valid SocialGraph object.
                 from and to are valid names in the network.
  Postcondition: Displays the shortest path between the two people
                 or a message if no path exists.
-------------------------------------------------------------------*/
void findShortestPath(SocialGraph& graph) {
    string from, to;
    cout << "Enter starting person's name: ";
    cin >> from;
    cout << "Enter destination person's name: ";
    cin >> to;
    vector<string> path = graph.shortestPath(from, to);
    if (path.empty()) {
        cout << "No path exists between '" << from << "' and '" << to << "'.\n";
    }
    else {
        cout << "Shortest path: ";
        for (int i = 0; i < path.size(); i++) {
            if (i != 0) cout << " -> ";
            cout << path[i];
        }
        cout << endl;
    }
}

/*-------------------------------------------------------------------
  Find the shortest path between two people while avoiding certain people.

  Precondition:  graph is a valid SocialGraph object.
                 from and to are valid names in the network.
                 blacklist contains names to avoid in the path.
  Postcondition: Displays the shortest path that avoids the specified
                 people or a message if no valid path exists.
-------------------------------------------------------------------*/
void findShortestPathAvoiding(SocialGraph& graph) {
    string from, to, blacklisted;
    vector<string> blacklist;
    cout << "Enter starting person's name: ";
    cin >> from;
    cout << "Enter destination person's name: ";
    cin >> to;

    cout << "Enter names to avoid (separated by spaces, end with 'done'): ";
    while (cin >> blacklisted && blacklisted != "done") {
        blacklist.push_back(blacklisted);
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    vector<string> path = graph.shortestPathAvoiding(from, to, blacklist);
    if (path.empty()) {
        cout << "No valid path exists that avoids the specified people.\n";
    }
    else {
        cout << "Shortest path avoiding specified people: ";
        for (int i = 0; i < path.size(); i++) {
            if (i != 0) cout << " -> ";
            cout << path[i];
        }
        cout << endl;
    }
}

/*-------------------------------------------------------------------
  Load a social network from a file.

  Precondition:  graph is a valid SocialGraph object.
  Postcondition: If successful, the network is loaded from either
                 EdgeList.txt in the current directory or x64/Debug.
                 Displays appropriate status messages.
-------------------------------------------------------------------*/
void loadFromFile(SocialGraph& graph) {
    string filename = "EdgeList.txt";

    if (graph.loadFromFile(filename)) {
        cout << "Network loaded successfully from " << filename << endl;
        return;
    }

    string debugPath = "x64/Debug/" + filename;
    if (graph.loadFromFile(debugPath)) {
        cout << "Network loaded successfully from " << debugPath << endl;
        return;
    }

    cerr << "Failed to load network. Please ensure EdgeList.txt exists in either:\n"
        << "1. The same directory as your executable\n"
        << "2. The x64/Debug directory\n";
}

/*-------------------------------------------------------------------
  Save the current social network to a text file.

  Precondition:  graph is a valid SocialGraph object.
  Postcondition: If successful, the network is saved to the specified
                 text file with .txt extension. Displays status message.
-------------------------------------------------------------------*/
void saveToFile(SocialGraph& graph) {
    string filename;
    cout << "\nEnter the name for the output text file (e.g., 'network.txt'): ";
    cin >> filename;

    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
        filename += ".txt";
    }

    if (graph.saveToFile(filename)) {
        cout << "Network successfully saved to " << filename << endl;
    }
    else {
        cerr << "Failed to save network to file." << endl;
    }
}

/*-------------------------------------------------------------------
  Display all people currently in the social network.

  Precondition:  graph is a valid SocialGraph object.
  Postcondition: Lists all people in the network to the console.
-------------------------------------------------------------------*/
void displayAllPeople(const SocialGraph& graph) {
    cout << "\nPeople in the network:\n";
    vector<SocialGraph::Node> people = graph.getNodes();
    if (people.empty()) {
        cout << "No people in the network.\n";
    }
    else {
        for (int i = 0; i < people.size(); i++) {
            cout << "- " << people[i].getName() << endl;
        }
    }
}

/*-------------------------------------------------------------------
  Display all friendship connections in the network.

  Precondition:  graph is a valid SocialGraph object.
  Postcondition: Lists all friendships in the network, showing each
                 person's connections.
-------------------------------------------------------------------*/
void displayAllFriendships(const SocialGraph& graph) {
    cout << "\nFriendships in the network:\n";
    vector<SocialGraph::Node> nodes = graph.getNodes();
    bool hasFriendships = false;

    for (int i = 0; i < nodes.size(); i++) {
        SocialGraph::Node current = nodes[i];
        vector<SocialGraph::Node> friends = graph.getFriends(current);

        if (!friends.empty()) {
            hasFriendships = true;
        }

        cout << current.getName() << ": ";
        for (int j = 0; j < friends.size(); j++) {
            cout << friends[j].getName();
            if (j != friends.size() - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }

    if (!hasFriendships) {
        cout << "No friendships in the network.\n";
    }
}

int main() {
    SocialGraph graph;
    int choice;

    do {
        displayMenu();
        cin >> choice;

        try {
            switch (choice) {
                // Add user
            case 1: addPerson(graph); break;
                // Add connection
            case 2: addFriendship(graph); break;
                // Remove connection
            case 3: removeFriendship(graph); break;
                // Remove user
            case 4: removePerson(graph); break;
                // Check connection
            case 5: checkConnection(graph); break;
                // Get Recommendations for friends
            case 6: getRecommendations(graph); break;
                // Find shortest path between 2 users
            case 7: findShortestPath(graph); break;
                // Find shortest path avoiding specific nodes
            case 8: findShortestPathAvoiding(graph); break;
                // Displays all nodes in network
            case 9: displayAllPeople(graph); break;
                // Displays edgelist of friendships
            case 10: displayAllFriendships(graph); break;
                // Loads network 
            case 11: loadFromFile(graph); break;
                // Saves network to .txt file
            case 12: saveToFile(graph); break;
            case 0: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Please try again.\n";
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    } while (choice != 0);

    return 0;
}