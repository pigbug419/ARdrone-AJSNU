#include <stdio.h>
#include <string.h>
#include "analyzer.hpp"

int main(int argc, char** argv)
{
    bool flag = false;
//	if(argc<2) return -1;
	Analyzer analyzer = Analyzer();
//	analyzer.Set_video_source(argv[1]);
	analyzer.Set_runtime(300);
	if(argc>1) analyzer.Set_realtest(true);
	analyzer.Initialize();
	if(argc>1){
        analyzer.Prepare();
        if(strcmp(argv[1],"run") == 0){
            flag = true;
            if(argc>2) analyzer.Set_runtime(atoi(argv[2]));
            else analyzer.Set_runtime(20);
        }
    }
	if(!flag) while(analyzer.Test());
    else while(analyzer.Run()); 
	return 0;
}
