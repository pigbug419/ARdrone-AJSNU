#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analyzer.hpp"

int main(int argc, char** argv)
{
	if(argc<2) return -1;
	Analyzer analyzer = Analyzer();
	analyzer.Set_runtime(atoi(argv[1]));
	analyzer.Initialize();
	analyzer.Prepare();
	while(analyzer.Run());
	return 0;
}
