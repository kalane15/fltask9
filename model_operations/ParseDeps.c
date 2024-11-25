#define _CRT_SECURE_NO_WARNINGS
#include "string.h"
#include "OpsAndDeps.h"
#include "Generics.h"

kErrors ParseDepsInfo(FILE* config, MainModel* model,
	int max_priority, int min_op_work, int max_op_work, int deps_count) {
	kErrors status;
	model->max_op_work = max_op_work;
	model->min_op_work = min_op_work;
	model->max_priority = max_priority;	
	model->deps_count = deps_count;
	model->departments_storage_structure = GenericMallocDepStruct(model);
	status = GenericCreateDepStruct(model->departments_storage_structure, model);
	if (status != SUCCESS) {
		return status;
	}
	int count;
	char buffer[1024];

	int temp_overload_coeffs[50];
	int temp_op_count[50];
	
	fgets(buffer, 1024, config);
	int i = 0;
	char* cur_lex = strtok(buffer, " ");
	while (cur_lex != NULL) {
		if (cur_lex[strlen(cur_lex) - 1] == '\n') {
			cur_lex[strlen(cur_lex)-1] = '\0';
		}
		status = StringToInt(cur_lex, &temp_op_count[i++]);
		if (status != SUCCESS) {
			return status;
		}
		cur_lex = strtok(NULL, " ");
	}

	if (i != deps_count) {
		return INC_NUM_OF_ARGS;
	}

	i = 0;
	fgets(buffer, 1024, config);
	cur_lex = strtok(buffer, " ");
	while (cur_lex != NULL) {
		if (cur_lex[strlen(cur_lex) - 1] == '\n') {
			cur_lex[strlen(cur_lex) - 1] = '\0';
		}
		status = StringToInt(cur_lex, &temp_overload_coeffs[i++]);
		if (status != SUCCESS) {
			return status;
		}
		cur_lex = strtok(NULL, " ");
	}

	if (i != deps_count) {
		return INC_NUM_OF_ARGS;
	}

	Department* cur_dep;
	char* temp_id = (char*)malloc(1024);
	if (temp_id == NULL) {
		return MEM_ALLOC_ERR;
	}
	model->departments_storage_array = (Department**)malloc(sizeof(Department*) * deps_count);

	for (int ind = 0; ind < deps_count; ind++) {
		sprintf(temp_id, "%d", ind);
		status = DepCreate(temp_op_count[ind], temp_overload_coeffs[ind], &cur_dep, temp_id, model);
		if (status != SUCCESS) {
			return status;
		}
		
		GenericInsert(model->departments_storage_structure, cur_dep, temp_id, model);
		model->departments_storage_array[ind] = cur_dep;
	}
	free(temp_id);
	return SUCCESS;
}