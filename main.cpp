#include "App/Application.h"


int main(int argc, char *argv[])
{
    Application a(argc, argv);

    if (a.work())
      return EXIT_SUCCESS;

    return EXIT_FAILURE;
}
