// DEV : YOANN LEHMANN, LUCAS BULLONI
// PROGRAM : SIMPLE SMTP CLIENT
// DATE : MAY/JUNE 2017
// DESCRIPTION : Send an email through smtp with all the details in the command parameters
// SOURCES : TCP part from Marc Schaefer : https://ssl.horus.ch/~schaefer/bin/view/HEArc/ReseauxINF1ProgrammationSocketTCPCorrige

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

/* STUDENT_START */
#define LOW_LEVEL_DEBUG /* plateform-specific spying */
/* STUDENT_END */

/* Private function definition */
static FILE *tcp_connect(const char *hostname, const char *port);

/* Public function implementation */

int main(int argc, char **argv) {
  int result = EXIT_FAILURE;

  if (argc > 5) {
    FILE *f;


    //Infos for mails
    char* from = argv[1];
    char* subject = argv[2];
    char* filename = argv[3];
    char* to = argv[5];
    //Infos to send to the server
    char* ipAddr;
    //Infos to connect to the server
    char* mailServer = argv[4];
    char* portNum;


    if(argc > 6) {
      portNum = argv[6];
    }
    else {
      portNum = "25";
    }

    if ((f = tcp_connect(mailServer, portNum))) {
      char buffer[1024];

      /* nothing to send anymore  MOVING IT AT THE END*/
      //shutdown(fileno(f), SHUT_WR);

      //get the first reply of the server
      fgets(buffer, sizeof(buffer), f);


      //check if the server accept the connection
      if(buffer[0] == '2') {
        /*Start the sending email process*/
        // Number of the step
        // 0 = HELO
        // 1 = MAIL FROM
        // 2 = RCPT TO
        // 3 = DATA
        // 4 = QUIT
        int step = 0;

        for(step = 0; step < 5; i++) {
          switch(step) {
            case 0:
            fprintf(f, "HELO %s\r\n", ipaddr);
            break;
            case 1:
            fputs("HELO \r\n", f);
            break;
            case 2:
            fputs("HELO \r\n", f);
            break;
            case 3:
            fputs("HELO \r\n", f);
            break;
            case 4:
            fputs("QUIT\r\n", f);
            break;
            default:

          }
          fflush(f);
          fgets(buffer, sizeof(buffer), f);
        }
      }
      else {

      }



      /* retrieving answer from HTTP server */
      while (fgets(buffer, sizeof(buffer), f)) {
        if (strlen(buffer) > 0) {
          /* filter out \n */
          buffer[strlen(buffer) - 1] = '\0';
        }
        puts(buffer);
      }

      if (fclose(f) == 0) {
        /* this also closes the underlying socket and thus
        * drops the connection
        */
        f = NULL;

        result = EXIT_SUCCESS;
      }
      else {
        perror("fclose(): failed: ");
      }
      /* STUDENT_END */
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

  /* STUDENT_START */
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
    /*  getaddrinfo() returns a list of address structures.
    *  Try each address until we successfully connect(2).
    */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
      char ipname[INET_ADDRSTRLEN];
      char servicename[6]; /* "65535\0" */

      /* socket creation */
      if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol))
      == -1) {
        perror("socket() failed: ");
        continue; /* next loop */
      }

      #ifdef LOW_LEVEL_DEBUG
      /* this is one of the reasons why you shouldn't use the older
      * level library calls such as gethostbyname(3), the others being
      * security and multithreading
      */
      if (rp->ai_family == AF_INET) {
        printf("HACK: raw port number: %d\n",
        ((struct sockaddr_in *) rp->ai_addr)->sin_port);
      }
      #endif /* LOW_LEVEL_DEBUG */

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
        }

        if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1) {
          /* from now on, read(2), write(2), shutdown(2) and close(2)
          * can be called on that socket (file descriptor) s -- note that
          * as for every character device, write(2) might not write
          * all we want, and read(2) will return even if not all bytes
          * are received: this is one of the reason we will promote
          * the socket into a fully-equipped libc FILE *.
          */

          break; /* end of loop */
        }
        else {
          perror("connect(): ");
        }

        close(s); /* err. ign. */
      }

      freeaddrinfo(result);

      if (rp == NULL) {
        fprintf(stderr, "Could not connect.\n");
      }
      else {
        /* associate the file descriptor to a C library file, to make
        * things easier for us
        */
        if (!(f = fdopen(s, "r+"))) {
          perror("fdopen() failed: ");
        }
      }
    }

    /* STUDENT_END */

    return f;
  }
