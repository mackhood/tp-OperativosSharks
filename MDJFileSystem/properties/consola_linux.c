	#include "consola_linux.h"
	#include "configurar.h"
	//#include "../src/mercadoDeJugadores.h"
	//#include "md5.h"
	#include "configurar.h"
	#include <unistd.h>
	#include <openssl/md5.h>

	void comand() {

	}


	COMANDOS comandos[] = {
			{ "cd", com_cd, "Cambiar el directorio por /DIR" },
			{ "md5", com_md5, "Genera clave md5 del FILE" },
			{ "ayuda", com_ayuda, "Muestra esta ayuda" },
			{ "?", com_ayuda, "Muestra esta ayuda" },
			{ "ls", com_lista, "Lista los archivos del directorio" },
			{ "pwd", com_pwd, "Direcorio actual" },
			{ "salir", com_salir, "Sale de la consola" },
			{ "estado", com_estado, "Muestra el estado del archivo" },
			{ "cat", com_ver, "Ver contenido del FILE" },
			{ (char *) NULL, (Function *) NULL, (char *) NULL }, };

	void logConsola() {
		log_consola = log_create("fifa_comandos.log", "Consola de la FIFA ",
		false, LOG_LEVEL_TRACE);
	}

	char *dupstr(int s) {

		strcpy(r, s);
		return (r);
	}
	void eliminarEstructura(COMANDOS *com){

		int i, j = 0;
		for (j = 0; j < 10; ++j) {
			//free(comandos[i].docExtra);
			comandos[i].docExtra = NULL;
			//free(comandos[i].nombre);
			comandos[i].nombre = NULL;
			//free(comandos[i].funcion);
			comandos[i].funcion = NULL;
		}
		free(comandos);
		while (com[i].nombre != NULL) {
			//free(com[i].nombre);
			com[i].nombre = NULL;
			//free(com[i].funcion);
			com[i].funcion = NULL;
			//free(com[i].docExtra);
			com[i].docExtra = NULL;
			i++;
		};
		com = NULL;
	}


	int ejecutar_linea (char* argumento){
	  register int i = 0;

	  while (argumento[i] && whitespace(argumento[i]))
		i++;
	  word = argumento + i;

	  while (argumento[i] && !whitespace (argumento[i]))
		i++;

	  if (argumento[i])
		  argumento[i++] = '\0';
		//log_info(fifa_log, "%s", argumento[i]);
		comando = buscar_comando(word);

		if (!comando) {
		  log_warning(log_consola,"%s: Faltan comandos para el Mercado de Jugadores. %s \n ",word);
		  return (-1);
		};

	 while (whitespace (argumento[i]))
		i++;

	  word = argumento + i;

		//printf("%s", argumento[i]);
		free(argumento);

		return ((*(comando->funcion))(word));
	}

	COMANDOS* buscar_comando (char *nombre){
	  register int i;

		for (i = 0; comandos[i].nombre; i++)
			if (strcmp(nombre, comandos[i].nombre) == 0)
				return (&comandos[i]);

	  return ((COMANDOS *)NULL);
	}

	char* stripwhite (char* string){
	  register char *s, *t;

	  for (s = string; whitespace(*s); s++);

	  if (*s == 0)
		return (s);

	  t = s + strlen (s) - 1;
	  while (t > s && whitespace(*t))
		t--;
	  *++t = '\0';


		t = NULL;
	  return(s);
	}

	void initialize_readline(){
	//
		rl_readline_name = "terminalFIFA";
		rl_attempted_completion_function = (CPPFunction *) completar_nombre;
	}

	void final_readline() {
		free(rl_readline_name);
		rl_clear_history();
	}
	char **completar_nombre(char* text, int start, int end){
	  char **matches;

	  matches = (char **)NULL;
	  if (start == 0){
		matches = completion_matches(text, comando_generador);
	  };

	  return (matches);
	}

	char* comando_generador (char* text, int state){
	  static int list_index, len;
	  char *nombre;

	  if (!state){
		  list_index = 0;
		  len = strlen(text);
		};

	while (nombre == comandos[list_index].nombre) {
		  list_index++;

		  if (strncmp (nombre, text, len) == 0)
			return (dupstr(nombre));
		};


	  return ((char *)NULL);
	}

	/* **************************************************************** */
	/*                                                                  */
	/*                Mercado de Jugadores: comandos                    */
	/*                                                                  */
	/* **************************************************************** */


	int com_lista (char* arg){
	  if (!arg)
		arg = "";

	  sprintf (syscom, "ls -FClg %s", arg);
		//log_trace(log_consola, "ls -FClg %s", arg);
	  return (system (syscom));
	}

	int com_ver(char* arg){
	  if (!argumentos_validos("cat", arg)){
			log_warning(log_consola, "Faltan argumentos?? \n");
		return (EXIT_FAILURE);
	  }else{
	  sprintf (syscom, "more %s", arg);
	  return (system (syscom));
	  };

	}

	int com_estado (char* arg){
	  struct stat finfo;

	  if (!argumentos_validos("estado", arg))
		return (1);

	  if (stat (arg, &finfo) == -1){
		  perror (arg);
		  return (1);
		}

	  printf ("Estadísticas `%s':\n", arg);
	  printf ("%s has %d link%s, and is %d byte%s in length.\n", arg,
			  finfo.st_nlink,
			  (finfo.st_nlink == 1) ? "" : "s",
			  finfo.st_size,
			  (finfo.st_size == 1) ? "" : "s");
	  log_info(log_consola,"Estadísticas `%s':\n", arg);
	  log_info(log_consola, "%s has %d link%s, and is %d byte%s in length.\n", arg,
		  finfo.st_nlink,
		  (finfo.st_nlink == 1) ? "" : "s",
		  finfo.st_size,
		  (finfo.st_size == 1) ? "" : "s");

	  printf ("\n Inode último cambio at: %s", ctime (&finfo.st_ctime));
	  printf ("\n Último acceso at: %s", ctime (&finfo.st_atime));
	  printf ("\n Última modificación at: %s", ctime (&finfo.st_mtime));

	  log_trace(log_consola,"\n Inode último cambio at: %s", ctime (&finfo.st_ctime));
	  log_trace(log_consola,"\n Último acceso at: %s", ctime (&finfo.st_atime));
	  log_trace(log_consola,"\n Última modificación at: %s", ctime (&finfo.st_mtime));

		//&finfo = NULL;


	  return (0);
	}

	/*int com_delete (char* arg){
	  too_dangerous ("delete");
	  return (1);
	}
	*/
	int com_ayuda (char* arg){
	  register int i;
	  int printed = 0;

		for (i = 0; comandos[i].nombre; i++) {
			if (!*arg || (strcmp(arg, comandos[i].nombre) == 0)) {
				printf("%s\t\t%s.\n", comandos[i].nombre, comandos[i].docExtra);
			  printed++;
			}
		}

	  if (!printed){
		  printf ("Comandos no encontrado `%s'.  Las posibilidades son:\n", arg);

			for (i = 0; comandos[i].nombre; i++) {

			  if (printed == 6){
				  printed = 0;
				  printf ("\n");
				};

				printf("%s\t", comandos[i].nombre);
			  printed++;
			};

		  if (printed)
			printf ("\n");
		}
	  return (0);
	}

	int com_cd(char *arg){
		if (chdir(arg) == -1) {
			perror(arg);

			return (EXIT_FAILURE);
	} else {
		log_debug(log_consola, "Se accedió a %s", arg);
	}
	return (system(" pwd"));
	}

	/* Print out the current working directory. */
	int com_pwd(char* i) {
	dir = "pwd ";

	return (system(dir));
	}

	int com_salir (char* arg){
		done = 1;
		imRun = false;
		morir();
		//c;hauMemoria();
	  return (0);
	}
	int com_md5(void* contenido) {

		//int md5;
		//char* path = argv;
	char* digest = malloc(MD5_DIGEST_LENGTH);
		MD5_CTX context;
		MD5_Init(&context);
		MD5_Update(&context, contenido, strlen(contenido) + 1);
		MD5_Final(digest, &context);

		free(contenido);
		log_debug(log_consola, "MD5: %s", digest);

		printf("MD5: %s\n", digest);

	system("md5");
		free(digest);
		return (0);
	}


	int too_dangerous (char* caller){
	  fprintf (stderr, "%s: Too dangerous for me to distribute.  Write it yourself.\n", caller);
	  log_warning(log_consola,"%s: Peligro!!! Peligro!!!  Write it yourself.\n", caller);
	  return(1);
	}


	int argumentos_validos(char *caller, char* arg){
	  if (!arg || !*arg){
		  fprintf (stderr, "%s: Se requiere argumento.\n", caller);
		  log_warning(log_consola, "%s: Faltan Agumentos \n", caller);
		  return (0);
		}

	  return (1);
	}

