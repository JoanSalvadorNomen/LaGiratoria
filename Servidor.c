#include <mysql.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>




typedef struct {
	
	char Nombre[20];
	int Socket;
	
}Conectado;

typedef struct {
	
	Conectado Conectados[100];
	int Num;
	
}ListaConectados;

typedef struct {
	
	int num;
	char jugador1[80];
	char jugador2[80];
	
}Partida;

typedef struct {
	
	Partida Partidas[50];
	
}ListaPartidas;
ListaPartidas miListaPartidas;
ListaConectados miLista;
int sockets[100];
int SocketsCreados;
int numeroAleatorio;
int Resultado;
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
int Eliminar(ListaConectados *Lista, char nombre[80], int socket)
{
	int Posicionp = Posicion(Lista, nombre);

	if (Posicionp != -1)
	{
		for (int i = Posicionp; i < Lista->Num; i++)
		{
			Lista->Conectados[i] = Lista->Conectados[i+1];
		}
		
		Lista->Num = (Lista->Num)-1;
		char respuesta[512];
		sprintf(respuesta, "7/");
		write (socket, respuesta, strlen(respuesta));	
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

void CrearPartida(ListaPartidas *Lista, char jugador1[80], char jugador2[80])
{
	pthread_mutex_lock(&mutex);
	int i;
	
	//Buscamos un hueco para poner una partida en nuestra tabla de partidas
	while (Lista->Partidas[i].num != -1)
	{
		i++;
	}
	
	//Inicializamos la partida con los nombres de los jugadores y reinicializamos el marcador
	Lista->Partidas[i].num = i;
	strcpy(Lista->Partidas[i].jugador1, jugador1);
	strcpy(Lista->Partidas[i].jugador2, jugador2);
	pthread_mutex_unlock(&mutex);
}

void RegistrarCuenta(MYSQL *conn, char Usuario[80], char Contrasena[80], char Respuesta[512])
{	
	pthread_mutex_lock(&mutex);
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[1250];
	int res;
	
	strcpy (consulta,"SELECT Jugador.Nombre FROM Jugador WHERE Jugador.Nombre = '");
	strcat (consulta, Usuario);
	strcat (consulta,"'");
	
	res = mysql_query (conn, consulta);
	if (res != 0)
	{
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
	}
	else if (res == 0)
	{
		//Recogemos el resultado de la consulta en una
		//tabla virtual MySQL
		resultado = mysql_store_result (conn);
		
		//Recogemos el resultado de la primera fila
		row = mysql_fetch_row (resultado);
		
		//Si no encuentra ning?n usuario con ese nombre
		if (row == NULL)
		{
			//Abrimos otra vez MySQL para poder contar el n?mero total de jugadores
			memset(consulta, 0, strlen(consulta));
			strcpy (consulta,"SELECT Jugador.IdJ FROM Jugador");
			int numconsulta = mysql_query (conn, consulta);
			if (numconsulta != 0)
			{
				printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
				sprintf(Respuesta, "8/%s/ERROR", Usuario);
				
			}
			else if (numconsulta == 0)
			{
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				int NumeroJugadoresInicial;
				char NumeroJugadoresFinal[100];
				
				while(row != NULL)
				{
					NumeroJugadoresInicial = atoi(row[0]);
					
					//Obtenemos la siguiente fila para el siguiente loop
					row = mysql_fetch_row (resultado);
				}
				
				NumeroJugadoresInicial++;
				sprintf(NumeroJugadoresFinal, "%d", NumeroJugadoresInicial);
				
				memset(consulta, 0, strlen(consulta));
				strcpy (consulta, "INSERT INTO Jugador VALUES (");
				strcat (consulta, NumeroJugadoresFinal);
				strcat (consulta, ", '");
				strcat (consulta, Usuario);
				strcat (consulta, "', '");
				strcat (consulta, Contrasena);
				strcat (consulta, "')");
				printf ("consulta: %s\n", consulta);
				resultado = mysql_query (conn, consulta);
				if (resultado == 0)
				{
					sprintf(Respuesta, "8/%s/SI", Usuario);
					
				}
				else if (resultado != 0)
				{
					printf ("Error al introducir datos en la base %u %s\n", mysql_errno(conn), mysql_error(conn));
					sprintf(Respuesta, "8/%s/ERROR", Usuario);
					
				}
				
			}
			
		}
		
		//Si se encuentra un usuario con ese nombre
		else if (row != NULL)
		{
			sprintf(Respuesta, "8/%s/NO", Usuario);
			
		}
	}
	
	
	pthread_mutex_unlock(&mutex);
}
void  QuitarCuenta(MYSQL *conn, char Usuario[80], char Contrasena[80], char Respuesta[512])
{	
	pthread_mutex_lock(&mutex);
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[1250];
	int res;
	
	strcpy (consulta,"SELECT Jugador.Nombre FROM Jugador WHERE NOT Jugador.Nombre = '");
	strcat (consulta, Usuario);
	strcat (consulta,"'");
	printf ("consulta: %s\n", consulta);
	res = mysql_query (conn, consulta);
	if (res != 0)
	{
		printf ("error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
	}
	else if (res == 0)
	{
		resultado = mysql_store_result (conn);

		row = mysql_fetch_row (resultado);
		
		if (row != NULL)
		{
			printf("halo\n");
			
			memset(consulta, 0, strlen(consulta));
			strcpy (consulta,"SELECT Jugador.IdJ FROM Jugador WHERE Jugador.Nombre = '");
			strcat (consulta, Usuario);
			strcat (consulta,"'");
			printf("consulta2: %s\n", consulta);
			int numc = mysql_query (conn, consulta);
			printf("numc: %d\n", numc);
			if (numc != 0)
			{
				printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
				sprintf(Respuesta, "9/%s/ERROR", Usuario);
				write (socket, Respuesta, strlen(Respuesta));
			}
			else if (numc == 0)
			{
				printf("klk\n");
				
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row (resultado);
				int NumeroJugador = atoi(row[0]);
				printf("n: %d\n", NumeroJugador);
				memset(consulta, 0, strlen(consulta));
				sprintf(consulta, "DELETE FROM Jugador WHERE Jugador.Nombre = '%s' AND Jugador.IdJ = %d AND Jugador.Contrasena = '%s'", Usuario, NumeroJugador, Contrasena);

				printf ("consulta: %s\n", consulta);
				resultado = mysql_query (conn, consulta);
				if (resultado == 0)
				{
					sprintf(Respuesta, "9/%s/SI", Usuario);
					printf ("resp: %s\n", Respuesta);
					write (socket, Respuesta, strlen(Respuesta));
				}
				else if (resultado != 0)
				{
					printf ("Error al introducir datos en la base %u %s\n", mysql_errno(conn), mysql_error(conn));
					sprintf(Respuesta, "9/%s/ERROR", Usuario);
					write (socket, Respuesta, strlen(Respuesta));
				}				
			}			
		}
		else if (row = NULL)
		{
			printf("jkj\n");
			sprintf(Respuesta, "9/%s/NO", Usuario);
			write (socket, Respuesta, strlen(Respuesta));
		}
	}
	pthread_mutex_unlock(&mutex);
	
}

void RegistrarPartida(MYSQL *conn, char Usuario[80], char Contrincante[80], char Fecha[20], char Hora[20], int Duracion, char Respuesta[512], int Ganancias, int Perdidas)
{	
	
	pthread_mutex_lock(&mutex);

	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[512];
	strcpy (consulta,"SELECT COUNT (*) FROM Partida");
	Resultado = mysql_query (conn, consulta);
	int IdP = Resultado + 1;
	sprintf(consulta, "INSERT INTO Partida VALUES(%d, '%s', '%s', '%s', '%s', %d)", IdP, Usuario, Contrincante, Fecha, Hora, Duracion);
	mysql_query (conn, consulta);
	
	strcpy (consulta,"SELECT Jugador.IdJ FROM Jugador WHERE Jugador.Nombre = '");
	strcat (consulta, Usuario);
	strcat (consulta, "'");
	Resultado = mysql_query (conn, consulta);
	
	resultado = mysql_store_result (conn);
	
	row = mysql_fetch_row (resultado);
	
	int IdJ;
	IdJ = atoi(row[0]);
	
	sprintf(consulta, "INSERT INTO Historial VALUES(%d, %d, %d, %d)", IdJ, IdP, Ganancias, Perdidas);
	mysql_query (conn, consulta);
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
	char invitado[50];
	char consulta[512];
	int totalganado = 0;
	int gananciatotal = 0;
	
	
	MYSQL *conn;
	int err;
	int err2;
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
		char dia[20];
		char mes[20];
		char ano[20];
		
		
		if (codigo ==4) //Iniciar sesion
		{
			p = strtok( NULL, "/");			
			strcpy (nombre, p);
			p = strtok ( NULL, "/");
			strcpy (contrasena, p);	
			printf ("nombre: %s , contrasena: %s\n", nombre, contrasena);
			sprintf (consulta, "SELECT Jugador.Nombre FROM Jugador WHERE Jugador.Nombre = '%s'", nombre);
			printf("mi consulta es: %s\n", consulta);
			err2 = mysql_query (conn, consulta);
			resultado = mysql_store_result (conn);
			row = mysql_fetch_row(resultado);
			printf("La contrasena es: %s\n", row[0]);
			if (err2 !=0){
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "4/No");
				exit(1);
			}
			sprintf (consulta, "SELECT Jugador.Contrasena FROM Jugador WHERE Jugador.Nombre = '%s'", nombre);
			printf("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			resultado = mysql_store_result (conn);
			row = mysql_fetch_row(resultado);
			printf("La contrasena es: %s\n", row[0]);
			if (err !=0){
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "4/No");
				exit(1);
			}
			else if (strcmp (row[0], contrasena) != 0){
				strcpy (respuesta, "4/No");
				printf("Contrasena incorrecta\n");
			}
			else if (strcmp (row[0], contrasena) == 0){
				strcpy (respuesta, "4/Si");
				printf("Estas conectado\n");
			}
			
			printf ("Voy a enviar respuesta\n");
			write (sock_conn,respuesta, strlen(respuesta));
			printf ("Ya he enviado\n");
			int anadido = AnadirConectado(&miLista, nombre, sock_conn);
			int socketj = BuscarSocket(&miLista, nombre);
			printf("socket: %d\n", socketj);
			printf("Jugadores: %d\n", miLista.Num);
			if ( miLista.Num > 0)
			{
				char TablaJugadoresConectados[1000];				
				ListaActivos(&miLista, TablaJugadoresConectados);
				sprintf(respuesta, "5/%s", TablaJugadoresConectados);
				printf("Conectados: %d\n", miLista.Num);

				for (int i = 0; i < miLista.Num; i++)
				{
					write(miLista.Conectados[i].Socket, respuesta, strlen(respuesta));
				}
			}
		}

		
		if (codigo ==0) //Desconectarse
		{						
			char jugador[80];
			p = strtok(NULL, "/");
			strcpy (jugador, p); //Ya tenemos el usuario
			int socketE = BuscarSocket(&miLista, jugador);
			Eliminar(&miLista, jugador, sock_conn);
			printf("Desconectamos el usuario %s\n", jugador);

			if (miLista.Num != 0 )
			{
				char TablaJugadoresConectados[1000];
				ListaActivos(&miLista, TablaJugadoresConectados);
				sprintf(respuesta, "5/%s", TablaJugadoresConectados);				
				for (int i = 0; i < miLista.Num; i++)
				{
					printf("Respuesta: %s\n", respuesta);
					if(miLista.Conectados[i].Socket != socketE)
						write(miLista.Conectados[i].Socket, respuesta, strlen(respuesta));
					printf("socket: %d\n", miLista.Conectados[i].Socket);
				}
				printf("socketMIO: %d\n", sock_conn);
			}
			strcpy(respuesta, "7/");
			write (sock_conn, respuesta, strlen(respuesta));
			terminar = 1;
			// Se acabo el servicio para este cliente
			close(sock_conn); 			
		}
		
		
		else if (codigo ==1)//Consulta numero 1
		{
			char Respuesta[512];
			char jugador[80];
			p = strtok(NULL, "/");
			strcpy (respuesta, "");
			strcpy (Respuesta, "");
			sprintf (consulta, "SELECT DISTINCT Jugador.Nombre FROM Jugador, Historial, Partida WHERE NOT Jugador.Nombre = '%s' AND (Partida.Jugador1 = '%s' OR Partida.Jugador2 = '%s') AND Jugador.IdJ = Historial.Jugador AND Historial.Partida = Partida.IdP", nombre, nombre, nombre);
			printf ("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			if (err !=0)
			{
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "1/No hay jugadores");
				exit(1);
			}
			else
			{
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row(resultado);
				if (row == NULL)
				{
					printf("Error al consultar la base de datos %u %s\n");
					strcpy (respuesta, "1/No hay jugadores");
				}
				else
				{
					while (row != NULL)
					{
						sprintf(Respuesta, "%s %s,", Respuesta, row[0]);
						row = mysql_fetch_row(resultado);					
					}
					printf("Respuesta1: %s\n", Respuesta);
					sprintf(respuesta, "1/%s", Respuesta);
					respuesta [strlen(respuesta) - 1] = '\0';
					printf("Respuesta: %s\n", respuesta);					
				}
			}
			printf("Mensaje enviado\n");
			write (sock_conn,respuesta, strlen(respuesta));			
		}
		
		
		
		else if (codigo ==2)//Consulta 2
		{
			char response[513];
			strcpy(response, "");
			char response2[513];
			strcpy(response2, "");
			char contrincante[20];
			strcpy (respuesta, "");
			p = strtok( NULL, "/");
			strcpy (nombre, p);
			p = strtok( NULL, "/");
			strcpy (contrincante, p);
			sprintf (consulta, "SELECT Historial.Ganancias FROM Jugador, Historial, Partida WHERE Jugador.Nombre = '%s' AND (Partida.Jugador1 = '%s' OR Partida.Jugador2 = '%s') AND (Partida.Jugador1 = '%s' OR Partida.Jugador2 = '%s') AND Jugador.IdJ = Historial.Jugador AND Partida.IdP = Historial.Partida ", nombre, nombre, nombre, contrincante, contrincante);
			printf ("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			if (err !=0)
			{
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "2/No");
				exit(1);
			}
			else
			{
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row(resultado);
				if (row == NULL)
				{
					printf("Error al consultar la base de datos %u %s\n");
					strcpy (response, "No jugaste ninguna partida");
					
				}
				else
				{
					while (row != NULL)
					{
						sprintf(response, "%s %s,", response, row[0]);
						row = mysql_fetch_row(resultado);
					}
					printf("ganancias: %s\n", response);
					response [strlen(response) - 1] = '\0';
					sprintf (consulta, "SELECT Historial.Perdidas FROM Jugador, Historial, Partida WHERE Jugador.Nombre = '%s' AND (Partida.Jugador1 = '%s' OR Partida.Jugador2 = '%s') AND (Partida.Jugador1 = '%s' OR Partida.Jugador2 = '%s') AND Jugador.IdJ = Historial.Jugador AND Partida.IdP = Historial.Partida ", nombre, nombre, nombre, contrincante, contrincante);
					printf ("mi consulta es: %s\n", consulta);
					err = mysql_query (conn, consulta);
					if (err !=0)
					{
						printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
						strcpy (respuesta, "2/No");
						exit(1);
					}
					else
					{
						resultado = mysql_store_result (conn);
						row = mysql_fetch_row(resultado);
						if (row == NULL)
						{
							printf("Error al consultar la base de datos %u %s\n");
							strcpy (response2, "No jugaste ninguna partida");
						}	
							else
							{
								while (row != NULL)
								{
								sprintf(response2, "%s %s,", response2, row[0]);
								row = mysql_fetch_row(resultado);
								}	
							}
							printf("perdidas: %s\n", response2);
							response2 [strlen(response2) - 1] = '\0';
					    }
				    }
					sprintf(respuesta, "2/Si/%s/%s", response, response2);
					printf("%s\n", respuesta);
			    }
				printf("Mensaje enviado\n");
				write (sock_conn,respuesta, strlen(respuesta));
			}
			
			
			
		else if (codigo ==3)//Consulta 3
		{
			char response[513];
			strcpy(response, "");
			strcpy (respuesta, ""); 
			p = strtok( NULL, "/");
			strcpy (nombre, p);
			p = strtok ( NULL, "/");
			strcpy (dia, p);
			p = strtok ( NULL, "/");
			strcpy (mes, p);
			p = strtok ( NULL, "/");
			strcpy (ano, p);
			sprintf(fecha, "%s/%s/%s", dia, mes, ano);
			printf ("nombre: %s , fecha: %s\n", nombre, fecha);
			sprintf (consulta, "SELECT Historial.Partida FROM Jugador, Partida, Historial WHERE Jugador.Nombre = '%s' AND Jugador.IdJ = Historial.Jugador AND Partida.Fecha = '%s' AND Partida.IdP = Historial.Partida", nombre, fecha);
			printf ("mi consulta es: %s\n", consulta);
			err = mysql_query (conn, consulta);
			if (err !=0)
			{
				printf("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
				strcpy (respuesta, "3/No");
				exit(1);
			}
			else
			{
				resultado = mysql_store_result (conn);
				row = mysql_fetch_row(resultado);
				if (row == NULL)
				{
					printf("Error al consultar la base de datos %u %s\n");
					strcpy (response, "Ninguna");
				}
				else
				{
					while (row != NULL)
					{
						sprintf(response, "%s %s,", response, row[0]);
						row = mysql_fetch_row(resultado);					
					}										
				}
				sprintf(respuesta, "3/%s", response);
				respuesta [strlen(respuesta) - 1] = '\0';
				printf("%s\n", respuesta);
			}
			printf("Mensaje enviado\n");
			write (sock_conn,respuesta, strlen(respuesta));
		}
		
		
		else if (codigo == 6)//gestionar las invitaciones
		{			
			char peticion[50];
			p = strtok(NULL, "/");
			strcpy (peticion, p);
			p = strtok(NULL, "/");
			strcpy (invitado, p);
			printf("peticion: %s, invitado: %s\n", peticion, invitado);
			
			int SocketInvitado = BuscarSocket(&miLista, invitado);			
			
			if (strcmp(peticion, "ENVIAR") == 0)
			{
				sprintf(respuesta, "6/RECIBIR/%s", nombre);
				printf("respuesta: %s\n", respuesta);
				write(SocketInvitado, respuesta, strlen(respuesta));
			}
			else if (strcmp(peticion, "ACEPTAR") == 0)
			{
				sprintf(respuesta, "6/SI/%s", nombre);
				write(SocketInvitado, respuesta, strlen(respuesta));
				srand(time(NULL));
				numeroAleatorio = rand() % 37;
				
			}
			else if (strcmp(peticion, "RECHAZAR") == 0)
			{
				sprintf(respuesta, "6/NO/%s", nombre);
				write(SocketInvitado, respuesta, strlen(respuesta));
			}
			
		}
		else if (codigo == 8) //Registrarse
		{
            char contrasena[80];
            p = strtok(NULL, "/");
            strcpy (nombre, p); // Ya tenemos el usuario
            p = strtok(NULL, "/");
            strcpy (contrasena, p); //Conseguimos la contrasena
            printf ("Codigo: %d, Nombre: %s, Contrasena: %s\n", codigo, nombre, contrasena);
            RegistrarCuenta(conn, nombre, contrasena, respuesta);			
			printf ("Respuesta: %s\n", respuesta);
			write (sock_conn, respuesta, strlen(respuesta));
        }
		
		
		 else if (codigo == 9) //Darse de baja
		{
            char contrasena[80];
            p = strtok(NULL, "/");
            strcpy (nombre, p); // Ya tenemos el usuario
            p = strtok(NULL, "/");
            strcpy (contrasena, p); //Conseguimos la contrasena
            printf ("Codigo: %d, Nombre: %s, Contrasena: %s\n", codigo, nombre, contrasena);
            QuitarCuenta(conn, nombre, contrasena, respuesta);
			printf ("hola\n");
            printf ("Respuesta: %s\n", respuesta);
            write (sock_conn, respuesta, strlen(respuesta));
        }
		 
		 
		else if (codigo == 10)//Gestion de la ruleta (el juego en si)
		{
			char jugada[50];
			int cantidad;
			int ganancia = 0;
			p = strtok(NULL, "/");
			strcpy (jugada, p);
			p = strtok(NULL, "/");
			cantidad = atoi(p);
			printf("jugada: %s, cantidad: %d \n",jugada,cantidad);
			printf("numerin: %d \n", numeroAleatorio);
			if (strcmp(jugada, "P") == 0 && ((numeroAleatorio%2)==0))
			{
				ganancia = ganancia + cantidad*2;
			}
			else if (strcmp(jugada, "I") == 0 && ((numeroAleatorio%2)==1))
			{
				ganancia = ganancia + cantidad*2;
			}
			else if (strcmp(jugada, "N") ==0)
			{
				int negro[] = {2 , 4 , 6 , 8 , 10 , 11 , 13 , 15 , 17 , 20 , 22 , 24 , 26 , 28 , 29 , 31 , 33 , 35};
				int i;
				for (i=0; i < 17; i++)
				{
					if (numeroAleatorio == negro[i])
					{
						ganancia = ganancia + cantidad*2;
					}
				}
			}
		
			else if (strcmp(jugada, "R") ==0)
			{
				int rojo []= {1 , 3 , 5 , 7 , 9 , 12 , 14 , 16 , 18 , 19 , 21 , 25 , 23 , 27 , 30 , 32 , 34 , 36};
				for(int i=0; i<17; i++)
				{
					if (numeroAleatorio == rojo[i])
					{
						ganancia = ganancia + cantidad*2;
					}
				}
			}
			else if (strcmp(jugada, "D1") ==0)
			{
				int primdocena[]= {1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12};
				for(int i=0; i<11; i++)
				{
					if (numeroAleatorio == primdocena[i])
					{
						ganancia = ganancia + cantidad*3;
					}
				}
			}
			else if (strcmp(jugada, "D2") ==0)
			{
				int segdocena[]={13 , 14 , 15 , 16 , 17 , 18 , 19 , 20 , 21 , 22 , 23 , 24};
				for(int i=0; i<11; i++)
				{
					if (numeroAleatorio == segdocena[i])
					{
						ganancia = ganancia + cantidad*3;
					}
				}
			}	
			else if (strcmp(jugada, "D3") ==0)
			{
				int tercdocena[]= {25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36};
				for(int i=0; i<11; i++)
				{
					if (numeroAleatorio == tercdocena[i])
					{
						ganancia = ganancia + cantidad*3;
					}
				}
			}	
			else if (strcmp(jugada, "m") ==0)
			{
				int primeramitad[]= {1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 , 14 , 15 , 16 , 17 , 18};
				for(int i=0; i<17; i++)
				{
					if (numeroAleatorio == primeramitad[i])
					{
						ganancia = ganancia + cantidad*2;
					}
				}
			}	
			else if (strcmp(jugada, "M") ==0)
			{
				int segundamitad[]= {19 , 20 , 21 , 22 , 23 , 24 , 25 , 26 , 27 , 28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 , 36};
				for(int i=0; i<17; i++)
				{
					if (numeroAleatorio == segundamitad[i])
					{
						ganancia = ganancia + cantidad*2;
					}
				}
			}	
			else if (strcmp(jugada, "F1") ==0)
			{
				int primerafila[]={3 , 6 , 9 , 12 , 15 , 18 , 21 , 24 , 27 , 30 , 33 , 36};
				for(int i=0; i<11; i++)
				{
					if (numeroAleatorio == primerafila[i])
					{
						ganancia = ganancia + cantidad*36;
					}
				}
			}	
			else if (strcmp(jugada, "F2") ==0)
			{
				int segundafila[]= {2 , 5 , 8 , 11 , 14 , 17 , 20 , 23 , 26 , 29 , 32 , 35};
				for(int i=0; i<11; i++)
				{
					if (numeroAleatorio == segundafila[i])
					{
						ganancia = ganancia + cantidad*3;
					}
				}
			}
			else if (strcmp(jugada, "F3") ==0)
			{
				int tercerafila[]= {1 , 4 , 7 , 10 , 13 , 16 , 19 , 22 , 25 , 28 , 31 , 34};
				for(int i=0; i<11; i++)
				{
					if (numeroAleatorio == tercerafila[i])
					{
						ganancia = ganancia + cantidad*3;
					}
				}
			}	
			else
			{
				int numjugada = atoi(jugada);
				if(numjugada == numeroAleatorio)
				{
					ganancia = ganancia + cantidad*36;
				}
			}
			gananciatotal = gananciatotal + ganancia;
			totalganado = totalganado - cantidad + ganancia;
			printf("Dinerototal: %d, dinerototalganado: %d\n", gananciatotal, totalganado);
			sprintf(respuesta, "10/%d/%d", totalganado, numeroAleatorio);
			printf("respuesta: %s \n",respuesta);
			write(sock_conn, respuesta, strlen(respuesta));			
		}
		
		
		else if (codigo ==12)//Generamos el numero aleatorio
		{
			printf("numerinanterior: %d\n", numeroAleatorio);
			numeroAleatorio = rand() % 37;
			printf("numerinactual: %d\n", numeroAleatorio);
		}
		
		
		else if (codigo == 13)//Preguntamos el numero que saldra en la ruleta
		{
			printf("numerini: %d\n", numeroAleatorio);
			printf("saldo: %d\n", totalganado);
			sprintf(respuesta, "13/%d", numeroAleatorio);
			write(sock_conn, respuesta, strlen(respuesta));
		}
		

		else if (codigo == 14) //El saldo actualizado
		{
			printf("saldoganado: %d\n", totalganado);
			sprintf(respuesta, "14/%d", totalganado);
			write(sock_conn, respuesta, strlen(respuesta));
		}
		else if (codigo == 15)//Registra los datos de la partida
		{
			char Usuario[20];
			char Contrincante[20];
			p = strtok( NULL, "/");
			strcpy (Usuario, p);
			p = strtok ( NULL, "/");
			strcpy (Contrincante, p);				
			int perdidas;
			int ganancias;
			char fecha[100];
			char hora[100];
			time_t t;
			time_t tiempo_actual;
			struct tm *tm;
			struct tm *info_tiempo;
			t=time(NULL);
			tm=localtime(&t);
			strftime(fecha, 100, "%d/%m/%Y", tm);
			printf ("Hoy es: %s\n", fecha);
			time(&tiempo_actual);
			info_tiempo = localtime(&tiempo_actual);			
			// Acceder a los componentes de la hora
			sprintf(hora, "%02d:%02d\n", info_tiempo->tm_hour, info_tiempo->tm_min);
			printf ("%s\n", hora);			
			if(totalganado < 0)
			{
				ganancias = 0;
				perdidas = totalganado * (-1);
				printf ("Ganancias: 0, Perdidas: %d\n", perdidas);
			}
			else 
			{
				ganancias = totalganado;
				perdidas = 0;
				printf ("Ganancias: %d, Perdidas: 0\n", ganancias);
			}
			RegistrarPartida(conn, Usuario, Contrincante, fecha, hora, 28, respuesta, ganancias, perdidas);
			printf ("La partida ha sido anadida a la base de datos\n");			
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
