/* strchr example */
#include <stdio.h>
#include <string.h>

int main ()
{
  char req[] = "http://www.cmu.edu:8765/hub/index.html";
  char reqwithoutport[] = "http://www.cmu.edu/hub/index.html";
  char filename[100], serverhostname[100], serverport[100];
  
//	sscanf(req, "%*[^:]%*3c%[^:]%*c%[^/]%*c%s", serverhostname, serverport, filename);
	sscanf(reqwithoutport, "%*[^:]%*3c%[^/]%*c%s", serverhostname, filename);
//   sscanf(bb, "Host: %s:%s", serverhostname, serverport);
  
//   printf(
//         "[DEBUG] received the request, the hostname is %s, the port is : %s\n",
//         serverhostname, serverport);
        
//   char tmp[] = "\r\nabc";
  // printf("serverhostname: %s, serverport: %s, filename: %s\n", serverhostname, serverport, filename);
  printf("serverhostname: %s, filename: %s\n", serverhostname, filename);
  return 0;
}
