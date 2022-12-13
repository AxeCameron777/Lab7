#define CATCH_CONFIG_RUNNER

#include "catch.hpp"
#include "check_time.h"

int main(int argc, char* argv[]) {
    using namespace std;
    check_time(150, 250, 25, false);
    int result = Catch::Session().run(argc, argv);
    return result;
}