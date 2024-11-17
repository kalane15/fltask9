#pragma once
#include "OpsAndDeps.h"
#include <stdio.h>
#include "stdlib.h"
#include "stdbool.h"

int RequestComparator(void* a, void* b);

kErrors ParseRequests(int argsc, char** args, MainModel* model);

kErrors UpdateDepsAndOperators(MainModel* model, FILE* out);

kErrors SimulateModel(MainModel* model, FILE* out);
