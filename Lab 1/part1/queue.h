
// Maximum number of elements in a queue
//
#define MAX_Q_SIZE	10

// Place function prototypes here.

void enq(double);
double deq();


void sum(double x);

void prod(double x);

void clear_sum();

void clear_prod();

double reduce();

double flex_reduce(void (*)(), void (*)(double));