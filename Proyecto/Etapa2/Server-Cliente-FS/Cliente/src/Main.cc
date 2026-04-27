// Copyright 2026a Equipo 1 CI-0123. ECCI-UCR. CC BY 4.0

#include <cstdio>

#include "Cliente.h"

int main(int argc, char* argv[]) {
  if (argc > 2) {
    int port = std::atoi(argv[2]);
    if (port == 0) return -1;
    Cliente c(argv[1], port);
    c.run();
  } else 
  return 0;
}