#include "InputParsing.h"
#include "ParseDeps.h"

kErrors SwitchReqStoreType(char* s, ReqStoreType* req_store_type) {
	if (strcmp(s, "BINARY_HEAP") == 0) {
		*req_store_type = BINARY_HEAP;
		return SUCCESS;
	}
	if (strcmp(s, "BINOMIAL_HEAP") == 0) {
		*req_store_type = BINOMIAL_HEAP;
		return SUCCESS;
	}
	if (strcmp(s, "FIB_HEAP") == 0) {
		*req_store_type = FIB_HEAP;
		return SUCCESS;
	}
	if (strcmp(s, "SKEW_HEAP") == 0) {
		*req_store_type = SKEW_HEAP;
		return SUCCESS;
	}
	if (strcmp(s, "LEFTIST_HEAP") == 0) {
		*req_store_type = LEFTIST_HEAP;
		return SUCCESS;
	}
	if (strcmp(s, "TREAP") == 0) {
		*req_store_type = TREAP;
		return SUCCESS;
	}
	return INC_REQ_STORE_TYPE;
}

kErrors SwitchDepStoreType(char* s, DepStoreType* req_store_type) {
	if (strcmp(s, "HASH_SET") == 0) {
		*req_store_type = HASH_SET;
		return SUCCESS;
	}
	if (strcmp(s, "DYNAMIC_ARRAY") == 0) {
		*req_store_type = DYNAMIC_ARRAY;
		return SUCCESS;
	}
	if (strcmp(s, "BINARY_SEARCH_TREE") == 0) {
		*req_store_type = BINARY_SEARCH_TREE;
		return SUCCESS;
	}
	if (strcmp(s, "TRIE") == 0) {
		*req_store_type = TRIE;
		return SUCCESS;
	}
	return INC_DEP_STORE_TYPE;
}

kErrors ParseTime(FILE* config, char* temp, time_t* model_start, time_t* model_finished) {
	struct tm start;
	struct tm finish;


	int count = fscanf(config, "%[^\n]", temp);
	fgetc(config);
	if (count != 1) {
		return INC_INP_DATA;
	}

	start = ConvertToTm(temp);
	count = fscanf(config, "%[^\n]", temp);
	fgetc(config);
	if (count != 1) {
		return INC_INP_DATA;
	}
	finish = ConvertToTm(temp);

	*model_start = _mkgmtime(&start);
	*model_finished = _mkgmtime(&finish);
	if (*model_finished == -1 || *model_start == -1) {
		return INC_TIME;
	}
	return SUCCESS;
}

kErrors ParseOpsAndDepsCount(FILE* config, char* temp, int* min_op_work, int* max_op_work, int* deps_count) {
	kErrors status = SUCCESS;

	int count;
	count = fscanf(config, "%s", temp);
	if (count != 1) {
		return INC_INP_DATA;
	}
	status = StringToInt(temp, min_op_work);
	if (status != SUCCESS) {
		return status;
	}
	count = fscanf(config, "%s", temp);
	if (count != 1) {
		return INC_INP_DATA;
	}
	status = StringToInt(temp, max_op_work);

	count = fscanf(config, "%s", temp);
	if (count != 1) {
		return INC_INP_DATA;
	}
	status = StringToInt(temp, deps_count);
	fgetc(config);
	return SUCCESS;
}

kErrors ParseInput(int argsc, char** args, int* max_priority, ReqStoreType* req_store_type, DepStoreType* dep_store_type, 
	time_t* modelling_start, time_t* modelling_finished, 
	int* min_op_work, int* max_op_work, int* deps_count, MainModel* model){
	if (argsc < 4) {
		return INC_NUM_OF_ARGS;
	}
	kErrors status = SUCCESS;
	status = StringToInt(args[1], max_priority);
	if (status != SUCCESS) {
		return status;
	}
	FILE* config = fopen(args[2], "r");
	if (config == NULL) {
		return INC_FILE;
	}
	
	char* temp = (char*)malloc(1024);
	if (temp == NULL) {
		fclose(config);
		return MEM_ALLOC_ERR;
	}
	int count = fscanf(config, "%[^\n]", temp);
	fgetc(config);
	if (count != 1) {
		fclose(config);
		free(temp);
		return INC_INP_DATA;
	}
	status = SwitchReqStoreType(temp, req_store_type);
	if (status != SUCCESS) {
		fclose(config);
		free(temp);
		return status;
	}

	count = fscanf(config, "%[^\n]", temp);
	fgetc(config);
	if (count != 1) {
		fclose(config);
		free(temp);
		return INC_INP_DATA;
	}
	status = SwitchDepStoreType(temp, dep_store_type);
	if (status != SUCCESS) {
		fclose(config);
		free(temp);
		return status;
	}
	//%Y-%m-%d
	model->dep_store_type = *dep_store_type;
	model->req_store_type = *req_store_type;

	status = ParseTime(config, temp, modelling_start, modelling_finished);
	if (status != SUCCESS) {
		fclose(config);
		free(temp);
		return INC_INP_DATA;
	}
	
	model->modelling_start = *modelling_start;
	model->modelling_finished = *modelling_finished;
	status = ParseOpsAndDepsCount(config, temp, min_op_work, max_op_work, deps_count);
	if (status != SUCCESS) {
		fclose(config);
		free(temp);
		return status;
	}
	
	status = ParseDepsInfo(config, model, *max_priority, *min_op_work, *max_op_work, *deps_count);
	free(temp);
	fclose(config);	
	return status;
}