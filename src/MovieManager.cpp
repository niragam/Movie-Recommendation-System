#include "MovieManager.h"

#include <unordered_map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <filesystem>
#include <algorithm>

#define MAX_RECOMMENDATIONS 10

// Adds a new user to the system
bool MovieManager::addUser(const unsigned long int &userId)
{
    std::unique_lock<std::shared_mutex> lock(managerMutex);
    // Search for the user by ID
    auto reqUser = std::find_if(users.begin(), users.end(),
                                [userId](const User &user)
                                { return user.getUserId() == userId; });

    // If the user already exists, return false
    if (reqUser != users.end())
    {
        return false;
    }

    // Add the user to the list of users
    users.push_back(User(userId));
    return true;
}

// Retrieves a user by their ID
User MovieManager::getUser(const unsigned long int &userId)
{
    std::shared_lock<std::shared_mutex> lock(managerMutex);
    auto reqUser = std::find_if(users.begin(), users.end(),
                                [userId](const User &user)
                                { return user.getUserId() == userId; });

    // If the user is found, return them
    if (reqUser != users.end())
    {
        return *reqUser;
    }
    User user = User(0); // return error
    return user;
}

// Adds movies to the specified user's list
bool MovieManager::addMovies(const unsigned long int &userId, const std::vector<unsigned long int> &movieIds)
{
    std::unique_lock<std::shared_mutex> lock(managerMutex);

    // Search for the user by ID
    auto reqUser = std::find_if(users.begin(), users.end(),
                                [userId](const User &user)
                                { return user.getUserId() == userId; });

    // If the user is not found, return false
    if (reqUser == users.end())
    {
        return false;
    }

    // Add each movie to the user's list if not already present
    for (const auto &movieId : movieIds)
    {
        if (!reqUser->hasWatched(movieId))
        {
            reqUser->addMovie(movieId);
        }
    }
    return true;
}

// Delete movies to the specified user's list
bool MovieManager::deleteMovies(const unsigned long int &userId, const std::vector<unsigned long int> &movieIds)
{
    std::unique_lock<std::shared_mutex> lock(managerMutex);

    // Search for the user by ID
    auto reqUser = std::find_if(users.begin(), users.end(),
                                [userId](const User &user)
                                { return user.getUserId() == userId; });

    // If the user is not found, return false
    if (reqUser == users.end())
    {
        return false;
    }

    // delete each movie to the user's list if not already present
    for (const auto &movieId : movieIds)
    {
        if (reqUser->hasWatched(movieId))
        {
            reqUser->deleteMovie(movieId);
        }
        else
        {
            return false;
        }
    }
    return true;
}

// Saves user data to a file
void MovieManager::saveData(const std::string &filename) const
{
    std::unique_lock<std::shared_mutex> lock(managerMutex);

    std::ofstream file(filename);
    if (!file)
    {
        return;
    }

    // Write each user's data (ID and movie list) to the file
    for (const auto &user : users)
    {
        file << user.getUserId();
        for (const auto &movieId : user.getMovies())
        {
            file << " " << movieId;
        }
        file << "\n";
    }

    file.close();
}

// Loads user data from a file
void MovieManager::loadData(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file)
    {
        return;
    }

    std::string line;

    // Read each line from the file
    while (std::getline(file, line))
    {
        std::istringstream lineStream(line);
        std::string userId;
        lineStream >> userId;

        // Convert user ID to integer and add the user
        unsigned long int id = std::stoul(userId);
        addUser(id);

        // Read and add the user's movies
        std::vector<unsigned long int> movieIds;
        std::string movieId;
        while (lineStream >> movieId)
        {
            unsigned long int mId = std::stoul(movieId);
            movieIds.push_back(mId);
        }
        addMovies(id, movieIds);
    }

    file.close();
}

// Helper function to count the number of common movies between two users
int MovieManager::countCommonMovies(User user1, User user2)
{
    std::shared_lock<std::shared_mutex> lock(managerMutex);
    int commonMovies = 0;
    for (unsigned long int movieOfUser1 : user1.getMovies())
    {
        for (unsigned long int movie : user2.getMovies())
        {
            if (movieOfUser1 == movie)
            {
                commonMovies++;
            }
        }
    }
    return commonMovies;
}

// Recommends movies to a user based on a reference movie ID
std::vector<unsigned long int> MovieManager::recommendMovies(unsigned long int userid, unsigned long int movieId)
{
    std::shared_lock<std::shared_mutex> lock(managerMutex);
    // Check if the user exists
    std::vector<unsigned long int> recommendations = {};
    User user = getUser(userid);
    if (user.getUserId() == 0)
    {
        recommendations.resize(1);
        recommendations[0] = 0;
        return recommendations;
    }

    // Calculate similarity with other users
    std::map<User, int> userSimilarity;
    for (const User &otherUser : users)
    {
        if (otherUser.getUserId() != userid) // Exclude the current user
        {
            userSimilarity[otherUser] = countCommonMovies(user, otherUser);
        }
    }

    // Find users who watched the reference movie
    std::vector<User> similarUsers;
    for (const auto &pair : userSimilarity)
    {
        const auto &otherUser = pair.first;
        const auto &similarity = pair.second;
        if (otherUser.hasWatched(movieId) && similarity > 0)
        {
            similarUsers.push_back(otherUser);
        }
    }

    // If no similar users are found, return an empty list
    if (similarUsers.empty())
    {
        return recommendations;
    }

    // Calculate movie relevance scores based on similar users
    std::unordered_map<unsigned long int, long long> movieRelevance;
    for (const User &similarUser : similarUsers)
    {
        int similarity = userSimilarity[similarUser];
        for (unsigned long int recommendedMovie : similarUser.getMovies())
        {
            // Avoid recommending movies the user has already watched or the reference movie
            if (recommendedMovie != movieId &&
                !user.hasWatched(recommendedMovie))
            {
                movieRelevance[recommendedMovie] += similarity;
            }
        }
    }

    // Sort movies by relevance (descending order)
    std::vector<std::pair<unsigned long int, long long>> sortedRecommendations(
        movieRelevance.begin(), movieRelevance.end());

    std::sort(sortedRecommendations.begin(), sortedRecommendations.end(),
              [](const std::pair<unsigned long int, long long> &a, const std::pair<unsigned long int, long long> &b)
              {
                  return (a.second > b.second) || ((a.second == b.second) && (a.first < b.first));
              });

    // Select top N recommendations
    for (const auto &pair : sortedRecommendations)
    {
        recommendations.push_back(pair.first);
        if (recommendations.size() >= MAX_RECOMMENDATIONS)
            break;
    }
    return recommendations;
}

bool MovieManager::checkIfStringIsOnlyDigits(std::string string)
{
    if (!std::all_of(string.begin(), string.end(), ::isdigit))
    {
        return false;
    }
    int signedInt = std::stoi(string);
    if (signedInt <= 0)
    {
        return false;
    }
    return true;
}
