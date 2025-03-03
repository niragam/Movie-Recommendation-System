#ifndef USER_H
#define USER_H

#include <string>
#include <set>

class User
{
private:
    unsigned long int id; // uniqe id
    std::set<unsigned long int> moviesWatched; // set of the movies the user has watched

public:
    User(const unsigned long int &id); //constructor
    const unsigned long int &getUserId() const; // getter for the user's id
    const std::set<unsigned long int> &getMovies() const; // getter for the set of the movies the user has watched
    bool hasWatched(const unsigned long int &movieId) const; // checks if the user has watched this movie
    bool addMovie(const unsigned long int &movieId); //adding the movie to the userws movies list
    bool deleteMovie(const unsigned long int& movieId); //deleting the movie to the userws movies list

    // Declare friend function
    friend bool operator<(const User &lhs, const User &rhs);
};

inline bool operator<(const User &lhs, const User &rhs)
{
    return lhs.id < rhs.id;
}

#endif
