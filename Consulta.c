#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>



int main(int argc, char *argv[]) {
	
	MYSQL *conn;
	char consulta[100];
	
	
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
	conn = mysql_real_connect (conn, "localhost","root", "mysql", "ruleta",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion: %u %s\n", 
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	err=mysql_query (conn, "SELECT Jugador.Nombre FROM Jugador");
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL){
		printf ("No se han obtenido datos en la consulta\n");
	}
	else
		while (row !=NULL) {
			
			// las columnas 0 y 1 contienen DNI y nombre 
			printf ("nombre: %s\n", row[0]);
			// obtenemos la siguiente fila
			row = mysql_fetch_row (resultado);
		}
		
	mysql_close (conn);
    exit(0);
}
