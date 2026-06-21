#include "trino-query.h"

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include <curl/curl.h>

static void printUsage(const char* prog) {
    std::cerr
        << "Usage: " << prog
        << " <uri> <catalog> <schema> <table> [limit]\n\n"
        << "  uri      Trino coordinator base URI  (e.g. http://localhost:8080)\n"
        << "  catalog  Trino catalog name\n"
        << "  schema   Trino schema / database name\n"
        << "  table    Table to query (SELECT * FROM <table>)\n"
        << "  limit    Optional maximum number of rows to return\n";
}

int main(int argc, char* argv[]) {
    if (argc < 5 || argc > 6) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const std::string uri     = argv[1];
    const std::string catalog = argv[2];
    const std::string schema  = argv[3];
    const std::string table   = argv[4];

    std::optional<int> limit;
    if (argc == 6) {
        try {
            const int n = std::stoi(argv[5]);
            if (n <= 0) throw std::invalid_argument("must be a positive integer");
            limit = n;
        } catch (const std::exception& ex) {
            std::cerr << "Error: invalid limit value '" << argv[5]
                      << "': " << ex.what() << '\n';
            return EXIT_FAILURE;
        }
    }

    // curl_global_init must be called once before any curl handle is used.
    curl_global_init(CURL_GLOBAL_DEFAULT);

    int exitCode = EXIT_SUCCESS;
    try {
        trino::Client client(uri, catalog, schema);
        std::cout << client.selectAll(table, limit) << '\n';
    } catch (const trino::TrinoException& ex) {
        std::cerr << "Trino error: " << ex.what() << '\n';
        exitCode = EXIT_FAILURE;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        exitCode = EXIT_FAILURE;
    }

    curl_global_cleanup();
    return exitCode;
}
