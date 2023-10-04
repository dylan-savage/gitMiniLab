#ifndef USER_H
#define USER_H

#include <iostream>
#include <vector>

class User {
    public:
   
    // pre: none
    // post: an empty user has been created
    User(); 

    //pre: none
    //post: a user has been created
    User(std::size_t Id, std::string Name,std::size_t BirthYear, std::size_t ZipCode, std::vector<std::size_t> friends);

    //pre: none
    //post: friend ID is added to current user's friend list
    void addFriend(std::size_t id); 

    //pre: friend's ID is apart of this user's list
    //post: friend ID is deleted from current user
    void deleteFriend(std::size_t id); 
  
    //pre: none 
    //post: user ID is returned
    const std::size_t getId() {return id;}

    //pre: none 
    //post: user name is returned
    const std::string getName() {return name;}

    //pre: none 
    //post: user birthYear is returned
    const std::size_t getYear() {return birthYear;}

    //pre: none 
    //post: user zipCode is returned
    const std::size_t getZip() {return zipCode;}

    //pre: none
    //post: user Friends are returned
    const std::vector<std::size_t>& getFriends() const {return friends;}

    //pre: none
    //post: returns whether or not potentialFriend is friends with current User object
    bool isFriend(std::size_t potentialFriend);

    private:

    std::size_t id;
    std::string name;
    std::size_t birthYear;
    std::size_t zipCode;
    std::vector<std::size_t> friends;
};

#endif