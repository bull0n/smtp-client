/* auto-extracted */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>


/* Private function definition */
static FILE *tcp_connect(const char *hostname, const char *port);

/* Public function implementation */

int main(int argc, char **argv) {
   int result = EXIT_FAILURE;

   if (argc == 3) {
      FILE *f;

      if ((f = tcp_connect(argv[1], argv[2]))) {
	char buffer[1024];

	fprintf(f, "GET / \n");
	if(fgets(buffer, sizeof(buffer), f));
	{
	  puts(buffer);
	}
      }
      /* else: failure */
   }
   else {
      fprintf(stderr, "%s remote-host port\n", argv[0]);
      fprintf(stderr, "%s: bad args.\n", argv[0]);
   }

   return result;
}

static FILE *tcp_connect(const char *hostname, const char *port) {
   FILE *f = NULL;

   printf("hostname: %s port %s\n", hostname, port);

   int s;
   struct addrinfo hints;
   struct addrinfo *result, *rp;
   
   hints.ai_family = AF_UNSPEC; /* IPv4 or v6 */
   hints.ai_socktype = SOCK_STREAM; /* TCP */
   hints.ai_flags = 0;
   hints.ai_protocol = 0; /* any protocol */
   
   if ((s = getaddrinfo(hostname, port, &hints, &result))) {
     fprintf(stderr, "getaddrinfo(): failed: %s.\n", gai_strerror(s));
   }
   else {
     /* getaddrinfo(3) retourne une liste d’adresses, essayons chacune
      * jusqu’à ce que la connexion fonctionne!
      */
     for (rp = result; rp != NULL; rp = rp->ai_next) {
       /* ici on peut afficher les adresses, puis créer le socket
	* et tenter une connexion !
	*/
       char ipname[INET_ADDRSTRLEN];
       char servicename[6]; /* "65535\0" */
       if (!getnameinfo(rp->ai_addr,
			rp->ai_addrlen,
			ipname,
			sizeof(ipname),
			servicename,
			sizeof(servicename),
			NI_NUMERICHOST|NI_NUMERICSERV)) {
	 printf("Trying connection to host %s:%s ...\n",
		ipname,
		servicename);

	 /* socket creation */
	 if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol))
	     == -1) {
	   perror("socket() failed: ");
	 }
	 /* connection */
	 if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1) {
	   printf("OK\n");
	   
	   if ((f = fdopen(s, "r+"))) {
	     /* depuis ici, vous pouvez utiliser les fonctions de la libc comme
	      * fprintf(3), fgets(3), fputs(3), ... sur f
	      */
	     break; /* of for */
	   }
	 }
	 else {
	   perror("connect(): ");
	 }
       }       
     }
     freeaddrinfo(result);
   }

   return f;
}

