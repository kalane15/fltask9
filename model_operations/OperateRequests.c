#define _CRT_SECURE_NO_WARNINGS
#include "OperateRequests.h"
#include "time.h"
#include "Generics.h"

int RequestComparator(void* const a, void* const b) {
	Request** r1 = (Request**)a;
	Request** r2 = (Request**)b;
	int res = (int)difftime(_mkgmtime(&(*r1)->req_time_get), _mkgmtime(&(*r2)->req_time_get));
	return res;
}

kErrors ParseRequests(int argsc, char** args, MainModel* model) {
	kErrors status = SUCCESS;
	FILE* cur_file;
	Request* cur_req;
	char* time_data = (char*)malloc(1024);
	char* cur_string = (char*)malloc(4096);
	char* time_time = (char*)malloc(128);
	char* id = (char*)malloc(256);
	char* text = (char*)malloc(4096);
	if (time_data == NULL || cur_string == NULL || time_time == NULL || id == NULL || text == NULL) {
		if (time_data != NULL) {
			free(time_data);
		}
		if (cur_string != NULL) {
			free(cur_string);
		}
		if (time_time != NULL) {
			free(time);
		}
		if (id != NULL) {
			free(id);
		}
		if (text != NULL) {
			free(text);
		}
	}

	int priority;	
	int count = 0;
	int r_id = 0;
	model->pending_requests = (queue_req*)malloc(sizeof(queue_req));
	if (model->pending_requests == NULL) {
		status = MEM_ALLOC_ERR;
	}

	if (status == SUCCESS) {
		bool bres = queue_create(model->pending_requests, 10);
		if (!bres) {
			return MEM_ALLOC_ERR;
		}
	}
	for (int i = 3; i < argsc; i++) {
		if (status != SUCCESS) {
			break;
		}
		cur_file = fopen(args[i], "r");
		if (cur_file == NULL) {
			status = INC_FILE;
			break;
		}
		while (fgets(cur_string, 4096, cur_file) != NULL) {
			count = sscanf(cur_string, "%s %s %d %s ''%s''", time_data, time_time, &priority, id, text);
			if (count != 5) {
				break;
			}
			strcat(time_data, " ");
			strcat(time_data, time_time);
			status = RequestCreate(priority, id, text, time_data, &cur_req, r_id, model);
			r_id++;
			if (status != SUCCESS) {
				break;
			}
			bool s = queue_push(model->pending_requests, cur_req);
			if (!s) {
				status = MEM_ALLOC_ERR;
				break;
			}
		}
		fclose(cur_file);
	}
	qsort(model->pending_requests->buffer, model->pending_requests->size, sizeof(Request*), RequestComparator);
	free(time_data);
	free(cur_string);
	free(time_time);
	free(id);
	free(text);
	return status;
}



kErrors UpdateDepsAndOperators(MainModel* model, FILE* out) {
	Department** deps = model->departments_storage_array;
	kErrors status = SUCCESS;	
	for (int i = 0; i < model->deps_count; i++) {
		for (int op = 0; op < deps[i]->op_count; op++) {
			if (deps[i]->operators[op].is_working) {
				deps[i]->operators[op].operating_time_remain -= 1;
				if (deps[i]->operators[op].operating_time_remain <= 0) {
					fprintf(out, "%s REQUEST_HANDLING_FINISHED req_id: %d, time handling: %d  operator_id: %d\n",
						model->current_time_string, deps[i]->operators[op].current_request->r_id, 
						deps[i]->operators[op].current_request->time_to_operate, op);
					deps[i]->operators[op].is_working = false;
					deps[i]->operators[op].current_request = NULL;
				}
			}
		}	
	}
	return status;
}


kErrors DistributeRequests(MainModel* model, FILE* out) {
	Department** deps = model->departments_storage_array;
	kErrors status = SUCCESS;
	for (int i = 0; i < model->deps_count; i++) {
		if (deps[i]->requests_in_queue > 0) {
			for (int op = 0; op < deps[i]->op_count; op++) {
				if (deps[i]->operators[op].is_working == false) {
					status = GenericDeleteMax(deps[i]->req_queue, &(deps[i]->operators[op].current_request), model);
					if (status != SUCCESS) {
						break;
					}
					deps[i]->operators[op].is_working = true;

					deps[i]->operators[op].operating_time_remain = deps[i]->operators[op].current_request->time_to_operate;
					deps[i]->requests_in_queue--;
					fprintf(out, "%s REQUEST_HANDLING_STARTED req_id: %d operator_num: %d\n",
						model->current_time_string, deps[i]->operators[op].current_request->r_id, op);
				}
			}
		}
	}
	return SUCCESS;
}

bool CheckOperators(MainModel* model) {
	Department** deps = model->departments_storage_array;
	for (int i = 0; i < model->deps_count; i++) {
		for (int op = 0; op < deps[i]->op_count; op++) {
			if (deps[i]->operators[op].is_working) {
				return false;
			}
		}
	}
	return true;
}

kErrors TryToMoveRequests(Department* dep_from, MainModel* model, Department** moved_to) {
	int min = 1e8;
	kErrors status = SUCCESS;
	Department* dep_min_load = NULL;
	Department* cur;
	for (int i = 0; i < model->deps_count; i++) {
		cur = model->departments_storage_array[i];
		if (cur->requests_in_queue >= cur->op_count * cur->overload_coeff) {
			continue;
		}
		else if (cur->op_count * cur->overload_coeff - cur->requests_in_queue < min){
			dep_min_load = model->departments_storage_array[i];
			min = cur->op_count * cur->overload_coeff - cur->requests_in_queue;
		}
	}
	*moved_to = dep_min_load;
	if (dep_min_load != NULL) {
		status = GenericMeldReqPq(dep_from->req_queue, dep_min_load->req_queue, model);
	}
	else {
		return status;
	}

	dep_min_load->requests_in_queue += dep_from->requests_in_queue;
	dep_from->requests_in_queue = 0;
	return status;
}

kErrors SimulateModel(MainModel* model, FILE* out) {
	kErrors status = SUCCESS;
	model->current_time = model->modelling_start;
	Request* cur_req = NULL;
	char* time_string = (char*)malloc(1024);
	if (time_string == NULL) {
		return MEM_ALLOC_ERR;
	}
	model->current_time_string = time_string;
	struct tm* t = (struct tm*)malloc(sizeof(struct tm));
	if (t == NULL) {
		free(time_string);
		return MEM_ALLOC_ERR;
	}
	Department* last_dep_modifyed = NULL;
	Department* dep_moved_to = NULL;
	
	while (true) {		
		gmtime_s(t, &model->current_time);
		strftime(time_string, 1024, "%Y-%m-%d %H:%M:%S", t);
		cur_req = queue_front(model->pending_requests);
		//printf("%s\n", time_string);
		while(model->pending_requests->size > 0 && 
			difftime(model->current_time, _mkgmtime(&cur_req->req_time_get)) >= 0 &&
			model->pending_requests->size > 0) {	

			
			status = GenericInsertRequest(model, cur_req, &last_dep_modifyed);
			fprintf(out, "%s NEW_REQUEST req_id: %d dep_id: %s\n", time_string, cur_req->r_id, cur_req->dep_id);
			if (status != SUCCESS) {
				free(time_string);
				free(t);
				return status;
			}
			
			status = DistributeRequests(model, out);
			if (status != SUCCESS) {
				free(time_string);
				free(t);
				return status;
			}
			if (last_dep_modifyed->requests_in_queue >= last_dep_modifyed->op_count * last_dep_modifyed->overload_coeff) {
				status = TryToMoveRequests(last_dep_modifyed, model, &dep_moved_to);
				if (status != SUCCESS) {
					free(time_string);
					free(t);
					return status;
				}
				if (dep_moved_to != NULL) {
					fprintf(out, "%s, DEPARTMENT_OVERLOADED req_id: %d, requests were moved to dep with id: %s\n",
						model->current_time_string, cur_req->r_id, dep_moved_to->dep_id);
				}
				else {
					fprintf(out, "%s, DEPARTMENT_OVERLOADED req_id: %d, requests cant be moved to other dep\n",
						model->current_time_string, cur_req->r_id);
				}
			}
			queue_pop(model->pending_requests);
			if (model->pending_requests->size == 0) {
				cur_req = NULL;
				break;
			}
			cur_req = queue_front(model->pending_requests);
		}
		status = UpdateDepsAndOperators(model, out);
		if (status != SUCCESS) {
			free(time_string);
			free(t);
			return status;
		}
		model->current_time += 60;

		if (difftime(model->current_time, model->modelling_finished) > 0) {
			break;
		}
	}
	free(t);
	return SUCCESS;
}