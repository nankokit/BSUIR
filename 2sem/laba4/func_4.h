#include "func_4.c"

void log_record(int event, ITCOMPANY exemplar);
int input_int();
void input_string(char *str);
int input_bord(int more, int less);

void record_name(ITCOMPANY *exemplar);
void record_date(DATEE *exemplar);
void record_date_deadline(ITCOMPANY *exemplar);
void record_cost(ITCOMPANY *exemplar);
void record_status1(STATUS1 *exemplar);
void record_status2(STATUS2 *exemplar);
void record_status(ITCOMPANY *exemplar);
void new_record(ITCOMPANY *exemplar, int last_id);
void print_stage(STAGES stage);
void print_exemplar(ITCOMPANY *exemplar);
void push(LIST **head, ITCOMPANY new_exemplar, int *count);
LIST *getLast(LIST *head);
void pushBack(LIST **head, ITCOMPANY new_exemplar, int *count);
void insert(LIST **tail, ITCOMPANY new_exemplar, int *count);
ITCOMPANY pop(LIST *head, int *count);
void print_list(LIST *head);
ITCOMPANY *go_to_ex(LIST *head, int num);
void modification(LIST *head, int count);
void deletion(LIST **head, int count);
int sort_date(DATEE a, DATEE b);
int sort_condition(ITCOMPANY a, ITCOMPANY b, int condition);
LIST *sort(LIST *head, int condition);