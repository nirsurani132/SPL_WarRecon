#include <iostream>
#include <cstdlib> // For system()

int main() {
    // List of input files
    const char* input_files[] = {"input1.txt", "input2.txt", "input3.txt"};
    const size_t num_files = sizeof(input_files) / sizeof(input_files[0]);

    // Loop through each input file
    for (size_t i = 0; i < num_files; ++i) {
        // Construct the output file name
        std::string input_file = input_files[i];
        std::string output_file =  "./outputs/" + input_file.substr(0, input_file.find_last_of('.')) + "_output.txt";

        // Construct the command string
        std::string command = "../bin/simulation ../config_file.txt < ./" + input_file + " > " + output_file;

        // Execute the command
        int result = system(command.c_str());
        if (result != 0) {
            std::cerr << "Error executing command: " << command << std::endl;
        } else {
            std::cout << "Processed " << input_file << " -> " << output_file << std::endl;
        }
    }

    return 0;
}
