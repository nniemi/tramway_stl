/* RASSE2
 *
 *
 * Description:
 * This program is designed to allow the user to access and store data related
 * to a tramway network. The data is initially read from an input file, then
 * stored to a data structure. The user is then prompted to enter one of
 * several commands, which function as follows:
 * QUIT: Terminates the program.
 * LINES: Prints out all lines in alphabetical order.
 * LINE <line>: Prints out all stops on <line>, in distance order.
 * STOPS: Prints out all stops in alphabetical order.
 * STOP <stop>: Prints out the lines containing <stop>.
 * DISTANCE <line> <stop1> <stop2>: Calculates and prints out the distance
 * between <stop1> and <stop2> on <line>.
 * ADDLINE <line>: Adds <line> to the data structure.
 * ADDSTOP <line> <new stop> <distance>: Adds <new stop> on <line> in the
 * data structure, with the <distance> given.
 * REMOVE <stop>: Removes <stop> from the data structure.
 *
 *
 */


#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>


const std::string LINE_ERROR = "Error: Stop/line already exists."
const std::string LINE_NOT_FOUND = "Error: Line could not be found."
const std::string STOP_NOT_FOUND = "Error: Stop could not be found."


// Declares the data structure.
using Tramway = std::map<std::string, std::map<std::string, double>>;



// Reads the given line, splits it and stores the data to the data structure.
bool read_line(std::string s, char delimiter, Tramway& tramway, bool ignore_empty = true){
    std::vector<std::string> result;
    std::string tmp = s;

    // Loops through the given line.
    while(tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());
        if(not (ignore_empty and new_part.empty()))
        {
            result.push_back(new_part);

        }
    }

    if(not (ignore_empty and tmp.empty()))
    {
        result.push_back(tmp);


    }

    // Checks if the line is valid size.
    if (result.size() >= 2)
    {
        if (result.size() == 2) {
            tramway[result.at(0)][result.at(1)] = 0.0;
            return true;
        }

        // Converts the given distance to a double.
        double dist = stod(result.at(2));

        // Checks that there are no duplicate stops.
        if (tramway[result.at(0)].find(result.at(1)) != tramway[result.at(0)].end()){

            std::cout << LINE_ERROR << std::endl;
            return false;

        }

        // Checks if there are stops with the same distance.
        if (tramway[result.at(0)].find(result.at(2)) != tramway[result.at(0)].end()){

            std::cout << LINE_ERROR << std::endl;
            return false;
        }
        // Inserts the values to the data structure.
        tramway[result.at(0)][result.at(1)] = dist;

    } else {

        return false;
    }

    return true;
}

// Reads the given input text file.
bool read_input_file(Tramway& tramway) {



    bool valid_file = true;

    std::string file_name = "";
    std::cout << "Give a name for input file: ";
    getline(std::cin, file_name);


    std::ifstream file_object(file_name);

    if ( not file_object) {

        // Prints an error message if reading the file failed.
        std::cout << "Error: File could not be read." << std::endl;
        return valid_file = false;
    }

       std::string row;
       std::vector <std::string> rows;
       while (std::getline(file_object, row)) {

           if (read_line(row, ';', tramway)) {

               continue;

           } else {

               // Prints an error message if the formatting is invalid.
               std::cout << "Error: Invalid format in file." << std::endl;
               return valid_file = false;
           }

       }
       // Closes the file.
       file_object.close();
        return valid_file;
}



// Split function for splitting the user input.
std::vector<std::string> split(const std::string& s, const char delimiter, bool ignore_empty = false){

    std::vector<std::string> result;
    std::string tmp = s;
    std::vector<std::string> temp_v;
    std::string temp_s;
    bool multi_part_split = false;



    while(tmp.find(delimiter) != std::string::npos)
    {
        std::string new_part = tmp.substr(0, tmp.find(delimiter));
        tmp = tmp.substr(tmp.find(delimiter)+1, tmp.size());

        if(not (ignore_empty and new_part.empty()))
        {
            // Checks if there are quotation marks in the current part of the command input.
            if (new_part.at(0) == '"') {
                new_part.erase(new_part.begin());
                temp_s += new_part;
                temp_s += " ";
                multi_part_split = true;

            } else {
              // Stores the current word to a vector.
              temp_v.push_back(new_part);
              result.push_back(new_part);
            }

        }
    }
    if(not (ignore_empty and tmp.empty()))
    {
        // Checks if there are quotation marks in the last part of the command input.
        // Also adds the last word to a string which forms a whole element to store.
        if(tmp.back() == '"') {
            tmp.pop_back();
            temp_s += tmp;
            temp_v.push_back(temp_s);

        } else {
          // Stores the last word to a vector.
          temp_v.push_back(tmp);
          result.push_back(tmp);

        }

    }

    // Determines which vector to return.
    if (multi_part_split) {
        return temp_v;
    } else {
        return result;
    }

}

// Function to print out all the lines stored in the data structure.
void lines(Tramway& tramway) {
    std::cout << "All tramlines in alphabetical order:" << std::endl;

    for (auto lines : tramway) {
        std::cout << lines.first << std::endl;
    }
}

// Function to flip the map, so the map will be sorted by distance.
std::multimap<double, std::string> flip_map(Tramway& tramway, std::string line) {


    std::multimap<double, std::string> flipped_stops;

    for(std::map<std::string, double>::const_iterator it = tramway[line].begin(); it != tramway[line].end(); ++it)
    {

        flipped_stops.insert(std::pair<double, std::string>(it -> second, it ->first));

    }

    return flipped_stops;
}

// Function to print all the stops and distances of a particular line.
void line_print(Tramway& tramway, std::string line) {

    std::cout << "Line " << line << " goes through these stops in the order they are listed: " << std::endl;


    std::multimap<double, std::string> flipped_stops = flip_map(tramway, line);


    for (auto stops : flipped_stops)
    {

        std::cout << "- " << stops.second << " : " << stops.first << std::endl;

    }

}

// Function to print every stop in the data structure in alphabetical order.
void stops(Tramway& tramway) {

    std::cout << "All stops in alphabetical order: " << std::endl;
    std::vector<std::string> stop_vector;

    for (auto lines : tramway) {

        for (auto stops : tramway[lines.first]) {
            if (std::find(stop_vector.begin(), stop_vector.end(), stops.first) == stop_vector.end()) {
                stop_vector.push_back(stops.first);
            }

        }
    }
    sort(stop_vector.begin(), stop_vector.end());

    for (std::string i : stop_vector) {
        std::cout << i << std::endl;
    }
}

// Function to print every line which contains the particular stop.
void stop_print(Tramway& tramway, std::string stop) {

    std::vector<std::string> temp_vector;
    for (auto lines : tramway) {
            if (lines.second.find(stop) != lines.second.end()) {
                temp_vector.push_back(lines.first);

            }   else {
                continue;
            }
    }
    sort(temp_vector.begin(),temp_vector.end());
    std::cout << "Stop " << stop << " can be found on the following lines: " << std::endl;
    for (std::string line : temp_vector) {
        std::cout << " - " << line  << std::endl;
    }
}

// Function to print the distance between two different lines.
void distance(Tramway& tramway, std::string line, std::string stop1, std::string stop2) {
    double distance;

    if (tramway[line][stop1] >= tramway[line][stop2]) {
        distance = tramway[line][stop1] - tramway[line][stop2];

    } else {
        distance = tramway[line][stop2] - tramway[line][stop1];
    }

    std::cout << "Distance between " << stop1 << " and " << stop2 << " is " << distance << std::endl;
}

//Function to add an empty line to the data structure.
void add_line(Tramway& tramway, std::string line) {

    std::map<std::string, double> new_map;

    if (tramway.find(line) == tramway.end()) {
        tramway.insert({line, new_map});
        std::cout << "Line was added." << std::endl;

    } else {
        std::cout << LINE_ERROR << std::endl;
    }

}

// Function to add a stop to the given line.
void add_stop(Tramway& tramway, std::string line, std::string stop, double distance) {

    bool diff_distances = true;
    if (tramway.find(line) != tramway.end()) {
        if (tramway[line].find(stop) == tramway[line].end()) {

            for (auto stops : tramway[line]) {
                if (stops.second !=  distance) {
                    continue;
                } else {
                    diff_distances = false;
                    std::cout << LINE_ERROR << std::endl;
                    break;
                }
            }
            if (diff_distances) {
                tramway[line].insert({stop, distance});
                std::cout << "Stop was added." << std::endl;
            }

        } else {
            std::cout << LINE_ERROR << std::endl;
      }
    } else {
        std::cout << LINE_NOT_FOUND << std::endl;
    }

}

// Function to remove a stop entirely from the data structure.
void remove(Tramway& tramway, std::string stop) {


    bool does_exist = false;
    for (auto lines : tramway) {
        if(lines.second.find(stop) != lines.second.end()) {

            does_exist = true;
        }
    }
    if (does_exist) {
        std::map<std::string, std::map<std::string, double>> ::iterator iter;
        iter = tramway.begin();
        while (iter != tramway.end()) {
            iter->second.erase(stop);
            ++iter;

            }
        std::cout << "Stop was removed from all lines." << std::endl;
    } else {
        std::cout << STOP_NOT_FOUND << std::endl;
    }



}

// The most magnificent function in this whole program.
// Prints a RASSE
void print_rasse()
{
    std::cout <<
                 "=====//==================//===\n"
                 "  __<<__________________<<__   \n"
                 " | ____ ____ ____ ____ ____ |  \n"
                 " | |  | |  | |  | |  | |  | |  \n"
                 " |_|__|_|__|_|__|_|__|_|__|_|  \n"
                 ".|                  RASSE   |. \n"
                 ":|__________________________|: \n"
                 "___(o)(o)___(o)(o)___(o)(o)____\n"
                 "-------------------------------" << std::endl;
}


// "Short and sweet" main.
int main()
{
    print_rasse();


    Tramway tramway;



    if (!read_input_file(tramway)) {
        return EXIT_FAILURE;
    }


       // Runs the prompts.
       // Error checking is integrated into each command.
         while(true){
           std::string line;
           std::cout << "tramway> ";
           getline(std::cin, line);
           std::vector<std::string> parts = split(line, ' ', true);

           std::string command = parts.at(0);

           // Converts the command input to uppercase.
           int i = 0;
           char c;
           while (command[i])
           {
               c = command[i];
               command[i] = toupper(c);
               i++;
           }



           if(command == "LINES"){
               if(parts.size() != 1){
                   std::cout << "Error: Invalid input." << std::endl;
                   continue;
                    }

               lines(tramway);


           } else if(command == "LINE"){
               if(parts.size() != 2){
                   std::cout << "Error: Invalid input." << std::endl;
                   continue;
               }
               std::string line = parts.at(1);

               if (tramway.find(line) != tramway.end()) {
                 line_print(tramway, line);
               } else {
                   std::cout << LINE_NOT_FOUND << std::endl;
                   continue;
               }


           } else if(command == "STOPS"){
               if(parts.size() != 1){
                   std::cout << "Error: Invalid input." << std::endl;
                   continue;
               }
               stops(tramway);


           } else if(command == "STOP"){
               if(parts.size() != 2){
                   std::cout << "Error: Invalid input." << std::endl;
                   continue;
               }

               std::string stop = parts.at(1);
               bool can_continue = false;

               for (auto lines : tramway) {
                   if (lines.second.find(stop) != lines.second.end()) {
                       can_continue = true;
                   }
               }
               if (can_continue) {
                   stop_print(tramway, stop);
               } else {
                   std::cout << STOP_NOT_FOUND << std::endl;
                   continue;
               }

               } else if(command == "DISTANCE"){

                   if(parts.size() != 4){
                       std::cout << "Error: Invalid input." << std::endl;
                       continue;
                   }
                   std::string line = parts.at(1);
                   std::string stop1 = parts.at(2);
                   std::string stop2 = parts.at(3);

                   if (tramway.find(line) != tramway.end()) {

                       // Checks that both of the stops exist on the given line.
                       if(tramway[line].find(stop1) == tramway[line].end() ||
                               tramway[line].find(stop2) == tramway[line].end()) {
                          std::cout << STOP_NOT_FOUND << std::endl;
                          continue;

                       } else {
                          distance(tramway, line, stop1, stop2);
                       }

                   } else {
                       std::cout << LINE_NOT_FOUND << std::endl;
                       continue;
                   }

               } else if(command == "ADDLINE"){
                   if(parts.size() != 2){
                       std::cout << "Error: Invalid input." << std::endl;
                       continue;
                   }
                   std::string line = parts.at(1);
                   add_line(tramway, line);


               } else if(command == "ADDSTOP"){
                   if(parts.size() != 4){
                       std::cout << "Error: Invalid input." << std::endl;
                       continue;
                   }
                   std::string line = parts.at(1);
                   std::string stop = parts.at(2);
                   double distance = stod(parts.at(3));

                   add_stop(tramway, line, stop, distance);


                } else if(command == "REMOVE"){
                      if(parts.size() != 2){
                          std::cout << "Error: Invalid input." << std::endl;
                          continue;
                      }
                      std::string stop = parts.at(1);
                      remove(tramway, stop);


               } else if(command == "QUIT"){
                              if(parts.size() != 1){
                                  std::cout << "Error: Invalid input." << std::endl;
                                  continue;
                    }      return EXIT_SUCCESS;

              } else {
               std::cout << "Error: Invalid input." << std::endl;

           }



    }


}

