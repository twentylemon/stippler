
#include "main.h"

int main(int argc, char** argv) {
    Stippler stippler(400, 400, 100);
    std::ofstream svg("output.svg");
    svg << stippler;
    svg.close();
    system("pause");
    return 0;
}
