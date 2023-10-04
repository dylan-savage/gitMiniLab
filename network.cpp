#include "network.h"
#include "user.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>

Network::Network() {
}

Network::~Network() {
    for (User* user : network) {
        delete user;
    }
    network.clear();
}

void Network::addUser(User* user) {
    network.push_back(user);
}

int Network::addConnection(const std::string& userName1, const std::string& userName2) {
    int id1 = getId(userName1);
    int id2 = getId(userName2);

    if (id1 == -1 || id2 == -1 ||id1 == id2) {
        return -1; 
    }

    network[id1]->addFriend(id2);
    network[id2]->addFriend(id1);

    return 0; 
}

int Network::removeConnection(const std::string& userName1, const std::string& userName2) {
    int id1 = getId(userName1);
    int id2 = getId(userName2);

    if (id1 == -1 || id2 == -1 || id1 == id2) {
        return -1; 
    }

    network[id1]->deleteFriend(id2);
    network[id2]->deleteFriend(id1);

    return 0; 
}

int Network::getId(const std::string& userName) const {
    for (std::size_t i = 0; i < network.size(); ++i) {
        if (network[i]->getName() == userName) {
            return static_cast<int>(i); 
        }
    }
    return -1; 
}

int Network::numUsers() const {
    return static_cast<int>(network.size());
}

User* Network::getUser(int id) {
    if (id >= 0 && static_cast<std::size_t>(id) < network.size()) {
        return network[id];
    }
    return nullptr; 
}

int Network::readUsers(char* fileName){
	std::ifstream toReadFile(fileName);
	if (!toReadFile.is_open()) {
        std::cout << "error opening file" << std::endl;
        return -1;
    }
    std::size_t currentID = 0;
	std::string currentName = "";
	std::size_t currentYear = 0;
	std::size_t currentZip = 0;
	std::string currentFriendString = "";

	int line = 0;

	std::string tempString;
    

	std::getline(toReadFile,tempString); 

	while(std::getline(toReadFile,tempString)){
		if(line == 0){
			currentID = stoi(tempString); 
			++line;
		}
		else if(line == 1){ 
			tempString.erase(0, tempString.find_first_not_of('\t')); 
			currentName = tempString;
			++line;
		}
		else if(line == 2){  
			currentYear = stoi(tempString);
			++line;
		}
		else if(line == 3){ 
			currentZip = stoi(tempString);
			++line;
		}
		else if(line == 4){ 
			tempString.erase(0, tempString.find_first_not_of('\t'));
			currentFriendString = tempString;

			std::istringstream friendStream(currentFriendString);
			size_t currentFriendID;

            User * currentUser = new User(currentID,currentName,currentYear,currentZip,{});

			while (friendStream >> currentFriendID) {
			    currentUser->addFriend(currentFriendID);
			}

			addUser(currentUser);
			line = 0;
		}
	}
	toReadFile.close();
	return 0;
}

int Network::writeUsers(const char* filename) {
    std::ofstream outputFile(filename);
    if (!outputFile) {
        return -1; 
    }

    outputFile << numUsers() << std::endl;

    for (User* user : network) {
        outputFile << user->getId() << std::endl;
        outputFile << "\t" << user->getName() << std::endl;
        outputFile << "\t" << user->getYear() << std::endl;
        outputFile << "\t" << user->getZip() << std::endl;
        outputFile << "\t";

        const std::vector<std::size_t>& friends = user->getFriends();
        for (std::size_t friendId : friends) {
            outputFile << friendId << " ";
        }
        outputFile << std::endl;
    }

    outputFile.close();
    return 0; 
}
    




//HomeWork2

std::vector<std::size_t> Network::shortestPath(std::size_t from, std::size_t to){

    std::vector<bool> visited(network.size(), false);
    std::vector<std::size_t> path;
    std::queue<std::size_t> queue;
    std::vector<std::size_t> previous(network.size(), UINT_MAX);

    // Mark 'from' as visited and enqueue it
    visited[from] = true;
    queue.push(from);
        
    while (!queue.empty()) {
        std::size_t current = queue.front();
        queue.pop();
            
        // If we have reached the 'to' user, construct the path and return it
        if (current == to) {
            while (current != UINT_MAX) {
                path.insert(path.begin(), network[current]->getId());
                current = previous[current];
            }
            return path;
        }
        
        // Visit all adjacent users
        for (std::size_t neighbor : network[current]->getFriends()) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push(neighbor);
                previous[neighbor] = current;
            }
        }
    }
    return path; // Empty path
}

    


//ungpt!!!

std::vector<std::size_t> Network::distanceUser(std::size_t from, std::size_t& to, std::size_t distance) {
    // Create a vector to track visited users and initialize it as false
    std::vector<bool> visited(network.size(), false);

    // Create a queue for BFS
    std::queue<std::size_t> queue;

    // Create a vector to store the path
    std::vector<std::size_t> path;

    // Find the index of the 'from' user in the network
    std::size_t fromIndex = UINT_MAX;

    for (std::size_t i = 0; i < network.size(); ++i) {
        if (network[i]->getId() == from) {
            fromIndex = i;
            visited[fromIndex] = true;
            queue.push(fromIndex);
            path.push_back(from);
            break;
        }
    }

    // If 'from' is not found in the network, return an empty path and set 'to' to -1
    if (fromIndex == UINT_MAX) {
        to = -1; // User not found
        return path; // Empty path
    }

    while (!queue.empty() && distance > 0) {
        std::size_t current = queue.front();
        queue.pop();

        for (std::size_t neighbor : network[current]->getFriends()) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue.push(neighbor);
                path.push_back(network[neighbor]->getId());
            }
        }

        // Decrement the distance for each level of traversal
        distance--;
    }

    // If 'distance' is greater than 0, no user at the specified distance was found
    if (distance > 0) {
        to = -1; // User not found
        path.clear(); // Empty path
    } else {
        // 'to' is the last user added to the path
        to = path.back();
    }

    return path;
}




// std::vector<std::size_t> Network::suggestFriends(std::size_t who, std::size_t& score){
//     // Initialize variables to keep track of suggested friends and the highest score
//     std::vector<std::size_t> suggestedFriends;
//     score = -1;

//     // Find the specified user in the network
//     User* specifiedUser = getUser(who);

//     if (specifiedUser == nullptr) {
//         return suggestedFriends; // User not found
//     }
    
//     std::vector<std::size_t> specifiedFriends = specifiedUser->getFriends();

//     for (auto specFriend : specifiedFriends){
//         User* friendOfFriend = getUser(specFriend);
//         std::vector<std::size_t> friendsOfSpecFriend = friendOfFriend->getFriends();

//         size_t commonFriends = 0;

//         for(auto potentialCommon : friendsOfSpecFriend){
            
//         }
        

//     }

//     // // Iterate through the network to find potential friends
//     // for (User* user : network) {
//     //     // Skip the specified user and users who are already friends
//     //     if (user->getId() != who && !specifiedUser->isFriend(user->getId())) {
//     //         // Calculate the number of common friends manually
//     //         std::size_t commonFriends = 0;

//     //         for (std::size_t friendID : user->getFriends()) {
//     //             if (specifiedUser->isFriend(friendID)) {
//     //                 commonFriends++;
//     //             }
//     //         }

//     //         // If the common friends count is greater than the current highest score, update the score
//     //         if (commonFriends > score) {
//     //             score = commonFriends;
//     //             suggestedFriends.clear();
//     //             suggestedFriends.push_back(user->getId());
//     //         } else if (commonFriends == score) {
//     //             // If the common friends count equals the current highest score, add to the suggestions
//     //             suggestedFriends.push_back(user->getId());
//     //         }
//     //     }
//     // }

//     return suggestedFriends;
// }




// std::vector<std::size_t> Network::suggestFriends(std::size_t who, std::size_t& score) {
//     // Initialize variables to keep track of suggested friends and the highest score
//     std::vector<std::size_t> suggestedFriends;
//     score = 0;
    

//     // Find the specified user in the network
//     User* specifiedUser = getUser(who);

//     if (specifiedUser == nullptr) {
//         return suggestedFriends; // User not found
//     }

//     std::vector<std::size_t> immediateFriends = specifiedUser->getFriends();

//     for (size_t immediateFriendID : immediateFriends) {
//         //get all immediate friends
//         User* immediateFriend = getUser(immediateFriendID);
//         //get friends of immediate friends (they are potential suggestions)
//         std::vector<std::size_t> friendsOfImmediateFriend = immediateFriend->getFriends();

//         //cycle through potential suggestions
//         for (size_t friendOfImmediateFriendID : friendsOfImmediateFriend){
//             //create users for potential suggestions
//             User* friendOfImmediateFriend = getUser(friendOfImmediateFriendID);
//             //create vector to cycle through potential common friends
//             std::vector<std::size_t> potentialCommons = friendOfImmediateFriend->getFriends();

//             size_t commonFriends = 0;

//             //cycle through potential commons
//             for(size_t potentialCommon: potentialCommons){
//                 //check if potential common is friends with specified User
//                 if(specifiedUser->isFriend(potentialCommon)){
//                     //add if true
//                     commonFriends++;
//                 }
//             }

//             std::cout << "Common Friends: " << commonFriends << std::endl;
//             std::cout << "Score: " << score << std::endl;

//             // If the common friends count is greater than the current highest score, update the score
//             if (commonFriends > score) {
//                 score = commonFriends;
//                 suggestedFriends.clear();
//                 suggestedFriends.push_back(friendOfImmediateFriendID);
//             } else if (commonFriends == score) {
//                 // If the common friends count equals the current highest score, add to the suggestions
//                 suggestedFriends.push_back(friendOfImmediateFriendID);
//             }
//         }
//     }
//     return suggestedFriends;
// }



std::vector<std::size_t> Network::suggestFriends(std::size_t who, std::size_t& score) {
    // Initialize variables to keep track of suggested friends and the highest score
    std::vector<std::size_t> suggestedFriends;
    score = 0;

    // Find the specified user in the network
    User* specifiedUser = getUser(who);

    if (specifiedUser == nullptr) {
        return suggestedFriends; // User not found
    }

    std::vector<std::size_t> specifiedFriends = specifiedUser->getFriends();

    for (User* friendOfFriend : network) {
        // Skip the specified user, users who are already friends, and users without friends
        if (friendOfFriend->getId() != who && !specifiedUser->isFriend(friendOfFriend->getId()) && !friendOfFriend->getFriends().empty()) {
            // Calculate the number of common friends using the isFriend function
            std::size_t commonFriends = 0;

            for (std::size_t friendID : friendOfFriend->getFriends()) {
                if (specifiedUser->isFriend(friendID)) {
                    commonFriends++;
                }
            }

            // If the common friends count is greater than the current highest score, update the score
            if (commonFriends > score) {
                score = commonFriends;
                suggestedFriends.clear();
                suggestedFriends.push_back(friendOfFriend->getId());
            } else if (commonFriends == score) {
                // If the common friends count equals the current highest score, add to the suggestions
                suggestedFriends.push_back(friendOfFriend->getId());
            }
        }
    }

    return suggestedFriends;
}

