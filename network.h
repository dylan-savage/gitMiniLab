#ifndef NETWORK_H
#define NETWORK_H
#include "user.h"
#include <vector>


class Network{
    public:


    Network(); // Default constructor
    ~Network(); // Destructor

    //pre: A file is given with users and information
    //post: The network is intitialized with the contents of the file
    int readUsers(char* filename); // Read users from a file

    //pre: Network is populated with Users
    //post: Network's information is written to a file
    int writeUsers(const char* filename); 

    //pre: none
    //post: User is added to the network
    void addUser(User* user);

    //pre: none
    //post: Two users are now connected
    int addConnection(const std::string& userName1, const std::string& userName2);

    //pre: two users have a connection
    //post: The connection is removed from the network
    int removeConnection(const std::string& userName1, const std::string& userName2); 

    //pre: none
    //post: ID of user is returned
    int getId(const std::string& userName) const; 

    //pre: none
    //post: user with specified ID is returned
    User* getUser(int id); 

    //pre: none
    //post: number of users in network is returned
    int numUsers() const; 


    //HW2

    //pre: a populated network
    //post: returns the shortest path from one user to another
    std::vector<std::size_t> shortestPath(std::size_t from, std::size_t to);

    //pre: a populated network
    //post: returns a vector of the path to a found user, and sets to variable as the ID of the found user
    std::vector<std::size_t> distanceUser(std::size_t from, std::size_t& to, std::size_t distance);

    //Pre: A populated network
    //post: returns users with most amount of friends of friends of inputed User
    std::vector<std::size_t> suggestFriends(std::size_t who, std::size_t& score);

    private:
    std::vector<User*> network;

};


#endif