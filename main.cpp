#include <iostream>
#include <time.h>
#include <math.h>

using namespace std;

#define TOP_LEFT "\u250c"
#define UP_RIGHT "\u2510"
#define BOTTOM_LEFT "\u2514"
#define BOTTOM_RIGHT "\u2518"
#define VERTICAL_LEFT "\u251c"
#define VERTICAL_RIGHT "\u2524"
#define HORIZONTAL_TOP "\u252c"
#define HORIZONTAL_BOTTOM "\u2534"
#define CROSS "\u253c"
#define VERTICAL "\u2502"
#define HORIZONTAL "\u2500"
#define SQUARE "\u2588"
#define STEP_Q 0.035


class FuncInfo {
    double param_a, param_b, step_dx, lower_x1, upper_x2;
    int num_steps;
    double *all_fx;
    int *normalized_fx;
public:
    FuncInfo(double a, double b, double dx, double x1, double x2);
    ~FuncInfo() { delete[] all_fx; delete[] normalized_fx; }
    double get_fx(double x);
    double get_s1();
    double get_s2();
    int get_num_steps() { return num_steps; }
    void Tab();
    void Print();
};

void printN(const char *s, int n);
double random(double lower, double upper, double delta);
void printTableTop(int n_columns, int column_sizes[]);
void printTableMiddle(int n_columns, int column_sizes[]);
void printTableBottom(int n_columns, int column_sizes[]);


int main() {
    setlocale(LC_ALL, "");
    srand(time(NULL));

    FuncInfo info = FuncInfo(2.0, 12.0, 0.5, -5.0, 5.0);
    info.Tab();

    double s1 = info.get_s1(), s2 = info.get_s2();

    int size = info.get_num_steps();
    double lower, upper, delta;
    if (s1 < s2) {
        lower = s1;
        upper = s2;
    } else {
        lower = s2;
        upper = s1;
    }
    delta = upper * STEP_Q;

    int column_sizes[] = {5, 10};
    cout << endl;
    printTableTop(2, column_sizes);
    printf("%s %-3s %s %8.3f %s\n", VERTICAL, "S1", VERTICAL, s1, VERTICAL);
    printTableMiddle(2, column_sizes);
    printf("%s %-3s %s %8.3f %s\n", VERTICAL, "S2", VERTICAL, s2, VERTICAL);
    printTableMiddle(2, column_sizes);
    printf("%s %-4s %s %8.3f %s\n", VERTICAL, "δ", VERTICAL, delta, VERTICAL);
    printTableMiddle(2, column_sizes);
    printf("%s rnd %s %8.3f %s\n", VERTICAL, VERTICAL, random(lower, upper, delta), VERTICAL);
    for (int i = 1; i < size; i++) {
        printf("%s     %s %8.3f %s\n", VERTICAL, VERTICAL, random(lower, upper, delta), VERTICAL);
    }

    printTableBottom(2, column_sizes);

    cout << endl;
    info.Print();

    return EXIT_SUCCESS;
}


FuncInfo::FuncInfo(double a, double b, double dx, double x1, double x2) {
    param_a = a;
    param_b = b;
    step_dx = dx;
    lower_x1 = x1;
    upper_x2 = x2;

    double x = lower_x1;
    num_steps = 0;
    while (x <= upper_x2) {
        x += step_dx;
        num_steps++;
    }

    double current, min, max;
    x = lower_x1;
    all_fx = new double[num_steps];
    min = get_fx(x);
    max = min;
    for (int i = 0; i < num_steps; i++) {
        current = get_fx(x);
        all_fx[i] = current;
        if (current < min) {
            min = current;
        } else if (current > max) {
            max = current;
        }
        x += step_dx;
    }

    normalized_fx = new int[num_steps];
    double norm_quota = 26 / (max - min);
    for (int i = 0; i < num_steps; i++) {
        normalized_fx[i] = int(round((all_fx[i] - min) * norm_quota));
    }
}


double FuncInfo::get_fx(double x) {
    if (x <= 1.0) return param_a * x * x + param_b;
    return (param_b - 2) / x;
}


double FuncInfo::get_s1() {
    double res = 0;
    for (int i = 0; i < num_steps; i++) {
        double fx = all_fx[i];
        if (fx > 0) res += fx;
    }
    return res;
}


double FuncInfo::get_s2() {
    double sum = 0;
    int i, num;
    for (i = 0, num = 0; i < num_steps; i += 2, num++) {
        double fx = all_fx[i];
        sum += fx;
    }
    return sum / num;
}


void FuncInfo::Tab() {
    int column_sizes[] = {8, 8, 13, 8};
    printTableTop(4, column_sizes);
    printf("%s %-6s %s %-6s %s %-11s %s %-6s %s\n",
           VERTICAL, "x", VERTICAL, "N", VERTICAL, "f(x)", VERTICAL, "Normal", VERTICAL);

    double x = lower_x1;
    for (int N = 0; N < num_steps; N++) {
        printTableMiddle(4, column_sizes);
        printf("%s %-6.1f %s %-6d %s %-11.5f %s %-6d %s\n",
               VERTICAL, x, VERTICAL, N + 1, VERTICAL, all_fx[N], VERTICAL, normalized_fx[N], VERTICAL);
        x += step_dx;
    }

    printTableBottom(4, column_sizes);
}


void FuncInfo::Print() {
    for (int i = 0; i < num_steps; i++) {
        printN(SQUARE, normalized_fx[i] + 1);
        cout << endl;
    }
}


void printN(const char *s, int n) {
    for (int i = 0; i < n; i++) {
        cout << s;
    }
}


double random(double lower, double upper, double delta) {
    auto size = int((upper - lower) / delta);
    return lower + (rand() % size) * delta;
}


void printTableTop(int n_columns, int column_sizes[]) {
    int i;
    cout << TOP_LEFT;
    for (i = 0; i < n_columns - 1; i++) {
        printN(HORIZONTAL, column_sizes[i]);
        cout << HORIZONTAL_TOP;
    }
    printN(HORIZONTAL, column_sizes[i]);
    cout << UP_RIGHT;
    cout << endl;
}


void printTableMiddle(int n_columns, int column_sizes[]) {
    int i;
    cout << VERTICAL_LEFT;
    for (i = 0; i < n_columns - 1; i++) {
        printN(HORIZONTAL, column_sizes[i]);
        cout << CROSS;
    }
    printN(HORIZONTAL, column_sizes[i]);
    cout << VERTICAL_RIGHT;
    cout << endl;
}


void printTableBottom(int n_columns, int column_sizes[]) {
    int i;
    cout << BOTTOM_LEFT;
    for (i = 0; i < n_columns - 1; i++) {
        printN(HORIZONTAL, column_sizes[i]);
        cout << HORIZONTAL_BOTTOM;
    }
    printN(HORIZONTAL, column_sizes[i]);
    cout << BOTTOM_RIGHT;
    cout << endl;
}
