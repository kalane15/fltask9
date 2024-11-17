#pragma once
typedef struct Department Department;
typedef struct MainModel MainModel;
#include "time.h"
#include "mylib.h"
#include "OpsAndDeps.h"
#include "Comparators.h"

kErrors ParseDepsInfo(FILE* config, MainModel* model,
	int max_priority, int min_op_work, int max_op_work, int deps_count);
