#ifndef _W18_QUERY_WINDOW_WIN_H_
#define _W18_QUERY_WINDOW_WIN_H_
#ifdef W18_QUERY_WINDOW_WIN_PRIVATE
	#define W18_QUERY_WINDOW_WIN_PUBLIC
#else
	#define W18_QUERY_WINDOW_WIN_PUBLIC extern
#endif

#define W18_QUERY_WINDOW_WIN	18
void W18_QueryWindowDisplay(void);
void W18_QueryWindowProcess(void);

//用户定义



#endif

