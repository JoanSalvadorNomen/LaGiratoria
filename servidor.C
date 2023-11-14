#include <mysql.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>


typedef struct {
	
	char Nombre[20];
	int Socket;
	
}Conectado;

typedef struct {
	
	Conectado Conectados[100];
	int Num;
	
}ListaConectados;

ListaConectados miLista;
int sockets[100];
int SocketsCreados;
//Acceso excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int AnadirConectado(ListaConectados *Lista, char nombre[80], int Socket)
{
	if (Lista->Num == 100)
	{
		return -1;
	}
	else
	{
		strcpy(Lista->Conectados[Lista->Num].Nombre, nombre);
		Lista->Conectados[Lista->Num].Socket = Socket;
		Lista->Num++;
		return 0;
	}
}
int Posicion(ListaConectados *Lista, char nombre[80])
{	
	for (int i = 0; i < Lista->Num; i++)
	{
		if (strcmp(Lista->Conectados[i].Nombre, nombre) == 0)
		{
			return i;
		}
	}
	return -1;
}
int Eliminar(ListaConectados *Lista, char nombre[80])
{
	int Posicionp = Posicion(Lista, nombre);

	if (Posicionp != -1)
	{
		for (int i = Posicionp; i < Lista->Num; i++)
		{
			Lista->Conectados[i] = Lista->Conectados[i+1];
		}
		
		Lista->Num = (Lista->Num)-1;
		return 0;
	}
	else
	{
		return -1;
	}
}

int BuscarSocket(ListaConectados *Lista, char nombre[80])
{
	int Encontrado = 0;
	int i;
	while (Encontrado != 1 && Lista->Num > i)
	{
		if (strcmp(Lista->Conectados[i].Nombre, nombre) == 0)
		{
			Encontrado = 1;
		}
		
		else
		{
			i++;
		}
	}
	
	if (Encontrado == 1)
	{
		return Lista->Conectados[i].Socket;
	}
	else
	{
		return -1;
	}
}

void ListaActivos(ListaConectados *Lista, char ListaResultado[1000])
{
	pthread_mutex_lock(&mutex);
	sprintf(ListaResultado, "%d", Lista->Num);
	
	for(int i = 0; i < Lista->Num; i++)
	{
		strcat(ListaResultado, ",");
		strcat(ListaResultado, Lista->Conectados[i].Nombre);
	}
	pthread_mutex_unlock(&mutex);
}

void *AtenderCliente (void *socket)
{
	int sock_conn;
	int *s;
	s= (int *) socket;
	sock_conn= *s;
	//int socket_conn = * (int *) socket;
	
	char peticion[512];
	char respuesta[512];
	int ret;
	
	char consulta[512];
	
	MYSQL *conn;
	
	int err;
	// Estructura especial para almacenar resultados de consultas 
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	conn = mysql_init(NULL);
	if (conn==NULL) {
		printf ("Error al crear la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "M5",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	printf ("Inicializa la base de datos\n");
	// INICIALITZACIONSprintf ("Inicializa la base de datos\n");
	
	int terminar =0;
	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	while (terminar ==0)
	{
		// Ahora recibimos la petici?n
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		
		printf ("Peticion: %s\n",peticion);
		
		// vamos a ver que quieren
		char *p = strtok( peticion, "/");
		int codigo =  atoi (p);
		// Ya tenemos el c?digo de la petici?n
		char nombre[20];
		char contrasena[20];
		char fecha[20];
		if (codigo ==4)
		{
			p = strtok( NULL, "/");
			
			strcpy (nombre, p);
			p = strtok ( NULL, "/");
			strcpy (contrasena, p);	
			printf ("nombre: %s , contrasena: %s\n", nombre, contrasena);
			sprintf (consulta, "SELECT Jugador.Contrasena FROM Jugador WHERE Jugador.Nombre = '%s'", nombre);
			printf("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			resultado = mysql_store_result (conn);
			row = mysql_fetch_row(resultado);
			printf("La contrasena es: %s\n", row[0]);
			if (err !=0){
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "No");
				exit(1);
			}
			else if (strcmp (row[0], contrasena) != 0){
				strcpy (respuesta, "No");
				printf("Contrase\ufff1a incorrecta\n");
			}
			else if (strcmp (row[0], contrasena) == 0){
				strcpy (respuesta, "Si");
				printf("Estas conectado\n");
			}
			
			printf ("Voy a enviar respuesta\n");
			write (sock_conn,respuesta, strlen(respuesta));
			printf ("Ya he enviado\n");
			int socketj = BuscarSocket(&miLista, nombre);
			int anadido = AnadirConectado(&miLista, nombre, socketj);
			printf("Jugadores: %d\n", miLista.Num);
			if ( miLista.Num > 0)
			{
				char TablaJugadoresConectados[1000];
				ListaActivos(&miLista, TablaJugadoresConectados);
				sprintf(respuesta, "6/%s", TablaJugadoresConectados);
				
				for (int i = 0; i < (SocketsCreados-1); i++)
				{
					write(sockets[i], respuesta, strlen(respuesta));
				}
			}
		}
		
		if (codigo ==0){
			
			
			char jugador[80];
			p = strtok(NULL, "/");
			strcpy (jugador, p); //Ya tenemos el usuario
			Eliminar(&miLista, jugador);
			sprintf(respuesta, "0/%s", jugador);
			printf("Desconectamos el usuario %s\n", jugador);

			if (miLista.Num != 0 )
			{
				char TablaJugadoresConectados[1000];
				ListaActivos(&miLista, TablaJugadoresConectados);
				sprintf(respuesta, "6/%s", TablaJugadoresConectados);
				
				for (int i = 0; i < SocketsCreados; i++)
				{
					write(sockets[i], respuesta, strlen(respuesta));
				}
			}
			terminar = 1;
			// Se acabo el servicio para este cliente
			close(sock_conn); 
			
		}
		
		
		else if (codigo ==1){
			strcpy (respuesta, ""); 
			sprintf (consulta, "SELECT Jugador.Nombre FROM Jugador ");
			printf ("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			if (err !=0){
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "No hay jugadores");
				exit(1);
			}
			else{
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row(resultado);
				if (row == NULL){
					printf("Error al consultar la base de datos %u %s\n");
					strcpy (respuesta, "No hay jugadores");
				}
				else{
					while (row != NULL){
						sprintf(respuesta, "%s %s,", respuesta, row[0]);
						row = mysql_fetch_row(resultado);
						
					}
					respuesta [strlen(respuesta) - 1] = '\0';
					printf("%s\n", respuesta);
					
				}
				
			}
			printf("Mensaje enviado\n");
			write (sock_conn,respuesta, strlen(respuesta));
			
		}
		
		else if (codigo ==2){
			strcpy (respuesta, ""); 
			sprintf (consulta, "SELECT Historial.Partida FROM Jugador, Historial WHERE Jugador.Nombre = '%s' AND Jugador.IdJ = Historial.Jugador ", nombre);
			printf ("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			if (err !=0){
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "No");
				exit(1);
			}
			else{
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row(resultado);
				if (row == NULL){
					printf("Error al consultar la base de datos %u %s\n");
					strcpy (respuesta, "No jugaste ninguna partida");
				}
				else{
					while (row != NULL){
						sprintf(respuesta, "%s %s,", respuesta, row[0]);
						row = mysql_fetch_row(resultado);
					}
					respuesta [strlen(respuesta) - 1] = '\0';
					printf("%s\n", respuesta);
				}
			}
			printf("Mensaje enviado\n");
			write (sock_conn,respuesta, strlen(respuesta));
		}	
		else if (codigo ==3){
			strcpy (respuesta, ""); 
			p = strtok( NULL, "/");
			strcpy (nombre, p);
			p = strtok ( NULL, "/");
			strcpy (fecha, p);	
			printf ("nombre: %s , fecha: %s\n", nombre, fecha);
			sprintf (consulta, "SELECT Historial.Ganancias FROM Jugador, Partida, Historial WHERE Jugador.Nombre = '%s' AND Partida.Fecha = '%s' AND Jugador.IdJ = Historial.Jugador AND Partida.IdP = Historial.Partida", nombre, fecha);
			printf ("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			if (err !=0){
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "No");
				exit(1);
			}
			else{
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row(resultado);
				if (row == NULL){
					printf("Error al consultar la base de datos %u %s\n");
					strcpy (respuesta, "Aun no has ganado nada, apuesta mas!");
				}
				else{
					sprintf(respuesta, "%s %s,", respuesta, row[0]);
					row = mysql_fetch_row(resultado);
					
				}
				respuesta [strlen(respuesta) - 1] = '\0';
				printf("%s\n", respuesta);
				
			}
			printf("Mensaje enviado\n");
			write (sock_conn,respuesta, strlen(respuesta));
		}
		else if (codigo ==5){
			char TablaJugadoresConectados[1000];
			ListaActivos(&miLista, TablaJugadoresConectados);
			printf("Jugadores: %s\n", TablaJugadoresConectados);
			sprintf(respuesta, "%s", TablaJugadoresConectados);
			printf ("Respuesta: %s\n", respuesta);
			write (sock_conn, respuesta, strlen(respuesta));
			
		}

	}
	mysql_close (conn);	

}

int main(int argc, char *argv[]) {
	
	
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	// Fem el bind al port
	
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// establecemos el puerto  escucha
	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	
	int i;
	int sockets[100];
	pthread_t thread;
	i=0;
	// Bucle para atender a 5 clientes
	for (;;){
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		
		sockets[i] =sock_conn;
		//sock_conn es el socket que usaremos para este cliente
		
		// Crear thead y decirle lo que tiene que hacer
		
		pthread_create (&thread, NULL, AtenderCliente,&sockets[i]);
		i=i+1;
		
	}
	
}
