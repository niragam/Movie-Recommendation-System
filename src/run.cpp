#include "run.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// Function to add a user and their watched movies
void addFunction(std::istringstream &input, MovieManager &manager)
{
    std::string userId;
    input >> userId; // Read user ID from input
    try
    {
        if (!std::all_of(userId.begin(), userId.end(), ::isdigit)) 
        {
            return; // Return if userId contains non-digit characters
        }
        int signedId = std::stoi(userId); // Check if negative
        if (signedId <= 0)
        {
            return;
        }

        unsigned long int id = std::stoul(userId); // Convert user ID to unsigned long int

        std::vector<unsigned long int> movieIds;
        std::string movieId;
        while (input >> movieId) // Read movie IDs from input
        {
            if (!std::all_of(movieId.begin(), movieId.end(), ::isdigit))
            {
                return; // Return if movieId contains non-digit characters
            }

            int signedMId = std::stoi(movieId); // Check if negative
            if (signedMId <= 0)
            {
                return;
            }
            unsigned long int mId = std::stoul(movieId); // Convert movie ID to unsigned long int
            movieIds.push_back(mId);                     // Add movie ID to the list
        }
        if (movieIds.empty())
        {
            return;
        }
        manager.addUser(id);             // Add user to the manager
        manager.addMovies(id, movieIds); // Add movies to the user
    }
    catch (const std::invalid_argument &e)
    {
        return; // Ignore invalid input that cannot be converted
    }
    catch (const std::out_of_range &e)
    {
        return; // Ignore input values that are out of range
    }
}

// Function to recommend movies for a user based on a reference movie
void recommendFunction(std::istringstream &input, MovieManager &manager, std::ostream &output)
{
    std::string userId, referenceMovieId;
    input >> userId >> referenceMovieId; // Read user ID and reference movie ID
    try
    {
        unsigned long int id = std::stoul(userId);                                           // Convert user ID to unsigned long int
        unsigned long int refId = std::stoul(referenceMovieId);                              // Convert reference movie ID to unsigned long int
        std::vector<unsigned long int> recommendations = manager.recommendMovies(id, refId); // Get recommendations

        if (recommendations.empty())
        {
            return; // No recommendations, exit the function
        }

        for (unsigned long int rec : recommendations) // Output recommendations
        {
            output << rec << " ";
        }
        output << "\n"; // End the recommendations with a newline
    }
    catch (const std::invalid_argument &e)
    {
        return; // Ignore invalid input that cannot be converted
    }
    catch (const std::out_of_range &e)
    {
        return; // Ignore input values that are out of range
    }
    catch (const std::runtime_error &e)
    {
        return; // Ignore runtime errors, such as user not found
    }
}

// Function to display help information
void helpFunction(std::ostream &output)
{
    output << "add [userid] [movieid1] [movieid2] ...\n" // Help for adding a user and movies
           << "recommend [userid] [movieid]\n"           // Help for recommending movies
           << "help\n";                                  // Help for displaying this message
}

// Main function to run the program
void run(std::istream &input, std::ostream &output)
{
    MovieManager manager;                                 // Create a MovieManager instance
    const std::string dataFile = "../data/user_data.txt"; // Define the data file path

    // Load user data from file
    manager.loadData(dataFile);

    // Infinite loop to handle user commands
    while (true)
    {
        std::string inputLine;
        std::getline(input, inputLine); // Read a line of input
        std::istringstream inputStream(inputLine);
        std::string command;
        inputStream >> command; // Extract the command

        if (command == "add") // Handle 'add' command
        {
            addFunction(inputStream, manager); // Call the add function
            manager.saveData(dataFile);        // Save updated data to file
        }
        else if (command == "recommend") // Handle 'recommend' command
        {
            recommendFunction(inputStream, manager, output); // Call the recommend function
        }
        else if (command == "help") // Handle 'help' command
        {
            helpFunction(output); // Call the help function
        }
    }
}
