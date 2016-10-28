#include <stdio.h>
#include "analyzer.hpp"

int main(int argc, char** argv)
{
	if(argc<2) return -1;
	Analyzer analyzer = Analyzer();
	analyzer.Set_video_source(argv[1]);
	analyzer.Set_runtime(300);
	if(argc>2) analyzer.Set_realtest(true);
	analyzer.Initialize();
//	analyzer.Prepare();
	while(analyzer.Test());
	return 0;
}
