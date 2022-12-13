#include "check_time.h"

void check_time(int start_index, int end_index, int step, bool native_bnb) {
    for (int n = start_index; n <= end_index; n += step) {
        Graph g = random_graph(n, 2, 5);

        auto time1 = std::chrono::high_resolution_clock::now();
        auto i =  tsp_ls(g, 1);
        auto time2 = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double>(time2 - time1).count();
        std::cout << "N = " << n << " TSP LS | Time: " << duration << " Length: " << path_length(g, i) << std::endl;

        time1 = std::chrono::high_resolution_clock::now();
        i = tsp_greed(g, 1);
        time2 = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration<double>(time2 - time1).count();
        std::cout << "N = " << n << " TSP GREED | Time: " << duration << " Length: " << path_length(g, i) << std::endl;

        if (native_bnb) {
            time1 = std::chrono::high_resolution_clock::now();
            i = tsp_bnb(g, 1);
            time2 = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration<double>(time2 - time1).count();
            std::cout << "N = " << n << " TSP BNB | Time: " << duration << " Length: " << path_length(g, i) << std::endl;

            time1 = std::chrono::high_resolution_clock::now();
            i = tsp_native(g, 1);
            time2 = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration<double>(time2 - time1).count();
            std::cout << "N = " << n << " TSP NATIVE | Time: " << duration << " Length: " << path_length(g, i) << std::endl;
        }
    }
}