#include <stdio.h>
#include "analyzer.hpp"

int main()
{
	Analyzer analyzer = Analyzer();
	analyzer.Set_video_source("test.avi");
	analyzer.Initialize();
	analyzer.Prepare();
	while(analyzer.Test());
	return 0;
}
