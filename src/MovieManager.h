#ifndef MOVIEMANAGER_H
#define MOVIEMANAGER_H

#include "user.h"

#include <vector>
#include <string>
#include <shared_mutex>
#include <mutex>


class MovieManager
{
private:
    std::vector<User> users; // set of all the users in the program.
    mutable std::shared_mutex managerMutex;

public:
    bool addUser(const unsigned long int &userId);                                                       // adding the user to the program.
    User getUser(const unsigned long int &userId);                                                       // getting the user.
    bool addMovies(const unsigned long int &userId, const std::vector<unsigned long int> &movieIds);     // add movies to this given user.
    bool deleteMovies(const unsigned long int &userId, const std::vector<unsigned long int> &movieIds);     // delete movies to this given user.
    void saveData(const std::string &filename) const;                                                    // saving the data to this file.
    void loadData(const std::string &filename);                                                          // loading the data from this file.
    int countCommonMovies(User user1, User user2);                                                       // count the amount of common movies between users.
    std::vector<unsigned long int> recommendMovies(unsigned long int userid, unsigned long int movieId); // algorythm for calculatr recommendation
    bool checkIfStringIsOnlyDigits(std::string string);
};

#endif