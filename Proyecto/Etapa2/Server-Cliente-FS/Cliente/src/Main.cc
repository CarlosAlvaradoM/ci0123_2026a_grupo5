// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <cstdio>

#include "Cliente.h"

int main(int argc, char* argv[]) {
  if (argc > 2) {
    Cliente c(argv[1], std::atoi(argv[2]));
    c.run();
  } else 
  return 0;
}