#include "lib.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc > 1) {

        otus::Bulk          bulk(atoi(argv[1]));
        otus::BulkProcessor proc;
        otus::BulkLogger    logg;

        bulk.subscribeBulk(std::bind(&otus::BulkProcessor::process, &proc, std::placeholders::_1));
        bulk.subscribeBulk(std::bind(&otus::BulkLogger::dump, &logg, std::placeholders::_1));
        bulk.subscribeTime(std::bind(&otus::BulkLogger::time, &logg, std::placeholders::_1));

        string line;
        while(getline(cin, line)) {
            bulk.append(line);
        }
    } else {
        cout << "Please, specify commands per block as argument." << endl;
        return 0;
    }
    return 0;
}
