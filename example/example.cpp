#include "gnuplot.h"

int main(){
    GnuplotPipe gp;
    gp.sendLine("plot [-pi/2:pi] cos(x),-(sin(x) > sin(x+1) ? sin(x) : sin(x+1))");
    return 0;
}
