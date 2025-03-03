# **Movie Recommendation System**

This is the second phase of a four-part project for the Advanced System Programming (Full Stack) course.

This program provides a command-line interface (CLI) that will be used as a movie recommendation system. 

The program supports the following commands:

### **Post command**
```bash
POST [userid] [movieid1] [movieid2] ...
```


**Description:** If the user does not exist, the command associates a list of movies (movie IDs) with the new user (user ID), indicating that the user has watched these movies.

**Example:**
running a valid POST command

![POST](Instruction-pictures/POST.png)


--------------------------------------------------------

### **PATCH command**
```bash
PATCH [userid] [movieid1] [movieid2] ...
```

**Description:** If the user exists, the command associates a list of movies (movie IDs) with a user (user ID), indicating that the user has watched these movies.

**Example:**
running a valid PATCH command

![PATCH](Instruction-pictures/PATCH.png)


--------------------------------------------------------

### **DELETE command**
```bash
DELETE [userid] [movieid1] [movieid2] ...
```
**Description:** The command allows you to delete watch history from a user

**Example:**

running a valid DELETE command

![DELETE](Instruction-pictures/DELETE.png)

--------------------------------------------------------

### **GET command**
```bash
GET [userid] [movieid]
```
**Description:** The GET command allows you to input a user ID and a single movie ID. The system will provide up to 10 additional movie recommendations based on the viewing preferences of other users with similar tastes.

**Example:**

running a valid GET command will print something like:

![GET](Instruction-pictures/GET.png)


depending on the movie ID and the appropriate recommendations

--------------------------------------------------------


### **3. Display Help**
```bash
help
```
**Description:** The command will print a list of all available commands on the screen.

**Example:**

running the help command will print this:

![help](Instruction-pictures/help.png)


----------------------------------------------------------------------------------------------------------------


## **Instructions to Run the Program Using Docker**

First we need to build the docker image and start a container, this will run the program in a docker container

### **1. Build docker image**

```bash
docker build -t movie_app .
```
This command creates a Docker image named `movie_app` using the Dockerfile in the current directory.



### **2. Run a container from the docker image**
```bash
docker run -it --name movie_app_run movie_app
```
This command starts a container named `movie_app_run` from the `movie_app` image.

### **3. If you are working on windows, use this command to convert ./runServer.sh from linux to windows**
```bash
sed -i 's/\r//' runServer.sh 
```
Convert ./runServer.sh from linux to windows

### **4. Run the server inside the container with port number**
```bash
./runServer.sh <portNumber>
```
Replace portNumber with your selected port number.

### **5. Run the client inside the container with port number and ip address**
To run a client, after the docker container is already running, open a new terminal and run
```bash
docker exec -it movie_app_run /bin/bash
```

Then run the client
```bash
./runClient.sh <ipAddress> <portNumber>
```
Replace ipAddress and portNumber with your selected ip address and the port number you selected for the server

**If you are working on windows, use this command before running the client, to convert ./runClient.sh from linux to windows**
```bash
sed -i 's/\r//' runClient.sh 
```
Convert ./runClient.sh from linux to windows


You can now run the commands on the client side

--------------------------------------------------------

### **Stop the container**
From a terminal that is not running inside the docker container
```bash
docker stop movie_app_run
```

From a terminal that is running inside the docker container press CTRL+D

--------------------------------------------------------

If the container was stopped 
### **Resume the container**
```bash
docker start -i movie_app_run
```
--------------------------------------------------------

### **Example of a complete run**

![complete-run](Instruction-pictures/run.png)

--------------------------------------------------------

### **Run Tests**
To run tests after building the docker image use this command inside the container terminal:

```bash
cd build && ./testsMovies
```
![tests-run](Instruction-pictures/testRun.png)

