#include "user.h"

User::User(const unsigned long int& id) : id(id) {} // constructor.

const unsigned long int& User::getUserId() const { // getting the id.
    return id;
}

const std::set<unsigned long int>& User::getMovies() const { // getting the set of watched movies (of the user).
    return moviesWatched; 
}

bool User::hasWatched(const unsigned long int& movieId) const { // checking if this user watched the movie.
    return moviesWatched.find(movieId) != moviesWatched.end();
}

bool User::addMovie(const unsigned long int& movieId) { // adding the movie to this user's watched movies set.
    return moviesWatched.insert(movieId).second; 
}

bool User::deleteMovie(const unsigned long int& movieId) { // deleting the movie to this user's watched movies set.
    return moviesWatched.erase(movieId) > 0; 
}



