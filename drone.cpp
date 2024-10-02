// Project Identifier: 1761414855B69983BD8035097EFBD312EB0527F0

#include <iostream>
#include <string>

#include "getopt.h"
#include "mst.h"
#include "opttsp.h"

using namespace std;

void getOptions(int argc, char** argv);

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false);
    cout.precision(2);   // Always show 2 decimal places
    cout << std::fixed;

    getOptions(argc, argv);
}

void getOptions(int argc, char** argv) {
    opterr = false;
    int choice;
    int index = 0;
    bool valid = true;

    // List of the options
    option long_options[] = {
        { "mode", required_argument, nullptr,  'm'},
        { "help",       no_argument, nullptr,  'h'},
        {nullptr,                 0, nullptr, '\0'}
    };

    while ((choice = getopt_long(argc, argv, "m:h", long_options, &index)) != -1) {
        valid = false;
        switch (choice) {
        case 'm': {
            string arg { optarg };
            if (arg == "MST") {
                MST mst;
                mst.readPoints();
                mst.runMST();
                valid = true;
            } else if (arg == "FASTTSP") {
                FASTTSP fasttsp;
                fasttsp.read();
                fasttsp.run();
                fasttsp.print();
                valid = true;
            } else if (arg == "OPTTSP") {
                OPTTSP opttsp;
                opttsp.read();
                opttsp.run();
                opttsp.print();
                valid = true;
            } else {
                cout << "Error: Invalid mode\n";
                exit(1);
            }
            break;
        }

        case 'h':
            cout << "This program reads in user input from command line and calculates"
                 << " the shortest path to all points in various scenarios.\n"
                 << "Usage: \'./drone\n\t[--mode | -m]\n"
                 << "\t[--help | -h]\n";
            exit(1);

        default:
            cout << "Error: Invalid command line option\n";
            exit(1);
        }
    }

    if (!valid) {
        cout << "Error: No mode specified\n";
        exit(1);
    }
}
