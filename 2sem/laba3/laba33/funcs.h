#include "funcs.c"

void save(char *fname, ITCOMPANY *exemplar, int size);
int struct_count_file();
void load(char *fname, ITCOMPANY **exemplar, int *size);

int input_int();
void input_string(char *str);
void record_name(ITCOMPANY *exemplar);
void record_day_deadline(ITCOMPANY *exemplar);
void record_month_deadline(ITCOMPANY *exemplar);
void record_year_deadline(ITCOMPANY *exemplar);
void record_cost(ITCOMPANY *exemplar);
void record_status(ITCOMPANY *exemplar);
void new_record(ITCOMPANY *exemplar);
void print_exemplar(ITCOMPANY *exemplar);
int find_deadline(int year, int month, int day);

int sort_condition(ITCOMPANY a, ITCOMPANY b, int condition);
int equal_condition(ITCOMPANY a, ITCOMPANY b, int condition);
int other_conditions(ITCOMPANY a, ITCOMPANY b, int *condition, int count_conditions);
void sorting_first(ITCOMPANY *exemplar, int count, int condition);
void sorting_second(ITCOMPANY *exemplar, int count, int *condition, int count_conditions);