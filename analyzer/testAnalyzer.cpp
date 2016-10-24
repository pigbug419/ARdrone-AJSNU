#include <stdio.h>
#include "analyzer.hpp"

int main(int argc, char** argv)
{
	if(argc<2) return -1;
	Analyzer analyzer = Analyzer();
	analyzer.Set_video_source(argv[1]);
	analyzer.Initialize();
//	analyzer.Prepare();
	while(analyzer.Run2());
	analyzer.SendCommand(LAND);
	return 0;
}
