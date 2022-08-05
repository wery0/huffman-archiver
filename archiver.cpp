#include "Encoder.h"
#include "Decoder.h"
#include <ctime>

void ConsoleHelp() {
    std::cout << "Hello! This is Haffman archiver.\n"
                 "It maintains 3 following commands:\n"
                 "1). -c archive_name file1 [file2 ...] - archive files file1 [file2 ...] and save result in file archive_name.\n"
                 "2). -d archive_name - unzip files from archive archive_name and put to the current directory.\n"
                 "3). -h - help.\n";
}

void EncodeFiles(const std::string &archive_name, std::vector<std::string> &files) {
    std::ofstream ostream = std::ofstream(archive_name, std::ios::binary);
    Haffman::Encoder encoder(ostream);
    for (const auto &file_name: files) {
        encoder.EncodeFile(file_name);
    }
    encoder.Close();
    std::cout << "All files encoded successfully." << std::endl;
}

void DecodeArchive(const std::string &archive_name) {
    std::ifstream istream = std::ifstream(archive_name, std::ios::binary);
    if (!istream) {
        throw std::invalid_argument("No such file " + archive_name);
    }
    Haffman::Decoder decoder;
    decoder.DecodeArchive(istream);
}

int main(int argc, const char *argv[]) {
    auto start_time = clock();
    try {
        if (argc == 1) {
            std::cout << "Error: expected arguments.";
            return 1;
        }
        std::string arg = argv[1];
        if (arg == "-h") {
            ConsoleHelp();
        } else if (arg == "-c") {
            if (argc == 2) {
                std::cout << "Error: Expected archive_name.";
                return 1;
            }
            if (argc == 3) {
                std::cout << "Error: No files to archive.";
                return 1;
            }
            std::string archive_name = argv[2];
            std::vector<std::string> files;
            for (size_t i = 3; i < argc; ++i) {
                files.push_back(argv[i]);
            }
            EncodeFiles(archive_name, files);
        } else if (arg == "-d") {
            if (argc == 2) {
                std::cout << "Error: Expected archive_name.";
                return 1;
            }
            if (argc > 3) {
                std::cout << "Error: Extra arguments. Expected only archive_name.";
                return 1;
            }
            DecodeArchive(argv[2]);
        } else {
            std::cout << "Error: Invalid command.";
            return 1;
        }
    }
    catch (std::invalid_argument &err) {
        std::cout << err.what() << std::endl;
    }
    catch (std::length_error &err) {
        std::cout << err.what() << std::endl;
    }
    std::cerr << "Working time: " << (clock() - start_time) * 1.0 / CLOCKS_PER_SEC << " s" << std::endl;
    return 0;
}
