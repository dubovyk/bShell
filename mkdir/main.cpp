#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

#include <iostream>

void make_dir(boost::filesystem::path dir) {
    if (fs::exists(dir)) {
        std::cout << "Can not create directory " << dir << ": File exists" << std::endl;
        return;
    }
    fs::create_directory(dir);
}

int main(int ac, char *av[]) {
    try {

        po::options_description desc("Allowed options");
        desc.add_options()
                ("help,h", "Display help message")
                ("dirs", po::value<std::vector<boost::filesystem::path>>(), "path to directory)");

        po::positional_options_description positionalOptions;
        positionalOptions.add("dirs", -1);

        po::variables_map vm;
        po::store(
                po::command_line_parser(ac, av)
                        .options(desc)
                        .positional(positionalOptions)
                        .run(),
                vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }
        if (vm.count("dirs")) {
            std::vector<boost::filesystem::path> dirs = vm["dirs"].as<std::vector<boost::filesystem::path>>();
            for (boost::filesystem::path dir:dirs) {
                make_dir(dir);
            }
        } else {
            std::cout << "Missing operand." << std::endl;
        }
    }
    catch (std::exception &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    catch (...) {
        std::cerr << "Exception of unknown type!" << std::endl;
    }
    return 0;
}
