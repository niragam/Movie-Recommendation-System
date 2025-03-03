#include "PATCHCommand.h"

PATCHCommand::PATCHCommand(MovieManager &manager) : manager(manager) {}

// Function to add a user and their watched movies if the user exist
void PATCHCommand::execute(std::istringstream &input, std::ostream &output)
{
    std::string userId;
    input >> userId;         // Read user ID from input
    if (input.str().empty()) // if there is jusr one argument
    {
        output << "400 Bad Request";
        return; // Ignore invalid input that cannot be converted
    }
    try
    {
        bool check1 = manager.checkIfStringIsOnlyDigits(userId);
        if (!check1)
        {
            output << "400 Bad Request";
            return;
        }

        unsigned long int id = std::stoul(userId); // Convert user ID to unsigned long int

        std::vector<unsigned long int> movieIds;
        std::string movieId;
        bool check2;
        while (input >> movieId) // Read movie IDs from input
        {
            check2 = manager.checkIfStringIsOnlyDigits(movieId);
            if (!check2)
            {
                output << "400 Bad Request";
                return;
            }
            unsigned long int mId = std::stoul(movieId); // Convert movie ID to unsigned long int
            movieIds.push_back(mId);                     // Add movie ID to the list
        }
        if (movieIds.empty())
        {
            output << "400 Bad Request";
            return;
        }
        User user = manager.getUser(id);
        if (user.getUserId() == 0)
        {
            output << "404 Not Found";
            return;
        }

        output << "204 No Content";
        manager.addMovies(id, movieIds); // Add movies to the user
    }
    catch (const std::invalid_argument &e)
    {
        output << "400 Bad Request";
        return; // Ignore invalid input that cannot be converted
    }
    catch (const std::out_of_range &e)
    {
        output << "400 Bad Request";
        return; // Ignore input values that are out of range
    }
}